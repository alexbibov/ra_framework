#define _SCL_SECURE_NO_WARNINGS

#include "rendering_passes_factory.h"
#include "../util/lua_support.h"
#include "../scene_section.h"

using namespace ra::rendering_passes;
using namespace ra::util;
using namespace ra;

namespace {

static RaRenderingPassesFactory* p_factory_instance{ nullptr };

}

RaRenderingPassesFactory* RaRenderingPassesFactory::initialize(RaContext const& context)
{
    if (!p_factory_instance)
        p_factory_instance = new RaRenderingPassesFactory{ context };

    return p_factory_instance;
}

void RaRenderingPassesFactory::shutdown()
{
    if (p_factory_instance)
    {
        delete p_factory_instance;
        p_factory_instance = nullptr;
    }
}

RaRenderingPassesFactory* RaRenderingPassesFactory::retrieve()
{
    return p_factory_instance;
}

bool RaRenderingPassesFactory::isValid() const
{
    return static_cast<bool>(p_factory_instance);
}

RaScatteringRenderingPass RaRenderingPassesFactory::createScatteringRenderingPass(
    RaSceneSection const& target_scene_section, 
    RaRayGenerator const& ray_caster,
    uint8_t num_spectra_pairs_supported,
    uint32_t max_recursion_depth, 
    float ray_marching_step_size, 
    uint32_t num_scattering_integral_importance_directions, 
    RaProgram const& absorption_probability_shader, 
    RaProgram const& scattering_probability_shader, 
    RaProgram const& scattering_phase_function_shader)
{
    return RaScatteringRenderingPass{ target_scene_section, ray_caster, num_spectra_pairs_supported,
    max_recursion_depth, ray_marching_step_size, num_scattering_integral_importance_directions,
    absorption_probability_shader, scattering_probability_shader, scattering_phase_function_shader };
}

RaScatteringRenderingPass RaRenderingPassesFactory::createScatteringRenderingPass(
    RaSceneSection const& target_scene_section, 
    RaRayGenerator const& ray_caster,
    uint8_t num_spectra_pairs_supported, 
    uint32_t max_recursion_depth, 
    float ray_marching_step_size, 
    uint32_t num_scattering_integral_importance_directions)
{
    return RaScatteringRenderingPass{ target_scene_section, ray_caster, num_spectra_pairs_supported,
        max_recursion_depth, ray_marching_step_size, num_scattering_integral_importance_directions };
}

RaRenderingPassesFactory::RaRenderingPassesFactory(RaContext const& context):
    m_context{ context }
{
    lua_support::LuaState::registerType<RaRenderingPass>(
        "RaRenderingPass",

        lua_support::NoConstructor::make_initializer(),

        "targetSceneSection", &RaRenderingPass::targetSceneSection,
        "render", &RaRenderingPass::render
    );
        

    lua_support::LuaState::registerSubType<RaScatteringRenderingPass>(
        "RaScatteringRenderingPass",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaRenderingPass>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [this](
                RaSceneSection const& target_scene_section,
                RaRayGeneratorWithOutputBuffer const& ray_caster,
                uint8_t num_spectra_pairs_supported,
                uint32_t max_recursion_depth,
                float ray_marching_step_size,
                uint32_t num_scattering_integral_importance_directions,
                RaProgram const& absorption_probability_shader,
                RaProgram const& scattering_probability_shader,
                RaProgram const& scattering_phase_function_shader)
            {
                return createScatteringRenderingPass(
                    target_scene_section,
                    ray_caster,
                    num_spectra_pairs_supported,
                    max_recursion_depth,
                    ray_marching_step_size,
                    num_scattering_integral_importance_directions,
                    absorption_probability_shader,
                    scattering_probability_shader,
                    scattering_phase_function_shader);
            },

            [this](
                RaSceneSection const& target_scene_section,
                RaRayGeneratorWithOutputBuffer const& ray_caster,
                uint8_t num_spectra_pairs_supported,
                uint32_t max_recursion_depth,
                float ray_marching_step_size,
                uint32_t num_scattering_integral_importance_directions)
            {
                return createScatteringRenderingPass(
                    target_scene_section,
                    ray_caster,
                    num_spectra_pairs_supported,
                    max_recursion_depth,
                    ray_marching_step_size,
                    num_scattering_integral_importance_directions);
            }
        ),

        "updateImportanceDirections",
        [](RaScatteringRenderingPass* p, 
            lua_support::LuaTable::table_type const& data)
        {
            uint32_t num_importance_directions = p->getNumberOfScatteringIntegralImportanceDirections();
            uint32_t num_spectra_pairs_supported = p->getNumberOfSpectraPairsSupported();
            uint32_t num_elements = num_importance_directions * (1 + num_spectra_pairs_supported);
            if (data.size() != num_elements)
                THROW_OX_WRAPPER_ERROR("updateImportanceDirections(...) must supply " +
                    std::to_string(num_elements) + " elements, but " 
                    + std::to_string(data.size()) + " elements were provided instead");

            auto converted_data = lua_support::LuaTable::toVector<float2>(data);
            float2* p_buffer = p->mapImportanceDirectionsBuffer();
            std::copy(converted_data.begin(), converted_data.end(), p_buffer);
            p->unmapImportanceDirectionsBuffer();
        },

        "getMaxRecursionDepth", &RaScatteringRenderingPass::getMaxRecursionDepth,
        "setMaxRecursionDepth", &RaScatteringRenderingPass::setMaxRecursionDepth,

        "getNumberOfScatteringIntegralImportanceDirections", &RaScatteringRenderingPass::getNumberOfScatteringIntegralImportanceDirections,
        "getNumberOfSpectraPairsSupported", &RaScatteringRenderingPass::getNumberOfSpectraPairsSupported,

        "getRayMarchingStepSize", &RaScatteringRenderingPass::getRayMarchingStepSize,
        "setRayMarchingStepSize", &RaScatteringRenderingPass::setRayMarchingStepSize,

        "getAbsorptionProbabilityShader", &RaScatteringRenderingPass::getAbsorptionProbabilityShader,
        "setAbsorptionProbabilityShader", &RaScatteringRenderingPass::setAbsorptionProbabilityShader,
        
        "getScatteringProbabilityShader", &RaScatteringRenderingPass::getScatteringProbabilityShader,
        "setScatteringProbabilityShader", &RaScatteringRenderingPass::setScatteringProbabilityShader,

        "getScatteringPhaseFunctionShader", &RaScatteringRenderingPass::getScatteringPhaseFunctionShader,
        "setScatteringPhaseFunctionShader", &RaScatteringRenderingPass::setScatteringPhaseFunctionShader
    );
}
