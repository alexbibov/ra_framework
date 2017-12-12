#include "context.h"
#include "scene_section.h"
#include "geometry_group.h"
#include "ray_generator.h"

using namespace ox_wrapper;

OxSceneSection::OxSceneSection(OxRayGenerator const& optix_ray_generator, OxBVHAlgorithm acceleration_structure_construction_algorithm) :
    OxContractWithOxContext{ optix_ray_generator.context() },
    OxTransformable{ optix_ray_generator.context() },
    m_optix_ray_generator{ optix_ray_generator },
    m_construction_begun{ false },
    m_construction_finished{ false }
{
    RTgroup group_native_handle{ nullptr };
    throwOptiXContextError(rtGroupCreate(nativeOptiXContextHandle(), &group_native_handle));
    m_native_group_handle.reset(group_native_handle,
        [this](RTgroup group)->void
    {
        logOptiXContextError(rtGroupDestroy(group));
    });

    RTacceleration acceleration_native_handle{ nullptr };
    throwOptiXContextError(rtAccelerationCreate(nativeOptiXContextHandle(), &acceleration_native_handle));
    m_native_acceleration_handle.reset(acceleration_native_handle,
        [this](RTacceleration a)->void
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

    throwOptiXContextError(rtAccelerationSetBuilder(acceleration_native_handle, acceleration_structure_construction_algorithm_name));
    throwOptiXContextError(rtGroupSetAcceleration(group_native_handle, acceleration_native_handle));
}

OxRayGenerator const& OxSceneSection::rayGenerator() const
{
    return m_optix_ray_generator;
}

void OxSceneSection::beginConstruction()
{
    if (m_construction_begun)
    {
        util::Log::retrieve()->out("WARNING: construction of scene section \""
            + getStringName() + "\" has already been started. Repetitive invocations of beginConstruction() are ignored", util::LogMessageType::exclamation);
    }
    if (m_construction_finished)
    {
        util::Log::retrieve()->out("WARNING: attempt to re-initialize construction of scene section \""
            + getStringName() + "\", which has already been finalized. Repetitive invocations of beginConstruction() are ignored", util::LogMessageType::exclamation);
        return;
    }
    m_construction_begun = true;
}

void OxSceneSection::addGeometryGroup(OxGeometryGroup const& geometry_group)
{
    if (!m_construction_begun)
    {
        throw OxException{ ("Error while adding geometry group \"" + geometry_group.getStringName() + "\" into scene section \""
            + getStringName() + "\": beginConstruction() command bracked must be invoked prior to construction of scene sections").c_str(),
        __FILE__, __FUNCTION__, __LINE__ };
    }
    if (m_construction_finished)
    {
        throw OxException{ ("Error while adding geometry group \"" + geometry_group.getStringName() + "\" into scene section \""
            + getStringName() + "\": construction of the scene section has already been finalized. When finalized, scene sections "
            "become effectively immutable and no new geometry groups can be added into them").c_str(),
            __FILE__, __FUNCTION__, __LINE__ };
    }

    if (!geometry_group.isValid())
    {
        throw OxException{ ("Geometry group \"" + geometry_group.getStringName() + "\" attempted to be added into scene section \""
            + getStringName() + "\" is not valid").c_str(), __FILE__, __FUNCTION__, __LINE__ };
    }

    m_geometry_groups.push_back(geometry_group);
}

void OxSceneSection::addSceneSection(OxSceneSection const& scene_section)
{
    if (!m_construction_begun)
    {
        throw OxException{ ("Error while joining scene section \"" + getStringName() 
        + "\" with scene section \"" + scene_section.getStringName() + "\": before the joining operation the former scene section "
            "should have been initialized for construction by invoking beginConstruction() command bracket").c_str(),
        __FILE__, __FUNCTION__, __LINE__ };
    }
    if (m_construction_finished)
    {
        throw OxException{ ("Error while joining scene section \"" + getStringName()
        + "\" with scene section\"" + scene_section.getStringName() + "\": construction of the former scene section "
            "has already been finalized").c_str(), __FILE__, __FUNCTION__, __LINE__ };
    }

    if (!scene_section.isValid())
    {
        throw OxException{ ("Scene section \"" + scene_section.getStringName() + "\" attempted to be joined with "
        "scene section \"" + getStringName() + "\" is not valid").c_str(), __FILE__, __FUNCTION__, __LINE__ };
    }

    m_attached_scene_sections.push_back(scene_section);
}

void OxSceneSection::endConstruction()
{
    if (!m_construction_begun)
    {
        util::Log::retrieve()->out("WARNING: attempt to finalize construction of scene section \"" + getStringName()
            + "\" before construction of this scene section has been initialized (was endConstruction() invoked before beginConstruction()?) "
            "The invocation of endConstruction() will have no effect", util::LogMessageType::exclamation);
        return;
    }

    throwOptiXContextError(rtGroupSetChildCount(m_native_group_handle.get(), 
        static_cast<unsigned int>(m_geometry_groups.size() + m_attached_scene_sections.size())));

    uint32_t idx{ 0U };
    for (auto const& gg : m_geometry_groups)
    {
        throwOptiXContextError(rtGroupSetChild(m_native_group_handle.get(), idx,
            OxGeometryGroupAttorney<OxSceneSection>::getGeometryGroupNativeHandle(gg)));
        ++idx;
    }

    for (auto const& ss : m_attached_scene_sections)
    {
        throwOptiXContextError(rtGroupSetChild(m_native_group_handle.get(), idx,
            ss.m_native_group_handle.get()));
        ++idx;
    }

    throwOptiXContextError(rtAccelerationMarkDirty(m_native_acceleration_handle.get()));

    m_construction_finished = true;
}

bool OxSceneSection::isValid() const
{
    RTresult group_validation_result = rtGroupValidate(m_native_group_handle.get());
    RTresult acceleration_validation_result = rtAccelerationValidate(m_native_acceleration_handle.get());
    logOptiXContextError(group_validation_result);
    logOptiXContextError(acceleration_validation_result);

    return group_validation_result == RT_SUCCESS
        && acceleration_validation_result == RT_SUCCESS
        && (m_construction_begun && m_construction_finished);
}

RTobject OxSceneSection::getEntryNode() const
{
    return isTransformApplied() ?
        static_cast<RTobject>(getNativeOptiXTransformHandle()) :
        static_cast<RTobject>(m_native_group_handle.get());
}

bool OxSceneSection::update() const
{
    bool rv{ false };
    for (auto& gg : m_geometry_groups)
    {
        if (OxGeometryGroupAttorney<OxSceneSection>::updateGeometryGroup(gg) && !rv)
        {
            throwOptiXContextError(rtAccelerationMarkDirty(m_native_acceleration_handle.get()));
            rv = true;
        }
    }

    for (auto& ss : m_attached_scene_sections)
    {
        if (ss.update() && !rv)
        {
            throwOptiXContextError(rtAccelerationMarkDirty(m_native_acceleration_handle.get()));
            rv = true;
        }
    }

    OxRayGeneratorAttorney<OxSceneSection>::updateRayGenerator(m_optix_ray_generator);

    return rv;
}

void OxSceneSection::runRayTracing() const
{
    m_optix_ray_generator.getRayGenerationShader().declareVariable("ox_entry_node", OxObjectHandle{ getEntryNode() });
    OxRayGeneratorAttorney<OxSceneSection>::launchRayGenerator(m_optix_ray_generator);
}

RTobject OxSceneSection::getTransformedObject() const
{
    return m_native_group_handle.get();
}
