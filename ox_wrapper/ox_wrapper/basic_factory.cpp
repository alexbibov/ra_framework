#include "basic_factory.h"
#include "util/lua_support.h"
#include "context.h"
#include "material_assembly.h"
#include "ray_payloads.h"
#include "ray_generator.h"
#include "scene.h"
#include "scene_section.h"
#include "ray_generator_with_output_buffer.h"


using namespace ox_wrapper;
using namespace ox_wrapper::util;


namespace {
static OxBasicFactory* p_basic_factory_instance{ nullptr };

template<OxBasicFactory::OxBufferFormat format>
struct buffer_format_to_cpp_format;

template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::FLOAT> { using type = float; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::FLOAT2> { using type = float2; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::FLOAT3> { using type = float3; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::FLOAT4> { using type = float4; };

template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::INT> { using type = int; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::INT2> { using type = int2; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::INT3> { using type = int3; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::INT4> { using type = int4; };

template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UINT> { using type = unsigned int; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UINT2> { using type = uint2; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UINT3> { using type = uint3; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UINT4> { using type = uint4; };

template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::CHAR> { using type = char; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::CHAR2> { using type = char2; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::CHAR3> { using type = char3; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::CHAR4> { using type = char4; };

template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UCHAR> { using type = unsigned char; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UCHAR2> { using type = uchar2; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UCHAR3> { using type = uchar3; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::UCHAR4> { using type = uchar4; };

template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::RAY_RADIANCE_PAYLOAD> { using type = OxRayRadiancePayload; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE> { using type = OxRayRadiancePayloadSimple; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC> { using type = OxRayRadiancePayloadMonochromatic; };
template<> struct buffer_format_to_cpp_format<OxBasicFactory::OxBufferFormat::RAY_OCCLUSION_PAYLOAD> { using type = OxRayOcclusionPayload; };



void luaRegisterGeneralTypes()
{
    lua_support::LuaState::registerType<float2>(
        "float2", lua_support::NoConstructor::make_initializer(),
        "x", &float2::x,
        "y", &float2::y
        );

    lua_support::LuaState::registerType<float3>(
        "float3", lua_support::NoConstructor::make_initializer(),
        "x", &float3::x,
        "y", &float3::y,
        "z", &float3::z
        );

    lua_support::LuaState::registerType<float4>(
        "float4", lua_support::NoConstructor::make_initializer(),
        "x", &float4::x,
        "y", &float4::y,
        "z", &float4::z,
        "w", &float4::w
        );


    lua_support::LuaState::registerType<int2>(
        "int2", lua_support::NoConstructor::make_initializer(),
        "x", &int2::x,
        "y", &int2::y
        );

    lua_support::LuaState::registerType<int3>(
        "int3", lua_support::NoConstructor::make_initializer(),
        "x", &int3::x,
        "y", &int3::y,
        "z", &int3::z
        );

    lua_support::LuaState::registerType<int4>(
        "int4", lua_support::NoConstructor::make_initializer(),
        "x", &int4::x,
        "y", &int4::y,
        "z", &int4::z,
        "w", &int4::w
        );


    lua_support::LuaState::registerType<uint2>(
        "uint2", lua_support::NoConstructor::make_initializer(),
        "x", &uint2::x,
        "y", &uint2::y
        );

    lua_support::LuaState::registerType<uint3>(
        "uint3", lua_support::NoConstructor::make_initializer(),
        "x", &uint3::x,
        "y", &uint3::y,
        "z", &uint3::z
        );

    lua_support::LuaState::registerType<uint4>(
        "uint4", lua_support::NoConstructor::make_initializer(),
        "x", &uint4::x,
        "y", &uint4::y,
        "z", &uint4::z,
        "w", &uint4::w
        );


    lua_support::LuaState::registerType<mat2x2>(
        "mat2x2", lua_support::NoConstructor::make_initializer(),
        "m11", &mat2x2::_11,
        "m12", &mat2x2::_12,
        "m21", &mat2x2::_21,
        "m22", &mat2x2::_22
        );

    lua_support::LuaState::registerType<mat2x3>(
        "mat2x3", lua_support::NoConstructor::make_initializer(),
        "m11", &mat2x3::_11,
        "m12", &mat2x3::_12,
        "m13", &mat2x3::_13,
        "m21", &mat2x3::_21,
        "m22", &mat2x3::_22,
        "m23", &mat2x3::_23
        );

    lua_support::LuaState::registerType<mat2x4>(
        "mat2x4", lua_support::NoConstructor::make_initializer(),
        "m11", &mat2x4::_11,
        "m12", &mat2x4::_12,
        "m13", &mat2x4::_13,
        "m14", &mat2x4::_14,
        "m21", &mat2x4::_21,
        "m22", &mat2x4::_22,
        "m23", &mat2x4::_23,
        "m24", &mat2x4::_24
        );


    lua_support::LuaState::registerType<mat3x2>(
        "mat3x2", lua_support::NoConstructor::make_initializer(),
        "m11", &mat3x2::_11,
        "m12", &mat3x2::_12,
        "m21", &mat3x2::_21,
        "m22", &mat3x2::_22,
        "m31", &mat3x2::_31,
        "m32", &mat3x2::_32
        );

    lua_support::LuaState::registerType<mat3x3>(
        "mat3x3", lua_support::NoConstructor::make_initializer(),
        "m11", &mat3x3::_11,
        "m12", &mat3x3::_12,
        "m13", &mat3x3::_13,
        "m21", &mat3x3::_21,
        "m22", &mat3x3::_22,
        "m23", &mat3x3::_23,
        "m31", &mat3x3::_31,
        "m32", &mat3x3::_32,
        "m33", &mat3x3::_33
        );

    lua_support::LuaState::registerType<mat3x4>(
        "mat3x4", lua_support::NoConstructor::make_initializer(),
        "m11", &mat3x4::_11,
        "m12", &mat3x4::_12,
        "m13", &mat3x4::_13,
        "m14", &mat3x4::_14,
        "m21", &mat3x4::_21,
        "m22", &mat3x4::_22,
        "m23", &mat3x4::_23,
        "m24", &mat3x4::_24,
        "m31", &mat3x4::_31,
        "m32", &mat3x4::_32,
        "m33", &mat3x4::_33,
        "m34", &mat3x4::_34
        );


    lua_support::LuaState::registerType<mat4x2>(
        "mat4x2", lua_support::NoConstructor::make_initializer(),
        "m11", &mat4x2::_11,
        "m12", &mat4x2::_12,
        "m21", &mat4x2::_21,
        "m22", &mat4x2::_22,
        "m31", &mat4x2::_31,
        "m32", &mat4x2::_32,
        "m41", &mat4x2::_41,
        "m42", &mat4x2::_42
        );

    lua_support::LuaState::registerType<mat4x3>(
        "mat4x3", lua_support::NoConstructor::make_initializer(),
        "m11", &mat4x3::_11,
        "m12", &mat4x3::_12,
        "m13", &mat4x3::_13,
        "m21", &mat4x3::_21,
        "m22", &mat4x3::_22,
        "m23", &mat4x3::_23,
        "m31", &mat4x3::_31,
        "m32", &mat4x3::_32,
        "m33", &mat4x3::_33,
        "m41", &mat4x3::_41,
        "m42", &mat4x3::_42,
        "m43", &mat4x3::_43
        );

    lua_support::LuaState::registerType<mat4x4>(
        "mat4x4", lua_support::NoConstructor::make_initializer(),
        "m11", &mat4x4::_11,
        "m12", &mat4x4::_12,
        "m13", &mat4x4::_13,
        "m14", &mat4x4::_14,
        "m21", &mat4x4::_21,
        "m22", &mat4x4::_22,
        "m23", &mat4x4::_23,
        "m24", &mat4x4::_24,
        "m31", &mat4x4::_31,
        "m32", &mat4x4::_32,
        "m33", &mat4x4::_33,
        "m34", &mat4x4::_34,
        "m41", &mat4x4::_41,
        "m42", &mat4x4::_42,
        "m43", &mat4x4::_43,
        "m44", &mat4x4::_44
        );

    lua_support::LuaState::registerType<OxEntityId>(
        "OxEntityId", lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "owningThread", &OxEntityId::owningThread,
        "toString", &OxEntityId::toString
        );

    lua_support::LuaState::registerType<OxEntity>(
        "OxEntity", lua_support::NoConstructor::make_initializer(),
        "getUniqueIdentifier", &OxEntity::getUniqueIdentifier,
        "getStringName", &OxEntity::getStringName,
        "setStringName", &OxEntity::setStringName
        );
}

void luaRegisterRayPayloadTypes()
{
    lua_support::LuaState::registerType<OxRayRadiancePayload>(
        "OxRayRadiancePayload",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "spectral_radiance", [](OxRayRadiancePayload* p)
    {
        std::array<float2, constants::max_spectra_pairs_supported> rv{};
        for (uint32_t i = 0; i < constants::max_spectra_pairs_supported; ++i)
            rv[i] = p->spectral_radiance[i];
        return rv;
    },
        "depth", &OxRayRadiancePayload::depth,
        "tracing_depth_and_aux", &OxRayRadiancePayload::tracing_depth_and_aux
        );

    lua_support::LuaState::registerType<OxRayRadiancePayloadSimple>(
        "OxRayRadiancePayloadSimple",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "spectral_radiance", &OxRayRadiancePayloadSimple::spectral_radiance,
        "depth", &OxRayRadiancePayloadSimple::depth,
        "tracing_depth_and_aux", &OxRayRadiancePayloadSimple::tracing_depth_and_aux
        );

    lua_support::LuaState::registerType<OxRayRadiancePayloadMonochromatic>(
        "OxRayRadiancePayloadMonochromatic",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "spectral_radiance", &OxRayRadiancePayloadMonochromatic::spectral_radiance,
        "depth", &OxRayRadiancePayloadMonochromatic::depth,
        "tracing_depth_and_aux", &OxRayRadiancePayloadMonochromatic::tracing_depth_and_aux
        );

    lua_support::LuaState::registerType<OxRayOcclusionPayload>(
        "OxRayOcclusionPayload",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "is_occluded", &OxRayOcclusionPayload::is_occluded,
        "tracing_depth", &OxRayOcclusionPayload::tracing_depth,
        "depth", &OxRayOcclusionPayload::depth
        );

    lua_support::LuaState::registerEnum(
        "OxRayPayloadType",
        "radiance", OxRayPayloadType::radiance,
        "radiance_simple", OxRayPayloadType::radiance_simple,
        "monochromatic", OxRayPayloadType::monochromatic,
        "occlusion", OxRayPayloadType::occlusion
    );

    lua_support::LuaState::registerEnum(
        "OxRayType",
        "unknown", OxRayType::unknown,
        "shadow", OxRayType::shadow,
        "scattered", OxRayType::scattered,
        "reflected", OxRayType::reflected
    );
}

void luaRegisterShaderTypes()
{
    lua_support::LuaState::registerEnum(
        "OxProgramSource",
        "file", OxProgram::Source::file,
        "string", OxProgram::Source::string
    );

    lua_support::LuaState::registerType<OxObjectHandle>(
        "OxObjectHandle",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "native", &OxObjectHandle::p_native
        );

    lua_support::LuaState::registerType<OxProgramId>(
        "OxProgramId",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "native", &OxProgramId::native
        );

    lua_support::LuaState::registerSubType<OxProgram>(
        "OxProgram",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](std::string const& source, OxProgram::Source source_type, std::string const& program_name)
            {
                return p_basic_factory_instance->createProgram(source, source_type, program_name);
            }
        ),

        "getId", &OxProgram::getId,

        "setVariableValue", lua_support::ListOfOverloads::make_initializer(
            static_cast<void(OxProgram::*)(std::string const&, float)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, float2 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, float3 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, float4 const&)>(&OxProgram::setVariableValue),

            static_cast<void(OxProgram::*)(std::string const&, int)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, int2 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, int3 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, int4 const&)>(&OxProgram::setVariableValue),

            static_cast<void(OxProgram::*)(std::string const&, unsigned int)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, uint2 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, uint3 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, uint4 const&)>(&OxProgram::setVariableValue),

            static_cast<void(OxProgram::*)(std::string const&, mat2x2 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, mat2x3 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, mat2x4 const&)>(&OxProgram::setVariableValue),

            static_cast<void(OxProgram::*)(std::string const&, mat3x2 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, mat3x3 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, mat3x4 const&)>(&OxProgram::setVariableValue),

            static_cast<void(OxProgram::*)(std::string const&, mat4x2 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, mat4x3 const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, mat4x4 const&)>(&OxProgram::setVariableValue),

            static_cast<void(OxProgram::*)(std::string const&, OxObjectHandle const&)>(&OxProgram::setVariableValue),
            static_cast<void(OxProgram::*)(std::string const&, OxProgramId const&)>(&OxProgram::setVariableValue)
        ),

        "getVariableValue1f", [](OxProgram* p, std::string const& name) { float val; p->getVariableValue(name, val); return val; },
        "getVariableValue2f", [](OxProgram* p, std::string const& name) { float2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3f", [](OxProgram* p, std::string const& name) { float3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4f", [](OxProgram* p, std::string const& name) { float4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue1i", [](OxProgram* p, std::string const& name) { int val; p->getVariableValue(name, val); return val; },
        "getVariableValue2i", [](OxProgram* p, std::string const& name) { int2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3i", [](OxProgram* p, std::string const& name) { int3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4i", [](OxProgram* p, std::string const& name) { int4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue1ui", [](OxProgram* p, std::string const& name) { unsigned int val; p->getVariableValue(name, val); return val; },
        "getVariableValue2ui", [](OxProgram* p, std::string const& name) { uint2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3ui", [](OxProgram* p, std::string const& name) { uint3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4ui", [](OxProgram* p, std::string const& name) { uint4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue2x2", [](OxProgram* p, std::string const& name) { mat2x2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue2x3", [](OxProgram* p, std::string const& name) { mat2x3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue2x4", [](OxProgram* p, std::string const& name) { mat2x4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue3x2", [](OxProgram* p, std::string const& name) { mat3x2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3x3", [](OxProgram* p, std::string const& name) { mat3x3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3x4", [](OxProgram* p, std::string const& name) { mat3x4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue4x2", [](OxProgram* p, std::string const& name) { mat4x2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4x3", [](OxProgram* p, std::string const& name) { mat4x3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4x4", [](OxProgram* p, std::string const& name) { mat4x4 val; p->getVariableValue(name, val); return val; },

        "getVariableValueObj", [](OxProgram* p, std::string const& name) { OxObjectHandle val; p->getVariableValue(name, val); return val; },


        "isValid", &OxProgram::isValid,
        "assignBuffer", &OxProgram::assignBuffer,
        "assignProgram", &OxProgram::assignProgram,
        "checkVariableExistance", &OxProgram::checkVariableExistance
        );
}


template<typename T>
void writeDataToBuffer(OxAbstractBuffer* p_buffer, size_t first_element, std::vector<T> const& data)
{
    T* p_buffer_data = static_cast<T*>(p_buffer->map(OxBufferMapKind::write)) + first_element;
    memcpy(p_buffer_data, data.data(), sizeof(T)*data.size());
    p_buffer->unmap();
}

template<typename T>
void readDataFromBuffer(OxAbstractBuffer* p_buffer, size_t first_element, size_t num_elements, std::vector<T>& out_data)
{
    T* p_out_data = static_cast<T*>(p_buffer->map(OxBufferMapKind::read)) + first_element;
    out_data.resize(num_elements);
    memcpy(out_data.data(), p_out_data, sizeof(T)*num_elements);
    p_buffer->unmap();
}



void luaRegisterBufferTypes()
{
    lua_support::LuaState::registerEnum("OxBufferFormat",
        "FLOAT", OxBasicFactory::OxBufferFormat::FLOAT,
        "FLOAT2", OxBasicFactory::OxBufferFormat::FLOAT2,
        "FLOAT3", OxBasicFactory::OxBufferFormat::FLOAT3,
        "FLOAT4", OxBasicFactory::OxBufferFormat::FLOAT4,

        "INT", OxBasicFactory::OxBufferFormat::INT,
        "INT2", OxBasicFactory::OxBufferFormat::INT2,
        "INT3", OxBasicFactory::OxBufferFormat::INT3,
        "INT4", OxBasicFactory::OxBufferFormat::INT4,

        "UINT", OxBasicFactory::OxBufferFormat::UINT,
        "UINT2", OxBasicFactory::OxBufferFormat::UINT2,
        "UINT3", OxBasicFactory::OxBufferFormat::UINT3,
        "UINT4", OxBasicFactory::OxBufferFormat::UINT4,

        "CHAR", OxBasicFactory::OxBufferFormat::CHAR,
        "CHAR2", OxBasicFactory::OxBufferFormat::CHAR2,
        "CHAR3", OxBasicFactory::OxBufferFormat::CHAR3,
        "CHAR4", OxBasicFactory::OxBufferFormat::CHAR4,

        "CUHAR", OxBasicFactory::OxBufferFormat::UCHAR,
        "UCHAR2", OxBasicFactory::OxBufferFormat::UCHAR2,
        "UCHAR3", OxBasicFactory::OxBufferFormat::UCHAR3,
        "UCHAR4", OxBasicFactory::OxBufferFormat::UCHAR4,

        "RAY_RADIANCE_PAYLOAD", OxBasicFactory::OxBufferFormat::RAY_RADIANCE_PAYLOAD,
        "RAY_RADIANCE_PAYLOAD_SIMPLE", OxBasicFactory::OxBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE,
        "RAY_RADIANCE_PAYLOAD_MONOCHROMATIC", OxBasicFactory::OxBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC,
        "RAY_OCCLUSION_PAYLOAD", OxBasicFactory::OxBufferFormat::RAY_OCCLUSION_PAYLOAD);

    lua_support::LuaState::registerEnum(
        "OxBufferKind",
        "input", OxBufferKind::input,
        "output", OxBufferKind::output,
        "input_output", OxBufferKind::input_output
        );

    lua_support::LuaState::registerEnum(
        "OxBufferDimension",
        "_1D", OxBufferDimension::_1D,
        "_2D", OxBufferDimension::_2D,
        "_3D", OxBufferDimension::_3D
        );

    lua_support::LuaState::registerSubType<OxAbstractBuffer>(
        "OxBuffer",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](OxBasicFactory::OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width)
            {
                return p_basic_factory_instance->createBuffer(buffer_format, 
                    buffer_kind, width);
            },

            [](OxBasicFactory::OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height)
            {
                return p_basic_factory_instance->createBuffer(buffer_format, 
                    buffer_kind, width, height);
            },

            [](OxBasicFactory::OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height, size_t depth)
            {
                return p_basic_factory_instance->createBuffer(buffer_format, 
                    buffer_kind, width, height, depth);
            }
        ),

        "getId", &OxAbstractBuffer::getId,

        "writeData", lua_support::ListOfOverloads::make_initializer(
            &writeDataToBuffer<float>, &writeDataToBuffer<float2>, &writeDataToBuffer<float3>, &writeDataToBuffer<float4>,
            &writeDataToBuffer<unsigned int>, &writeDataToBuffer<uint2>, &writeDataToBuffer<uint3>, &writeDataToBuffer<uint4>,
            &writeDataToBuffer<int>, &writeDataToBuffer<int2>, &writeDataToBuffer<int3>, &writeDataToBuffer<int4>,
            &writeDataToBuffer<unsigned char>, &writeDataToBuffer<uchar2>, &writeDataToBuffer<uchar3>, &writeDataToBuffer<uchar4>,
            &writeDataToBuffer<char>, &writeDataToBuffer<char2>, &writeDataToBuffer<char3>, &writeDataToBuffer<char4>,
            &writeDataToBuffer<OxRayRadiancePayload>, &writeDataToBuffer<OxRayRadiancePayloadSimple>,
            &writeDataToBuffer<OxRayRadiancePayloadMonochromatic>, &writeDataToBuffer<OxRayOcclusionPayload>
        ),
        "readData", lua_support::ListOfOverloads::make_initializer(
            &readDataFromBuffer<float>, &readDataFromBuffer<float2>, &readDataFromBuffer<float3>, &readDataFromBuffer<float4>,
            &readDataFromBuffer<unsigned int>, &readDataFromBuffer<uint2>, &readDataFromBuffer<uint3>, &readDataFromBuffer<uint4>,
            &readDataFromBuffer<int>, &readDataFromBuffer<int2>, &readDataFromBuffer<int3>, &readDataFromBuffer<int4>,
            &readDataFromBuffer<unsigned char>, &readDataFromBuffer<uchar2>, &readDataFromBuffer<uchar3>, &readDataFromBuffer<uchar4>,
            &readDataFromBuffer<char>, &readDataFromBuffer<char2>, &readDataFromBuffer<char3>, &readDataFromBuffer<char4>,
            &readDataFromBuffer<OxRayRadiancePayload>, &readDataFromBuffer<OxRayRadiancePayloadSimple>,
            &readDataFromBuffer<OxRayRadiancePayloadMonochromatic>, &readDataFromBuffer<OxRayOcclusionPayload>
        ),
        "OxBufferDimension", &OxAbstractBuffer::getDimension,
        "getWidth", &OxAbstractBuffer::getWidth,
        "getHeight", &OxAbstractBuffer::getHeight,
        "getDepth", &OxAbstractBuffer::getDepth,
        "isValid", &OxAbstractBuffer::isValid
    );
}

void luaRegisterMaterialTypes()
{
    lua_support::LuaState::registerSubType<OxMaterial>(
        "OxMaterial",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](OxProgram closest_hit_shader, OxProgram any_hit_shader, OxRayType ray_type)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, any_hit_shader, ray_type);
            },

            [](sol::nil_t, OxProgram any_hit_shader, OxRayType ray_type)
            {
                return p_basic_factory_instance->
                    createMaterial(util::Optional<OxProgram>{}, any_hit_shader, ray_type);
            },

            [](OxProgram closest_hit_shader, sol::nil_t, OxRayType ray_type)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, util::Optional<OxProgram>{}, ray_type);
            },

            [](OxProgram closest_hit_shader, OxProgram any_hit_shader)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, any_hit_shader);
            },

            [](sol::nil_t, OxProgram any_hit_shader)
            {
                return p_basic_factory_instance->
                    createMaterial(util::Optional<OxProgram>{}, any_hit_shader);
            },

            [](OxProgram closest_hit_shader, sol::nil_t)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, util::Optional<OxProgram>{});
            }
        ),

        "getClosestHitShader",
            [](OxMaterial* p)
            {
                util::Optional<OxProgram> rv = p->getClosestHitShader();
                if (rv.isValid()) return sol::optional<OxProgram>{static_cast<OxProgram&>(rv)};
                else return sol::optional<OxProgram>{};
            },

        "getAnyHitShader",
            [](OxMaterial* p)
            {
                util::Optional<OxProgram> rv = p->getAnyHitShader();
                if (rv.isValid()) return sol::optional<OxProgram>{static_cast<OxProgram&>(rv)};
                else return sol::optional<OxProgram>{};
            },

        "isValid", &OxMaterial::isValid
    );


    lua_support::LuaState::registerSubType<OxMaterialAssembly>(
        "OxMaterialAssembly",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](lua_support::LuaTable::table_type const& materials)
            {
                return p_basic_factory_instance->createMaterialAssembly(
                    lua_support::LuaTable::toVector<OxMaterial>(materials));
            },

            [](void)
            {
                return OxMaterialAssembly{};
            }
        ),

        "getMaterialById",
            [](OxMaterialAssembly* p, OxEntityId id)
        {
            util::Optional<OxMaterial> rv = p->getMaterialById(id);
            if (rv.isValid()) return sol::optional<OxMaterial>{static_cast<OxMaterial&>(rv)};
            else return sol::optional<OxMaterial>{};
        },

        "getMaterialByName",
            [](OxMaterialAssembly* p, std::string const& name)
        {
            util::Optional<OxMaterial> rv = p->getMaterialByName(name);
            if (rv.isValid()) return sol::optional<OxMaterial>{static_cast<OxMaterial&>(rv)};
            else return sol::optional<OxMaterial>{};
        },

        "getMaterialByRayType",
            [](OxMaterialAssembly* p, OxRayType ray_type)
        {
            util::Optional<OxMaterial> rv = p->getMaterialByRayType(ray_type);
            if (rv.isValid()) return sol::optional<OxMaterial>{static_cast<OxMaterial&>(rv)};
            else return sol::optional<OxMaterial>{};
        },

        "getMaterialCount", &OxMaterialAssembly::getMaterialCount,
        "isValid", &OxMaterialAssembly::isValid
    );
}

void luaRegisterGeometryTypes()
{
    lua_support::LuaState::registerSubType<OxGeometry>(
        "OxGeometry",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfConstructors::make_initializer(
            lua_support::Constructor<OxProgram const&, OxProgram const&>{},
            lua_support::Constructor<OxProgram const&, OxProgram const&, OxMaterialAssembly const&>{}
        ),

        "getMaterialAssembly", 
            [](OxGeometry* p)
        {
            util::Optional<OxMaterialAssembly> rv = p->getMaterialAssembly();
            if (rv.isValid()) return sol::optional<OxMaterialAssembly>{static_cast<OxMaterialAssembly&>(rv)};
            else return sol::optional<OxMaterialAssembly>{};
        },

        "setMaterialAssembly", &OxGeometry::setMaterialAssembly,
        "getAABBShader", &OxGeometry::getAABBShader,
        "getIntersectionShader", &OxGeometry::getIntersectionShader,
        "getPrimitiveCount", &OxGeometry::getPrimitiveCount,
        "isValid", &OxGeometry::isValid
    );

    lua_support::LuaState::registerEnum(
        "OxBVHAlgorithm", 
        "trbvh", OxBVHAlgorithm::trbvh,
        "sbvh", OxBVHAlgorithm::sbvh,
        "bvh", OxBVHAlgorithm::bvh,
        "none", OxBVHAlgorithm::none
    );

    lua_support::LuaState::registerType<OxTransformable>(
        "OxTransformable",
        lua_support::NoConstructor::make_initializer(),
        "applyTransform", &OxTransformable::applyTransform,
        "getTransform", &OxTransformable::getTransform
    );

    lua_support::LuaState::registerSubType<OxGeometryGroup>(
        "OxGeometryGroup",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{},
            lua_support::BaseClass<OxTransformable>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](OxBVHAlgorithm acceleration_structure_construction_algorithm)
            {
                return p_basic_factory_instance->createGeometryGroup(acceleration_structure_construction_algorithm);
            }
        ),

        "getNumberOfGeometries", &OxGeometryGroup::getNumberOfGeometries,

        "beginConstruction", &OxGeometryGroup::beginConstruction,
        "addGeometry", &OxGeometryGroup::addGeometry,
        "endConstruction", &OxGeometryGroup::endConstruction,

        "geometries", &OxGeometryGroup::geometries,

        "isValid", &OxGeometryGroup::isValid
    );
}

void luaRegisterMissShaderTypes()
{
    lua_support::LuaState::registerSubType<OxMissShader>(
        "OxMissShader",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfConstructors::make_initializer(
            lua_support::Constructor<OxProgram const&, OxRayType>{},
            lua_support::Constructor<OxProgram const&>{}
        ),

        "getProgram", &OxMissShader::getProgram,
        "rayType", &OxMissShader::rayType,
        "isValid", &OxMissShader::isValid
    );

    lua_support::LuaState::registerSubType<OxMissShaderAssembly>(
        "OxMissShaderAssembly",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](lua_support::LuaTable::table_type const& table)
            {
                return p_basic_factory_instance->createMissShaderAssembly(
                    lua_support::LuaTable::toVector<OxMissShader>(table)
                );
            }
        ),

        "getMissShaderById",
            [](OxMissShaderAssembly* p, OxEntityId const& id)
        {
            util::Optional<OxMissShader> rv = p->getMissShaderById(id);
            if (rv.isValid()) return sol::optional<OxMissShader>{static_cast<OxMissShader&>(rv)};
            else return sol::optional<OxMissShader>{};
        },

        "getMissShaderByName",
            [](OxMissShaderAssembly* p, std::string const& name)
        {
            util::Optional<OxMissShader> rv = p->getMissShaderByName(name);
            if (rv.isValid()) return sol::optional<OxMissShader>{static_cast<OxMissShader&>(rv)};
            else return sol::optional<OxMissShader>{};
        },

        "getMissShaderByRayType",
            [](OxMissShaderAssembly* p, OxRayType ray_type)
        {
            util::Optional<OxMissShader> rv = p->getMissShaderByRayType(ray_type);
            if (rv.isValid()) return sol::optional<OxMissShader>{static_cast<OxMissShader&>(rv)};
            else return sol::optional<OxMissShader>{};
        },

        "isValid", &OxMissShaderAssembly::isValid
    );
   
}

void luaRegisterRayGeneratorTypes()
{
    lua_support::LuaState::registerSubType<OxRayGeneratorWithOutputBuffer>(
        "OxRayGenerator",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfConstructors::make_initializer(
            lua_support::Constructor<OxProgram const&, OxAbstractBuffer const&,
                std::string const&, uint32_t, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<OxProgram const&, OxAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<OxProgram const&, OxAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t>{},
            lua_support::Constructor<OxProgram const&, OxAbstractBuffer const&,
            std::string const&, uint32_t>{},

            lua_support::Constructor<OxProgram const&, 
                OxMissShaderAssembly const&, OxAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<OxProgram const&,
            OxMissShaderAssembly const&, OxAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<OxProgram const&,
            OxMissShaderAssembly const&, OxAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t>{},
            lua_support::Constructor<OxProgram const&,
            OxMissShaderAssembly const&, OxAbstractBuffer const&,
            std::string const&, uint32_t>{}
        ),

        "getRayGenerationShader", &OxRayGenerator::getRayGenerationShader,
        "getMissShaderAssembly", 
            [](OxRayGeneratorWithOutputBuffer* p)
            {
                util::Optional<OxMissShaderAssembly> rv = p->getMissShaderAssembly();
                if (rv.isValid()) return sol::optional<OxMissShaderAssembly>{static_cast<OxMissShaderAssembly&>(rv)};
                else return sol::optional<OxMissShaderAssembly>{};
            },
        "setMissShaderAssembly", &OxRayGenerator::setMissShaderAssembly,
        "isValid", &OxRayGenerator::isValid,
        "getGeneratorDimensions", &OxRayGenerator::getGeneratorDimensions,
        "numberOfRays", &OxRayGenerator::numberOfRays,
        "outputBuffer", &OxRayGeneratorWithOutputBuffer::outputBuffer
    );
}

void luaRegisterSceneTypes()
{
    lua_support::LuaState::registerSubType<OxSceneSection>(
        "OxSceneSection",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{},
            lua_support::BaseClass<OxTransformable>{}
        ),

        lua_support::ListOfConstructors::make_initializer(
            lua_support::Constructor<OxRayGenerator const&, OxBVHAlgorithm>{}
        ),

        "rayGenerator", &OxSceneSection::rayGenerator,
        "beginConstruction", &OxSceneSection::beginConstruction,
        "addGeometryGroup", &OxSceneSection::addGeometryGroup,
        "addSceneSection", &OxSceneSection::addSceneSection,
        "endConstruction", &OxSceneSection::endConstruction,

        "sceneSections", &OxSceneSection::sceneSections,
        "geometryGroups", &OxSceneSection::geometryGroups,

        "isValid", &OxSceneSection::isValid,
        "update", &OxSceneSection::update,
        "trace", &OxSceneSection::trace
    );

    lua_support::LuaState::registerSubType<OxScene>(
        "OxScene",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<OxEntity>{}
        ),

        lua_support::ListOfConstructors::make_initializer(),

        "addSceneSection", &OxScene::addSceneSection,
        "isValid", &OxScene::isValid,
        "update", &OxScene::update,
        "trace", &OxScene::trace
    );
}

}

OxBasicFactory* ox_wrapper::OxBasicFactory::initialize(OxContext const& context)
{
    if (!p_basic_factory_instance)
        p_basic_factory_instance = new OxBasicFactory{ context };

    return p_basic_factory_instance;
}

void OxBasicFactory::shutdown()
{
    if (p_basic_factory_instance)
    {
        delete p_basic_factory_instance;
        p_basic_factory_instance = nullptr;
    }
}

OxBasicFactory* OxBasicFactory::retrieve()
{
    return p_basic_factory_instance;
}

bool OxBasicFactory::isValid() const
{
    return p_basic_factory_instance;
}

OxProgram OxBasicFactory::createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const
{
    return m_context.createProgram(source, source_type, program_name);
}

OxAbstractBuffer OxBasicFactory::createBuffer(OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width) const
{
    switch (buffer_format)
    {
    case OxBufferFormat::FLOAT:
        return m_context.createBuffer<float>(buffer_kind, width);
    case OxBufferFormat::FLOAT2:
        return m_context.createBuffer<float2>(buffer_kind, width);
    case OxBufferFormat::FLOAT3:
        return m_context.createBuffer<float3>(buffer_kind, width);
    case OxBufferFormat::FLOAT4:
        return m_context.createBuffer<float4>(buffer_kind, width);
    case OxBufferFormat::INT:
        return m_context.createBuffer<int>(buffer_kind, width);
    case OxBufferFormat::INT2:
        return m_context.createBuffer<int2>(buffer_kind, width);
    case OxBufferFormat::INT3:
        return m_context.createBuffer<int3>(buffer_kind, width);
    case OxBufferFormat::INT4:
        return m_context.createBuffer<int4>(buffer_kind, width);
    case OxBufferFormat::UINT:
        return m_context.createBuffer<unsigned int>(buffer_kind, width);
    case OxBufferFormat::UINT2:
        return m_context.createBuffer<uint2>(buffer_kind, width);
    case OxBufferFormat::UINT3:
        return m_context.createBuffer<uint3>(buffer_kind, width);
    case OxBufferFormat::UINT4:
        return m_context.createBuffer<uint4>(buffer_kind, width);
    case OxBufferFormat::CHAR:
        return m_context.createBuffer<char>(buffer_kind, width);
    case OxBufferFormat::CHAR2:
        return m_context.createBuffer<char2>(buffer_kind, width);
    case OxBufferFormat::CHAR3:
        return m_context.createBuffer<char3>(buffer_kind, width);
    case OxBufferFormat::CHAR4:
        return m_context.createBuffer<char3>(buffer_kind, width);
    case OxBufferFormat::UCHAR:
        return m_context.createBuffer<unsigned char>(buffer_kind, width);
    case OxBufferFormat::UCHAR2:
        return m_context.createBuffer<uchar2>(buffer_kind, width);
    case OxBufferFormat::UCHAR3:
        return m_context.createBuffer<uchar3>(buffer_kind, width);
    case OxBufferFormat::UCHAR4:
        return m_context.createBuffer<uchar4>(buffer_kind, width);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD:
        return m_context.createBuffer<OxRayRadiancePayload>(buffer_kind, width);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        return m_context.createBuffer<OxRayRadiancePayloadSimple>(buffer_kind, width);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        return m_context.createBuffer<OxRayRadiancePayloadMonochromatic>(buffer_kind, width);
    case OxBufferFormat::RAY_OCCLUSION_PAYLOAD:
        return m_context.createBuffer<OxRayOcclusionPayload>(buffer_kind, width);
    default:
        throw OxException{ "Unknown buffer format", __FILE__, __FUNCTION__, __LINE__ };
    }
}

OxAbstractBuffer OxBasicFactory::createBuffer(OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height) const
{
    switch (buffer_format)
    {
    case OxBufferFormat::FLOAT:
        return m_context.createBuffer<float>(buffer_kind, width, height);
    case OxBufferFormat::FLOAT2:
        return m_context.createBuffer<float2>(buffer_kind, width, height);
    case OxBufferFormat::FLOAT3:
        return m_context.createBuffer<float3>(buffer_kind, width, height);
    case OxBufferFormat::FLOAT4:
        return m_context.createBuffer<float4>(buffer_kind, width, height);
    case OxBufferFormat::INT:
        return m_context.createBuffer<int>(buffer_kind, width, height);
    case OxBufferFormat::INT2:
        return m_context.createBuffer<int2>(buffer_kind, width, height);
    case OxBufferFormat::INT3:
        return m_context.createBuffer<int3>(buffer_kind, width, height);
    case OxBufferFormat::INT4:
        return m_context.createBuffer<int4>(buffer_kind, width, height);
    case OxBufferFormat::UINT:
        return m_context.createBuffer<unsigned int>(buffer_kind, width, height);
    case OxBufferFormat::UINT2:
        return m_context.createBuffer<uint2>(buffer_kind, width, height);
    case OxBufferFormat::UINT3:
        return m_context.createBuffer<uint3>(buffer_kind, width, height);
    case OxBufferFormat::UINT4:
        return m_context.createBuffer<uint4>(buffer_kind, width, height);
    case OxBufferFormat::CHAR:
        return m_context.createBuffer<char>(buffer_kind, width, height);
    case OxBufferFormat::CHAR2:
        return m_context.createBuffer<char2>(buffer_kind, width, height);
    case OxBufferFormat::CHAR3:
        return m_context.createBuffer<char3>(buffer_kind, width, height);
    case OxBufferFormat::CHAR4:
        return m_context.createBuffer<char3>(buffer_kind, width, height);
    case OxBufferFormat::UCHAR:
        return m_context.createBuffer<unsigned char>(buffer_kind, width, height);
    case OxBufferFormat::UCHAR2:
        return m_context.createBuffer<uchar2>(buffer_kind, width, height);
    case OxBufferFormat::UCHAR3:
        return m_context.createBuffer<uchar3>(buffer_kind, width, height);
    case OxBufferFormat::UCHAR4:
        return m_context.createBuffer<uchar4>(buffer_kind, width, height);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD:
        return m_context.createBuffer<OxRayRadiancePayload>(buffer_kind, width, height);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        return m_context.createBuffer<OxRayRadiancePayloadSimple>(buffer_kind, width, height);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        return m_context.createBuffer<OxRayRadiancePayloadMonochromatic>(buffer_kind, width, height);
    case OxBufferFormat::RAY_OCCLUSION_PAYLOAD:
        return m_context.createBuffer<OxRayOcclusionPayload>(buffer_kind, width, height);
    default:
        throw OxException{ "Unknown buffer format", __FILE__, __FUNCTION__, __LINE__ };
    }
}

OxAbstractBuffer OxBasicFactory::createBuffer(OxBufferFormat buffer_format, OxBufferKind buffer_kind, size_t width, size_t height, size_t depth) const
{
    switch (buffer_format)
    {
    case OxBufferFormat::FLOAT:
        return m_context.createBuffer<float>(buffer_kind, width, height, depth);
    case OxBufferFormat::FLOAT2:
        return m_context.createBuffer<float2>(buffer_kind, width, height, depth);
    case OxBufferFormat::FLOAT3:
        return m_context.createBuffer<float3>(buffer_kind, width, height, depth);
    case OxBufferFormat::FLOAT4:
        return m_context.createBuffer<float4>(buffer_kind, width, height, depth);
    case OxBufferFormat::INT:
        return m_context.createBuffer<int>(buffer_kind, width, height, depth);
    case OxBufferFormat::INT2:
        return m_context.createBuffer<int2>(buffer_kind, width, height, depth);
    case OxBufferFormat::INT3:
        return m_context.createBuffer<int3>(buffer_kind, width, height, depth);
    case OxBufferFormat::INT4:
        return m_context.createBuffer<int4>(buffer_kind, width, height, depth);
    case OxBufferFormat::UINT:
        return m_context.createBuffer<unsigned int>(buffer_kind, width, height, depth);
    case OxBufferFormat::UINT2:
        return m_context.createBuffer<uint2>(buffer_kind, width, height, depth);
    case OxBufferFormat::UINT3:
        return m_context.createBuffer<uint3>(buffer_kind, width, height, depth);
    case OxBufferFormat::UINT4:
        return m_context.createBuffer<uint4>(buffer_kind, width, height, depth);
    case OxBufferFormat::CHAR:
        return m_context.createBuffer<char>(buffer_kind, width, height, depth);
    case OxBufferFormat::CHAR2:
        return m_context.createBuffer<char2>(buffer_kind, width, height, depth);
    case OxBufferFormat::CHAR3:
        return m_context.createBuffer<char3>(buffer_kind, width, height, depth);
    case OxBufferFormat::CHAR4:
        return m_context.createBuffer<char3>(buffer_kind, width, height, depth);
    case OxBufferFormat::UCHAR:
        return m_context.createBuffer<unsigned char>(buffer_kind, width, height, depth);
    case OxBufferFormat::UCHAR2:
        return m_context.createBuffer<uchar2>(buffer_kind, width, height, depth);
    case OxBufferFormat::UCHAR3:
        return m_context.createBuffer<uchar3>(buffer_kind, width, height, depth);
    case OxBufferFormat::UCHAR4:
        return m_context.createBuffer<uchar4>(buffer_kind, width, height, depth);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD:
        return m_context.createBuffer<OxRayRadiancePayload>(buffer_kind, width, height, depth);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        return m_context.createBuffer<OxRayRadiancePayloadSimple>(buffer_kind, width, height, depth);
    case OxBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        return m_context.createBuffer<OxRayRadiancePayloadMonochromatic>(buffer_kind, width, height, depth);
    case OxBufferFormat::RAY_OCCLUSION_PAYLOAD:
        return m_context.createBuffer<OxRayOcclusionPayload>(buffer_kind, width, height, depth);
    default:
        throw OxException{ "Unknown buffer format", __FILE__, __FUNCTION__, __LINE__ };
    }
}

OxMaterial OxBasicFactory::createMaterial(util::Optional<OxProgram> const& closest_hit_shader, util::Optional<OxProgram> const& any_hit_shader, OxRayType ray_type) const
{
    return OxMaterial{ closest_hit_shader, any_hit_shader, ray_type };
}

OxMaterialAssembly OxBasicFactory::createMaterialAssembly(std::vector<OxMaterial> const& materials) const
{
    return OxMaterialAssembly{ materials };
}

OxGeometry OxBasicFactory::createGeometry(OxProgram const& intersection_shader, OxProgram const& aabb_shader) const
{
    return OxGeometry{ intersection_shader, aabb_shader };
}

OxGeometry OxBasicFactory::createGeometry(OxProgram const& intersection_shader, 
    OxProgram const& aabb_shader, OxMaterialAssembly const& material_assembly) const
{
    return OxGeometry{ intersection_shader, aabb_shader, material_assembly };
}

OxGeometryGroup OxBasicFactory::createGeometryGroup(OxBVHAlgorithm acceleration_structure_construction_algorithm) const
{
    return OxGeometryGroup{ m_context, acceleration_structure_construction_algorithm };
}

OxMissShader OxBasicFactory::createMissShader(OxProgram const& miss_shader, OxRayType ray_type) const
{
    return OxMissShader{ miss_shader, ray_type };
}

OxMissShaderAssembly OxBasicFactory::createMissShaderAssembly(std::vector<OxMissShader> const& miss_shaders) const
{
    return OxMissShaderAssembly{ miss_shaders };
}

OxRayGeneratorWithOutputBuffer OxBasicFactory::createRayGenerator(OxProgram const& ray_generation_shader, 
    OxAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, uint32_t entry_point_index) const
{
    return OxRayGeneratorWithOutputBuffer{ ray_generation_shader, output_buffer, output_buffer_binding_name,
    num_rays_x, num_rays_y, num_rays_z, entry_point_index };
}

OxRayGeneratorWithOutputBuffer OxBasicFactory::createRayGenerator(OxProgram const& ray_generation_shader, 
    OxMissShaderAssembly const& miss_shader_assembly, OxAbstractBuffer const& output_buffer, 
    std::string const& output_buffer_binding_name, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z,
    uint32_t entry_point_index) const
{
    return OxRayGeneratorWithOutputBuffer{ ray_generation_shader, miss_shader_assembly, output_buffer,
    output_buffer_binding_name, num_rays_x, num_rays_y, num_rays_z, entry_point_index };
}

OxSceneSection OxBasicFactory::createSceneSection(OxRayGenerator const& ray_generator, OxBVHAlgorithm acceleration_structure_construction_algorithm) const
{
    return OxSceneSection{ ray_generator, acceleration_structure_construction_algorithm };
}

OxScene OxBasicFactory::createScene() const
{
    return OxScene{};
}

OxBasicFactory::OxBasicFactory(OxContext const& context):
    m_context{ context }
{
    setStringName("OxBasicFactory");

    // Register factory functions in Lua
    {
        // general ox_wrapper types
        luaRegisterGeneralTypes();

        // ray payload types
        luaRegisterRayPayloadTypes();

        // shaders support
        luaRegisterShaderTypes();

        // buffers support
        luaRegisterBufferTypes();

        // materials support
        luaRegisterMaterialTypes();

        // geometries support
        luaRegisterGeometryTypes();

        // miss shaders support
        luaRegisterMissShaderTypes();

        // ray generators support
        luaRegisterRayGeneratorTypes();

        // scenes support
        luaRegisterSceneTypes();
    }
}
