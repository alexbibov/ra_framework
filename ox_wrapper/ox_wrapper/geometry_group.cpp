#include "context.h"
#include "geometry_group.h"
#include "scene_section.h"
#include "ray_generator.h"


using namespace ox_wrapper;

uint32_t OxGeometryGroup::getNumberOfGeometries() const
{
    unsigned int count;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGroupGetChildCount(m_native_geometry_group.get(), &count));
    return static_cast<uint32_t>(count);
}

void OxGeometryGroup::beginConstruction()
{
    if (m_construction_begun)
    {
        util::Log::retrieve()->out("WARNING: construction of geometry group \""
            + getStringName() + "\" has already been started. Repetitive invocations of beginConstruction() are ignored", util::LogMessageType::exclamation);
    }
    if (m_construction_finished)
    {
        util::Log::retrieve()->out("WARNING: attempt to re-initialize construction of geometry group \""
            + getStringName() + "\", which has already been finalized. Repetitive invocations of beginConstruction() are ignored", util::LogMessageType::exclamation);
        return;
    }
    m_construction_begun = true;
}

void OxGeometryGroup::addGeometry(OxGeometry const& geometry)
{
    if (!m_construction_begun)
    {
        THROW_OX_WRAPPER_ERROR("Error while adding geometry \"" + geometry.getStringName() + "\" into geometry group \""
            + getStringName() + "\": beginConstruction() command bracked must be invoked prior to construction of geometry groups");
    }

    if (m_construction_finished)
    {
        THROW_OX_WRAPPER_ERROR("Error while adding geometry \"" + geometry.getStringName() + "\" into geometry group \""
        + getStringName() + "\": construction of the geometry group has already been finalized. When finalized, geometry groups "
        "become effectively immutable and no new geometries can be added into them");
    }

    if (!geometry.isValid())
    {
        THROW_OX_WRAPPER_ERROR("Geometry \"" + geometry.getStringName() + "\" added into geometry group \""
            + getStringName() + "\" is not valid"); 
    }

    m_list_of_geometries.push_back(geometry);
}

void OxGeometryGroup::endConstruction()
{
    if (!m_construction_begun)
    {
        util::Log::retrieve()->out("WARNING: attempt to finalize construction of geometry group \"" + getStringName()
            + "\" before construction of this group has been initialized (was endConstruction() invoked before beginConstruction()?) "
            "The invocation of endConstruction() will have no effect", util::LogMessageType::exclamation);
        return;
    }

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
        rtGeometryGroupSetChildCount(m_native_geometry_group.get(), static_cast<unsigned int>(m_list_of_geometries.size())));
    unsigned int idx{ 0U };
    for (auto const& g : m_list_of_geometries)
    {
        if(!OxMaterialAssemblyAttorney<OxGeometryGroup>::isDummyMaterialAssembly(g.getMaterialAssembly()))
        {
            THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGroupSetChild(m_native_geometry_group.get(), idx,
                OxMaterialAssemblyAttorney<OxGeometryGroup>::getNativeGeometryInstanceHandle(g.getMaterialAssembly())));
        }
        ++idx;
    }
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationMarkDirty(m_native_acceleration.get()));

    m_construction_finished = true;
}

std::list<OxGeometry> const& ox_wrapper::OxGeometryGroup::geometries() const
{
    return m_list_of_geometries;
}

bool OxGeometryGroup::isValid() const
{
    RTresult geometry_group_validation_result;
    RTresult acceleration_structure_validation_result;

    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), geometry_group_validation_result = rtGeometryGroupValidate(m_native_geometry_group.get()));
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), acceleration_structure_validation_result = rtAccelerationValidate(m_native_acceleration.get()));


    if (m_construction_begun && !m_construction_finished)
    {
        util::Log::retrieve()->out("WARNING: construction of geometry group \"" + getStringName()
            + "\" has been started, but has not been finalized yet. Geometry groups must be finalized before they "
            "can be added to scene sections", util::LogMessageType::exclamation);
    }

    return geometry_group_validation_result == RT_SUCCESS
        && acceleration_structure_validation_result == RT_SUCCESS
        && (m_construction_begun && m_construction_finished);
}

RTobject OxGeometryGroup::getObjectToBeTransformed() const
{
    return m_native_geometry_group.get();
}

bool OxGeometryGroup::update(OxObjectHandle top_scene_object) const
{
    bool rv{ false };
    unsigned int idx{ 0U };
    for (auto& g : m_list_of_geometries)
    {
        if (!OxMaterialAssemblyAttorney<OxGeometryGroup>::isDummyMaterialAssembly(g.getMaterialAssembly()))
        {
            THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGroupSetChild(m_native_geometry_group.get(), idx,
                OxMaterialAssemblyAttorney<OxGeometryGroup>::getNativeGeometryInstanceHandle(g.getMaterialAssembly())));
        }
        ++idx;

        if (!rv && OxGeometryAttorney<OxGeometryGroup>::isGeometryDirty(g))
        {
            THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationMarkDirty(m_native_acceleration.get()));
            rv = true;
        }

        OxGeometryAttorney<OxGeometryGroup>::updateGeometry(g, top_scene_object);
    }

    if (rv)
    {
        for (auto& g : m_list_of_geometries)
            OxGeometryAttorney<OxGeometryGroup>::markGeometryClean(g);
    }

    return rv;
}

OxGeometryGroup::OxGeometryGroup(OxContext const& optix_context, OxBVHAlgorithm acceleration_structure_construction_algorithm):
    OxContractWithOxContext{ optix_context },
    OxTransformable{ optix_context },
    m_construction_begun{ false },
    m_construction_finished{ false }
{
    RTgeometrygroup geometry_group_native_handle;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGroupCreate(nativeOptiXContextHandle(), &geometry_group_native_handle));

    m_native_geometry_group.reset(geometry_group_native_handle, 
        [this](RTgeometrygroup gg) -> void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGroupDestroy(gg));
    });

    RTacceleration acceleration_native;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationCreate(nativeOptiXContextHandle(), &acceleration_native));
    m_native_acceleration.reset(acceleration_native, 
        [this](RTacceleration a) -> void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationDestroy(a));
    });

    char const* acceleration_structure_construction_algorithm_name{ nullptr };
    switch (acceleration_structure_construction_algorithm)
    {
    case OxBVHAlgorithm::trbvh:
        acceleration_structure_construction_algorithm_name = "Trbvh";
        break;
    case OxBVHAlgorithm::sbvh:
        acceleration_structure_construction_algorithm_name = "Sbvh";
        break;
    case OxBVHAlgorithm::bvh:
        acceleration_structure_construction_algorithm_name = "Bvh";
        break;
    case OxBVHAlgorithm::none:
        acceleration_structure_construction_algorithm_name = "NoAccel";
        break;
    }
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationSetBuilder(acceleration_native, acceleration_structure_construction_algorithm_name));
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGeometryGroupSetAcceleration(m_native_geometry_group.get(), m_native_acceleration.get()));
}
