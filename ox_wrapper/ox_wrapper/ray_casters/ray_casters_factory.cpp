#include "ray_casters_factory.h"
#include "parallel_ray_generator.h"
#include "recaster_generator.h"
#include "../util/lua_support.h"

using namespace ox_wrapper;
using namespace ox_wrapper::ray_casters;
using namespace ox_wrapper::util;


namespace {

OxRaycastersFactory* p_factory_instance{ nullptr };

}



OxRaycastersFactory* OxRaycastersFactory::initialize(OxContext const& context)
{
    if (!p_factory_instance)
        p_factory_instance = new OxRaycastersFactory{ context };

    return p_factory_instance;
}

void OxRaycastersFactory::shutdown()
{
    if (p_factory_instance)
    {
        delete p_factory_instance;
        p_factory_instance = nullptr;
    }
}

OxRaycastersFactory* OxRaycastersFactory::retrieve()
{
    return p_factory_instance;
}

bool OxRaycastersFactory::isValid() const
{
    return p_factory_instance;
}

OxParallelRayGenerator OxRaycastersFactory::createParallelRayGenerator(uint32_t num_rays,
    float emitter_size, float emitter_position, float emitter_rotation,
    uint8_t num_spectra_pairs_supported, uint32_t entry_point_index) const
{
    return OxParallelRayGenerator{ m_context, num_rays, emitter_size, emitter_position, emitter_rotation,
    num_spectra_pairs_supported, entry_point_index };
}

OxRecasterGenerator OxRaycastersFactory::createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer, 
    OxBuffer<OxRayRadiancePayload> const& output_buffer, OxRayType recasted_ray_type) const
{
    return OxRecasterGenerator{ traverse_backup_buffer, output_buffer, recasted_ray_type };
}

OxRecasterGenerator OxRaycastersFactory::createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer,
    OxBuffer<OxRayRadiancePayloadSimple> const& output_buffer, OxRayType recasted_ray_type) const
{
    return OxRecasterGenerator{ traverse_backup_buffer, output_buffer, recasted_ray_type };
}

OxRecasterGenerator OxRaycastersFactory::createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer,
    OxBuffer<OxRayRadiancePayloadMonochromatic> const& output_buffer, OxRayType recasted_ray_type) const
{
    return OxRecasterGenerator{ traverse_backup_buffer, output_buffer, recasted_ray_type };
}

OxRecasterGenerator OxRaycastersFactory::createRecasterGenerator(OxTraverseBackupBuffer const& traverse_backup_buffer,
    OxBuffer<OxRayOcclusionPayload> const& output_buffer, OxRayType recasted_ray_type) const
{
    return OxRecasterGenerator{ traverse_backup_buffer, output_buffer, recasted_ray_type };
}

OxTraverseBackupBuffer OxRaycastersFactory::createTraverseBackupBuffer(size_t max_ray_storage_capacity)
{
    return OxTraverseBackupBuffer{ m_context, max_ray_storage_capacity };
}

OxRaycastersFactory::OxRaycastersFactory(OxContext const& context) :
    m_context{ context }
{
    setStringName("OxRaycastersFactory");

    // register factory functions in Lua
    {
        lua_support::LuaState::registerSubType<OxParallelRayGenerator>(
            "OxParallelRayGenerator",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<OxEntity>{},
                lua_support::BaseClass<OxRayGeneratorWithOutputBuffer>{}
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

            "setEmitterSize", &OxParallelRayGenerator::setEmitterSize,
            "setEmitterPosition", &OxParallelRayGenerator::setEmitterPosition,
            "setEmitterRotation", &OxParallelRayGenerator::setEmitterRotation,
            "getNumberOfRays", &OxParallelRayGenerator::getNumberOfRays,
            "getEmitterSize", &OxParallelRayGenerator::getEmitterSize,
            "getEmitterRotation", &OxParallelRayGenerator::getEmitterRotation,
            "getNumberOfSpectraPairsSupported", &OxParallelRayGenerator::getNumberOfSpectraPairsSupported,
            
            "updateSpectralFluxBuffer",
            [](OxParallelRayGenerator* p, lua_support::LuaTable::table_type const& data)
            {
                auto converted_data = lua_support::LuaTable::toVector<float2>(data);
                float2* p_destinations = p->mapSpectralFluxBuffer();
                memcpy(p_destinations, converted_data.data(), sizeof(float2)*converted_data.size());
                p->unmapSpectralFluxBuffer();
            }
        );


        lua_support::LuaState::registerSubType<OxTraverseBackupBuffer>(
            "OxTraverseBackupBuffer",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<OxEntity>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](size_t max_ray_storage_capacity)
                {
                    return createTraverseBackupBuffer(max_ray_storage_capacity);
                }
            ),

            "getRawBuffer",
            [](OxTraverseBackupBuffer* p)
            {
                OxAbstractBuffer rv = p->getRawBuffer();
                return rv;
            },

            "isValid", &OxTraverseBackupBuffer::isValid
        );


        lua_support::LuaState::registerSubType<OxRecasterGenerator>(
            "OxRecasterGenerator",

            lua_support::ListOfBaseClasses::make_initializer(
                lua_support::BaseClass<OxEntity>{},
                lua_support::BaseClass<OxRayGeneratorWithOutputBuffer>{}
            ),

            lua_support::ListOfFactories::make_initializer(
                [this](OxTraverseBackupBuffer const& traverse_backup_buffer,
                    OxAbstractBuffer const& output_buffer, OxRayPayloadType payload_type,
                    OxRayType recasted_ray_type)
                {
                    switch (payload_type)
                    {
                    case OxRayPayloadType::radiance:
                        return createRecasterGenerator(traverse_backup_buffer,
                            dynamic_cast<OxBuffer<OxRayRadiancePayload> const&>(output_buffer),
                            recasted_ray_type);

                    case OxRayPayloadType::radiance_simple:
                        return createRecasterGenerator(traverse_backup_buffer,
                            dynamic_cast<OxBuffer<OxRayRadiancePayloadSimple> const&>(output_buffer),
                            recasted_ray_type);

                    case OxRayPayloadType::monochromatic:
                        return createRecasterGenerator(traverse_backup_buffer,
                            dynamic_cast<OxBuffer<OxRayRadiancePayloadMonochromatic> const&>(output_buffer),
                            recasted_ray_type);
                    case OxRayPayloadType::occlusion:
                        return createRecasterGenerator(traverse_backup_buffer,
                            dynamic_cast<OxBuffer<OxRayOcclusionPayload> const&>(output_buffer),
                            recasted_ray_type);
                    default:
                        throw OxException{ "unknown payload type", __FILE__, __FUNCTION__, __LINE__ };
                    }
                }
            )
        );

        
    }
}