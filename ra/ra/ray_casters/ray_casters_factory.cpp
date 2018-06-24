#define _SCL_SECURE_NO_WARNINGS

#include "ray_casters_factory.h"
#include "parallel_ray_generator.h"
#include "recaster_generator.h"
#include "../util/lua_support.h"

using namespace ra;
using namespace ra::ray_casters;
using namespace ra::util;


namespace {

RaRaycastersFactory* p_factory_instance{ nullptr };

}



RaRaycastersFactory* RaRaycastersFactory::initialize(RaContext const& context)
{
    if (!p_factory_instance)
        p_factory_instance = new RaRaycastersFactory{ context };

    return p_factory_instance;
}

void RaRaycastersFactory::shutdown()
{
    if (p_factory_instance)
    {
        delete p_factory_instance;
        p_factory_instance = nullptr;
    }
}

RaRaycastersFactory* RaRaycastersFactory::retrieve()
{
    return p_factory_instance;
}

bool RaRaycastersFactory::isValid() const
{
    return p_factory_instance;
}

RaParallelRayGenerator RaRaycastersFactory::createParallelRayGenerator(uint32_t num_rays,
    float emitter_size, float emitter_position, float emitter_rotation,
    uint8_t num_spectra_pairs_supported, uint32_t entry_point_index) const
{
    return RaParallelRayGenerator{ m_context, num_rays, emitter_size, emitter_position, emitter_rotation,
    num_spectra_pairs_supported, entry_point_index };
}

RaRecasterGenerator RaRaycastersFactory::createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
    RaBuffer<RaRayRadiancePayload> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length) const
{
    return RaRecasterGenerator{ original_problem_size, traverse_backup_buffer, output_buffer, recasted_ray_type, recasted_ray_parametric_length };
}

RaRecasterGenerator RaRaycastersFactory::createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
    RaBuffer<RaRayRadiancePayloadSimple> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length) const
{
    return RaRecasterGenerator{ original_problem_size, traverse_backup_buffer, output_buffer, recasted_ray_type, recasted_ray_parametric_length };
}

RaRecasterGenerator RaRaycastersFactory::createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
    RaBuffer<RaRayRadiancePayloadMonochromatic> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length) const
{
    return RaRecasterGenerator{ original_problem_size, traverse_backup_buffer, output_buffer, recasted_ray_type, recasted_ray_parametric_length };
}

RaRecasterGenerator RaRaycastersFactory::createRecasterGenerator(uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
    RaBuffer<RaRayOcclusionPayload> const& output_buffer, RaRayType recasted_ray_type, float recasted_ray_parametric_length) const
{
    return RaRecasterGenerator{ original_problem_size, traverse_backup_buffer, output_buffer, recasted_ray_type, recasted_ray_parametric_length };
}

RaTraverseBackupBuffer RaRaycastersFactory::createTraverseBackupBuffer(size_t max_ray_storage_capacity)
{
    return RaTraverseBackupBuffer{ m_context, max_ray_storage_capacity };
}

