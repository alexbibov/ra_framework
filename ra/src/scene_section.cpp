#include "context.h"
#include "scene_section.h"
#include "geometry_group.h"
#include "ray_generator.h"

using namespace ra;

RaSceneSection::RaSceneSection(RaContext const& context, RaBVHAlgorithm acceleration_structure_construction_algorithm) :
    RaContractWithRaContext{ context },
    RaTransformable{ context },
    m_construction_begun{ false },
    m_construction_finished{ false }
{
    RTgroup group_native_handle{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGroupCreate(nativeOptiXContextHandle(), &group_native_handle));
    m_native_group_handle.reset(group_native_handle,
        [this](RTgroup group)->void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtGroupDestroy(group));
    });

    RTacceleration acceleration_native_handle{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationCreate(nativeOptiXContextHandle(), &acceleration_native_handle));
    m_native_acceleration_handle.reset(acceleration_native_handle,
        [this](RTacceleration a)->void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationDestroy(a));
    });

    char const* acceleration_structure_construction_algorithm_name{ nullptr };
    switch (acceleration_structure_construction_algorithm)
    {
    case RaBVHAlgorithm::trbvh:
        acceleration_structure_construction_algorithm_name = "Trbvh";
        break;
    case RaBVHAlgorithm::sbvh:
        acceleration_structure_construction_algorithm_name = "Sbvh";
        break;
    case RaBVHAlgorithm::bvh:
        acceleration_structure_construction_algorithm_name = "Bvh";
        break;
    case RaBVHAlgorithm::none:
        acceleration_structure_construction_algorithm_name = "NoAccel";
        break;
    }

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationSetBuilder(acceleration_native_handle, acceleration_structure_construction_algorithm_name));
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGroupSetAcceleration(group_native_handle, acceleration_native_handle));
}

void RaSceneSection::beginConstruction()
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

void RaSceneSection::addGeometryGroup(RaGeometryGroup const& geometry_group)
{
    if (!m_construction_begun)
    {
        THROW_RA_ERROR("Error while adding geometry group \"" + geometry_group.getStringName() + "\" into scene section \""
            + getStringName() + "\": beginConstruction() command bracked must be invoked prior to construction of scene sections");
    }
    if (m_construction_finished)
    {
        THROW_RA_ERROR("Error while adding geometry group \"" + geometry_group.getStringName() + "\" into scene section \""
            + getStringName() + "\": construction of the scene section has already been finalized. When finalized, scene sections "
            "become effectively immutable and no new geometry groups can be added into them");
    }

    if (!geometry_group.isMaterialAssignmentDelayed() && !geometry_group.isValid())
    {
        THROW_RA_ERROR("Geometry group \"" + geometry_group.getStringName() + "\" attempted to be added into scene section \""
            + getStringName() + "\" is not valid");
    }

    m_geometry_groups.push_back(geometry_group);
}

void RaSceneSection::addSceneSection(RaSceneSection const& scene_section)
{
    if (!m_construction_begun)
    {
        THROW_RA_ERROR("Error while joining scene section \"" + getStringName()
        + "\" with scene section \"" + scene_section.getStringName() + "\": before the joining operation the former scene section "
            "should have been initialized for construction by invoking beginConstruction() command bracket");
    }
    if (m_construction_finished)
    {
        THROW_RA_ERROR("Error while joining scene section \"" + getStringName()
        + "\" with scene section\"" + scene_section.getStringName() + "\": construction of the former scene section "
            "has already been finalized");
    }

    if (!scene_section.isConstructionDelayed() && !scene_section.isValid())
    {
        THROW_RA_ERROR("Scene section \"" + scene_section.getStringName() + "\" attempted to be joined with "
        "scene section \"" + getStringName() + "\" is not valid");
    }

    m_attached_scene_sections.push_back(scene_section);
}

