#define _SCL_SECURE_NO_WARNINGS

#include "rendering_passes_factory.h"
#include "../util/lua_support.h"
#include "../scene_section.h"

using namespace ox_wrapper::rendering_passes;
using namespace ox_wrapper::util;
using namespace ox_wrapper;

namespace {

static OxRenderingPassesFactory* p_factory_instance{ nullptr };

}

OxRenderingPassesFactory* OxRenderingPassesFactory::initialize(OxContext const& context)
{
    if (!p_factory_instance)
        p_factory_instance = new OxRenderingPassesFactory{ context };

    return p_factory_instance;
}

void OxRenderingPassesFactory::shutdown()
{
    if (p_factory_instance)
    {
        delete p_factory_instance;
        p_factory_instance = nullptr;
    }
}

OxRenderingPassesFactory* OxRenderingPassesFactory::retrieve()
{
    return p_factory_instance;
}

bool OxRenderingPassesFactory::isValid() const
{
    return static_cast<bool>(p_factory_instance);
}

OxScatteringRenderingPass OxRenderingPassesFactory::createScatteringRenderingPass(
    OxSceneSection const& target_scene_section, 
    uint8_t num_spectra_pairs_supported,
    uint32_t max_recursion_depth, 
    float ray_marching_step_size, 
    uint32_t num_scattering_integral_importance_directions, 
    OxProgram const& absorption_probability_shader, 
    OxProgram const& scattering_probability_shader, 
    OxProgram const& scattering_phase_function_shader)
{
    return OxScatteringRenderingPass{ target_scene_section, num_spectra_pairs_supported,
    max_recursion_depth, ray_marching_step_size, num_scattering_integral_importance_directions,
    absorption_probability_shader, scattering_probability_shader, scattering_phase_function_shader };
}

OxScatteringRenderingPass OxRenderingPassesFactory::createScatteringRenderingPass(
    OxSceneSection const& target_scene_section, 
    uint8_t num_spectra_pairs_supported, 
    uint32_t max_recursion_depth, 
    float ray_marching_step_size, 
    uint32_t num_scattering_integral_importance_directions)
{
    return OxScatteringRenderingPass{ target_scene_section, num_spectra_pairs_supported,
        max_recursion_depth, ray_marching_step_size, num_scattering_integral_importance_directions };
}

OxRenderingPassesFactory::OxRenderingPassesFactory(OxContext const& context):
    m_context{ context }
{
    lua_support::LuaState::registerType<OxRenderingPass>(
        "OxRenderingPass",

        lua_support::NoConstructor::make_initializer(),

        "targetSceneSection", &OxRenderingPass::targetSceneSection,
        "render", &OxRenderingPass::render
    );
        

    lua_support::LuaState::registerSubType<OxScatteringRenderingPass>(
        "OxScatteringRenderingPass",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxRenderingPass>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [this](
                OxSceneSection const& target_scene_section,
                uint8_t num_spectra_pairs_supported,
                uint32_t max_recursion_depth,
                float ray_marching_step_size,
                uint32_t num_scattering_integral_importance_directions,
                OxProgram const& absorption_probability_shader,
                OxProgram const& scattering_probability_shader,
                OxProgram const& scattering_phase_function_shader)
            {
                return createScatteringRenderingPass(
                    target_scene_section,
                    num_spectra_pairs_supported,
                    max_recursion_depth,
                    ray_marching_step_size,
                    num_scattering_integral_importance_directions,
                    absorption_probability_shader,
                    scattering_probability_shader,
                    scattering_phase_function_shader);
            },

            [this](
                OxSceneSection const& target_scene_section,
                uint8_t num_spectra_pairs_supported,
                uint32_t max_recursion_depth,
                float ray_marching_step_size,
                uint32_t num_scattering_integral_importance_directions)
            {
                return createScatteringRenderingPass(
                    target_scene_section,
                    num_spectra_pairs_supported,
                    max_recursion_depth,
                    ray_marching_step_size,
                    num_scattering_integral_importance_directions);
            }
        ),

        "updateImportanceDirections",
        [](OxScatteringRenderingPass* p, std::vector<float2> const& data)
        {
            uint32_t num_elements = p->getNumberOfScatteringIntegralImportanceDirections();
            if (data.size() != num_elements)
                throw OxException{ ("updateImportanceDirections(...) must supply " +
                    std::to_string(num_elements) + " elements").c_str(), __FILE__, __FUNCTION__, __LINE__ };
            float2* p_buffer = p->mapImportanceDirectionsBuffer();
            std::copy(data.begin(), data.end(), p_buffer);
            p->unmapImportanceDirectionsBuffer();
        },

        "getMaxRecursionDepth", &OxScatteringRenderingPass::getMaxRecursionDepth,
        "setMaxRecursionDepth", &OxScatteringRenderingPass::setMaxRecursionDepth,

        "getNumberOfScatteringIntegralImportanceDirections", &OxScatteringRenderingPass::getNumberOfScatteringIntegralImportanceDirections,
        "getNumberOfSpectraPairsSupported", &OxScatteringRenderingPass::getNumberOfSpectraPairsSupported,

        "getRayMarchingStepSize", &OxScatteringRenderingPass::getRayMarchingStepSize,
        "setRayMarchingStepSize", &OxScatteringRenderingPass::setRayMarchingStepSize,

        "getAbsorptionProbabilityShader", &OxScatteringRenderingPass::getAbsorptionProbabilityShader,
        "setAbsorptionProbabilityShader", &OxScatteringRenderingPass::setAbsorptionProbabilityShader,
        
        "getScatteringProbabilityShader", &OxScatteringRenderingPass::getScatteringProbabilityShader,
        "setScatteringProbabilityShader", &OxScatteringRenderingPass::setScatteringProbabilityShader,

        "getScatteringPhaseFunctionShader", &OxScatteringRenderingPass::getScatteringPhaseFunctionShader,
        "setScatteringPhaseFunctionShader", &OxScatteringRenderingPass::setScatteringPhaseFunctionShader
    );
}