RaRaycastersFactory::RaRaycastersFactory(RaContext const& context) :
    m_context{ context }
{
    setStringName("RaRaycastersFactory");

    // register factory functions in Lua
    {
        lua_support::LuaState::registerSubType<RaParallelRayGenerator>(
            "RaParallelRayGenerator",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<RaEntity>{},
                lua_support::BaseClass<RaRayGeneratorWithOutputBuffer>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](uint32_t num_rays, float emitter_size, float emitter_position,
                    float emitter_rotation, uint8_t num_spectra_pairs_supported, uint32_t entry_point_idx)
                {
                    return createParallelRayGenerator(num_rays, emitter_size, emitter_position, emitter_rotation,
                        num_spectra_pairs_supported, entry_point_idx);
                },

                [this](uint32_t num_rays, float emitter_size, float emitter_position,
                    float emitter_rotation, uint8_t num_spectra_pairs_supported)
                {
                    return createParallelRayGenerator(num_rays, emitter_size, emitter_position, emitter_rotation,
                        num_spectra_pairs_supported);
                },

                [this](uint32_t num_rays, float emitter_size, float emitter_position,
                    float emitter_rotation)
                {
                    return createParallelRayGenerator(num_rays, emitter_size, emitter_position, emitter_rotation);
                },

                [this](uint32_t num_rays, float emitter_size, float emitter_position)
                {
                    return createParallelRayGenerator(num_rays, emitter_size, emitter_position);
                },

                [this](uint32_t num_rays, float emitter_size)
                {
                    return createParallelRayGenerator(num_rays, emitter_size);
                }
                    ),

                "setEmitterSize", &RaParallelRayGenerator::setEmitterSize,
                    "setEmitterPosition", &RaParallelRayGenerator::setEmitterPosition,
                    "setEmitterRotation", &RaParallelRayGenerator::setEmitterRotation,
                    "getNumberOfRays", &RaParallelRayGenerator::getNumberOfRays,
                    "getEmitterSize", &RaParallelRayGenerator::getEmitterSize,
                    "getEmitterRotation", &RaParallelRayGenerator::getEmitterRotation,
                    "getNumberOfSpectraPairsSupported", &RaParallelRayGenerator::getNumberOfSpectraPairsSupported,

                    "updateSpectralFluxBuffer",
                    [](RaParallelRayGenerator* p, lua_support::LuaTable::table_type const& data)
                {
                    uint32_t num_elements = p->numberOfRays() * p->getNumberOfSpectraPairsSupported();
                    if (data.size() != num_elements)
                        THROW_OX_WRAPPER_ERROR("Error while updating spectral flux buffer of parallel ray generator \""
                            + p->getStringName() + "\": updateSpectralFluxBuffer(...) must supply "
                            + std::to_string(num_elements)
                            + " elements, but " + std::to_string(data.size()) + " elements were provided instead");

                auto converted_data = lua_support::LuaTable::toVector<float2>(data);
                float2* p_destinations = p->mapSpectralFluxBuffer();
                std::copy(converted_data.begin(), converted_data.end(), p_destinations);
                p->unmapSpectralFluxBuffer();
            }
        );


        lua_support::LuaState::registerSubType<RaTraverseBackupBuffer>(
            "RaTraverseBackupBuffer",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<RaEntity>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](size_t max_ray_storage_capacity)
                {
                    return createTraverseBackupBuffer(max_ray_storage_capacity);
                }
            ),

            "readBuffer",
            [](RaTraverseBackupBuffer* p)
            {
                RaAbstractBuffer rv = p->readBuffer();
                return rv;
            },

            "writeBuffer",
                [](RaTraverseBackupBuffer* p)
            {
                RaAbstractBuffer rv = p->writeBuffer();
                return rv;
            },

            "isValid", &RaTraverseBackupBuffer::isValid
        );


        lua_support::LuaState::registerSubType<RaRecasterGenerator>(
            "RaRecasterGenerator",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<RaEntity>{},
                lua_support::BaseClass<RaRayGeneratorWithOutputBuffer>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
                    RaAbstractBuffer const& output_buffer, RaRayPayloadType payload_type,
                    RaRayType recasted_ray_type, float recasted_ray_parametric_length)
                {
                    switch (payload_type)
                    {
                    case RaRayPayloadType::radiance:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayRadiancePayload> const&>(output_buffer),
                            recasted_ray_type, recasted_ray_parametric_length);

                    case RaRayPayloadType::radiance_simple:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayRadiancePayloadSimple> const&>(output_buffer),
                            recasted_ray_type, recasted_ray_parametric_length);

                    case RaRayPayloadType::monochromatic:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayRadiancePayloadMonochromatic> const&>(output_buffer),
                            recasted_ray_type, recasted_ray_parametric_length);
                    case RaRayPayloadType::occlusion:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayOcclusionPayload> const&>(output_buffer),
                            recasted_ray_type, recasted_ray_parametric_length);
                    default:
                        THROW_OX_WRAPPER_ERROR("unknown payload type");
                    }
                },

                [this](uint3 const& original_problem_size, RaTraverseBackupBuffer const& traverse_backup_buffer,
                    RaAbstractBuffer const& output_buffer, RaRayPayloadType payload_type,
                    RaRayType recasted_ray_type)
                {
                    switch (payload_type)
                    {
                    case RaRayPayloadType::radiance:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayRadiancePayload> const&>(output_buffer),
                            recasted_ray_type);

                    case RaRayPayloadType::radiance_simple:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayRadiancePayloadSimple> const&>(output_buffer),
                            recasted_ray_type);

                    case RaRayPayloadType::monochromatic:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayRadiancePayloadMonochromatic> const&>(output_buffer),
                            recasted_ray_type);
                    case RaRayPayloadType::occlusion:
                        return createRecasterGenerator(original_problem_size, traverse_backup_buffer,
                            dynamic_cast<RaBuffer<RaRayOcclusionPayload> const&>(output_buffer),
                            recasted_ray_type);
                    default:
                        THROW_OX_WRAPPER_ERROR("unknown payload type");
                    }
                }
            )
        );

        
    }
}