#include "context.h"
#include "geometry_group.h"
#include "scene_section.h"
#include "ray_generator.h"


using namespace ox_wrapper;

uint32_t OxGeometryGroup::getNumberOfGeometries() const
{
    unsigned int count;
    throwOptiXContextError(rtGeometryGroupGetChildCount(m_native_geometry_group.get(), &count));
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
        throw OxException{ ("Error while adding geometry \"" + geometry.getStringName() + "\" into geometry group \""
            + getStringName() + "\": beginConstruction() command bracked must be invoked prior to construction of geometry groups").c_str(),
            __FILE__, __FUNCTION__, __LINE__ };
    }

    if (m_construction_finished)
    {
        throw OxException{ ("Error while adding geometry \"" + geometry.getStringName() + "\" into geometry group \""
        + getStringName() + "\": construction of the geometry group has already been finalized. When finalized, geometry groups "
        "become effectively immutable and no new geometries can be added into them").c_str(),
        __FILE__, __FUNCTION__, __LINE__ };
    }

    if (!geometry.isValid())
    {
        throw OxException{ ("Geometry \"" + geometry.getStringName() + "\" attempted to be added into geometry group \""
            + getStringName() + "\" is not valid").c_str(), __FILE__, __FUNCTION__, __LINE__ }; 
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

    throwOptiXContextError(rtGeometryGroupSetChildCount(m_native_geometry_group.get(), static_cast<unsigned int>(m_list_of_geometries.size())));
    unsigned int idx{ 0U };
    for (auto const& g : m_list_of_geometries)
    {
        throwOptiXContextError(rtGeometryGroupSetChild(m_native_geometry_group.get(), idx,
            OxMaterialAssemblyAttorney<OxGeometryGroup>::getNativeGeometryInstanceHandle(g.getMaterialAssembly())));
        ++idx;
    }
    throwOptiXContextError(rtAccelerationMarkDirty(m_native_acceleration.get()));

    m_construction_finished = true;
}

bool OxGeometryGroup::isValid() const
{
    RTresult geometry_group_validation_result = rtGeometryGroupValidate(m_native_geometry_group.get());
    RTresult acceleration_structure_validation_result = rtAccelerationValidate(m_native_acceleration.get());

    logOptiXContextError(geometry_group_validation_result);
    logOptiXContextError(acceleration_structure_validation_result);


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

RTobject OxGeometryGroup::getTransformedObject() const
{
    return m_native_geometry_group.get();
}

bool OxGeometryGroup::updateGeometryGroup()
{
    for (auto& g : m_list_of_geometries)
    {
        if (OxGeometryAttorney<OxGeometryGroup>::isGeometryDirty(g))
        {
            throwOptiXContextError(rtAccelerationMarkDirty(m_native_acceleration.get()));
            return true;
        }
    }

    return false;
}

OxGeometryGroup::OxGeometryGroup(OxContext const& optix_context, OxBVHAlgorithm acceleration_structure_construction_algorithm):
    OxContractWithOxContext{ optix_context },
    OxTransformable{ optix_context },
    m_construction_begun{ false },
    m_construction_finished{ false }
{
    RTgeometrygroup geometry_group_native_handle;
    throwOptiXContextError(rtGeometryGroupCreate(nativeOptiXContextHandle(), &geometry_group_native_handle));

    m_native_geometry_group.reset(geometry_group_native_handle, 
        [this](RTgeometrygroup gg) -> void
    {
        logOptiXContextError(rtGeometryGroupDestroy(gg));
    });

    RTacceleration acceleration_native;
    throwOptiXContextError(rtAccelerationCreate(nativeOptiXContextHandle(), &acceleration_native));
    m_native_acceleration.reset(acceleration_native, 
        [this](RTacceleration a) -> void
    {
        logOptiXContextError(rtAccelerationDestroy(a));
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
    throwOptiXContextError(rtAccelerationSetBuilder(acceleration_native, acceleration_structure_construction_algorithm_name));
    throwOptiXContextError(rtGeometryGroupSetAcceleration(m_native_geometry_group.get(), m_native_acceleration.get()));
}