void RaSceneSection::endConstruction()
{
    if (isConstructionDelayed())
        return;

    if (!m_construction_begun)
    {
        util::Log::retrieve()->out("WARNING: attempt to finalize construction of scene section \"" + getStringName()
            + "\" before construction of this scene section has been initialized (was endConstruction() invoked before beginConstruction()?) "
            "The invocation of endConstruction() will have no effect", util::LogMessageType::exclamation);
        return;
    }

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGroupSetChildCount(m_native_group_handle.get(), 
        static_cast<unsigned int>(m_geometry_groups.size() + m_attached_scene_sections.size())));

    uint32_t idx{ 0U };
    for (auto const& gg : m_geometry_groups)
    {
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGroupSetChild(m_native_group_handle.get(), idx,
            RaGeometryGroupAttorney<RaSceneSection>::getGeometryGroupNativeHandle(gg)));
        ++idx;
    }

    for (auto const& ss : m_attached_scene_sections)
    {
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGroupSetChild(m_native_group_handle.get(), idx,
            ss.m_native_group_handle.get()));
        ++idx;
    }

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationMarkDirty(m_native_acceleration_handle.get()));

    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtGroupValidate(m_native_group_handle.get()));

    m_construction_begun = false;
    m_construction_finished = true;
}

bool RaSceneSection::isConstructionDelayed() const
{
    for (auto const& ss : m_attached_scene_sections)
    {
        if (ss.isConstructionDelayed())
            return true;
    }

    for (auto const& gg : m_geometry_groups)
    {
        if (gg.isMaterialAssignmentDelayed())
            return true;
    }

    return false;
}

std::list<RaSceneSection>& RaSceneSection::sceneSections()
{
    return m_attached_scene_sections;
}

std::list<RaGeometryGroup>& RaSceneSection::geometryGroups()
{
    return m_geometry_groups;
}

bool RaSceneSection::isValid() const
{
    RTresult group_validation_result;
    RTresult acceleration_validation_result;
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), group_validation_result = rtGroupValidate(m_native_group_handle.get()));
    LOG_OPTIX_ERROR(nativeOptiXContextHandle(), acceleration_validation_result = rtAccelerationValidate(m_native_acceleration_handle.get()));

    return group_validation_result == RT_SUCCESS
        && acceleration_validation_result == RT_SUCCESS
        && (m_construction_begun && m_construction_finished);
}

void RaSceneSection::trace(RaRayGenerator const& ray_caster) const
{
    _update(ray_caster, RaObjectHandle{ getEntryNode() });
    _trace(ray_caster);
}

RTobject RaSceneSection::getEntryNode() const
{
    return isTransformApplied() ?
        static_cast<RTobject>(getNativeOptiXTransformHandle()) :
        static_cast<RTobject>(m_native_group_handle.get());
}

bool RaSceneSection::_update(RaRayGenerator const& ray_caster, RaObjectHandle top_scene_object) const
{
    bool rv{ false };
    for (auto& gg : m_geometry_groups)
    {
        if (RaGeometryGroupAttorney<RaSceneSection>::updateGeometryGroup(gg, top_scene_object) && !rv)
        {
            THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationMarkDirty(m_native_acceleration_handle.get()));
            rv = true;
        }
    }

    for (auto& ss : m_attached_scene_sections)
    {
        if (ss._update(ray_caster, top_scene_object) && !rv)
        {
            THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtAccelerationMarkDirty(m_native_acceleration_handle.get()));
            rv = true;
        }
    }

    RaRayGeneratorAttorney<RaSceneSection>::updateRayGenerator(ray_caster, top_scene_object);

    return rv;
}

void RaSceneSection::_trace(RaRayGenerator const& ray_caster) const
{
    ray_caster.getRayGenerationShader().setVariableValue("ra_entry_node", RaObjectHandle{ getEntryNode() });
    RaRayGeneratorAttorney<RaSceneSection>::launchRayGenerator(ray_caster);
}

RTobject RaSceneSection::getObjectToBeTransformed() const
{
    return m_native_group_handle.get();
}
