#include "basic_factory.h"
#include "util/lua_support.h"
#include "context.h"
#include "material_assembly.h"
#include "ray_payloads.h"
#include "ray_generator.h"
#include "scene.h"
#include "scene_section.h"
#include "ray_generator_with_output_buffer.h"


using namespace ra;
using namespace ra::util;


namespace {
static RaBasicFactory* p_basic_factory_instance{ nullptr };

template<RaBasicBufferFormat format>
struct buffer_format_to_cpp_format;

template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::FLOAT> { using type = float; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::FLOAT2> { using type = float2; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::FLOAT3> { using type = float3; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::FLOAT4> { using type = float4; };

template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::INT> { using type = int; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::INT2> { using type = int2; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::INT3> { using type = int3; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::INT4> { using type = int4; };

template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UINT> { using type = unsigned int; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UINT2> { using type = uint2; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UINT3> { using type = uint3; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UINT4> { using type = uint4; };

template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::CHAR> { using type = char; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::CHAR2> { using type = char2; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::CHAR3> { using type = char3; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::CHAR4> { using type = char4; };

template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UCHAR> { using type = unsigned char; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UCHAR2> { using type = uchar2; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UCHAR3> { using type = uchar3; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::UCHAR4> { using type = uchar4; };

template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD> { using type = RaRayRadiancePayload; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE> { using type = RaRayRadiancePayloadSimple; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC> { using type = RaRayRadiancePayloadMonochromatic; };
template<> struct buffer_format_to_cpp_format<RaBasicBufferFormat::RAY_OCCLUSION_PAYLOAD> { using type = RaRayOcclusionPayload; };



void luaRegisterGeneralTypes()
{
    lua_support::LuaState::registerType<float2>(
        "float2", 
        lua_support::ListOfFactories::make_initializer(
            [](float x, float y) {return float2{ x, y }; }
        ),
        "x", &float2::x,
        "y", &float2::y
        );

    lua_support::LuaState::registerType<float3>(
        "float3", 
        lua_support::ListOfFactories::make_initializer(
            [](float x, float y, float z) {return float3{ x, y, z }; },
            [](float2 const& xy, float z) {return float3{ xy.x, xy.y, z }; },
            [](float x, float2 const& yz) {return float3{ x, yz.x, yz.y }; }
        ),
        "x", &float3::x,
        "y", &float3::y,
        "z", &float3::z
        );

    lua_support::LuaState::registerType<float4>(
        "float4", 
        lua_support::ListOfFactories::make_initializer(
            [](float x, float y, float z, float w) {return float4{ x, y, z, w }; },
            [](float2 const& xy, float z, float w) {return float4{ xy.x, xy.y, z, w }; },
            [](float2 const& xy, float2 const& zw) {return float4{ xy.x, xy.y, zw.x, zw.y }; },
            [](float x, float2 const& yz, float w) {return float4{ x, yz.x, yz.y, w }; },
            [](float x, float y, float2 const& zw) {return float4{ x, y, zw.x, zw.y }; }
        ),
        "x", &float4::x,
        "y", &float4::y,
        "z", &float4::z,
        "w", &float4::w
        );


    lua_support::LuaState::registerType<int2>(
        "int2", 
        lua_support::ListOfFactories::make_initializer(
            [](int x, int y) {return int2{ x, y }; }
        ),
        "x", &int2::x,
        "y", &int2::y
        );

    lua_support::LuaState::registerType<int3>(
        "int3", 
        lua_support::ListOfFactories::make_initializer(
            [](int x, int y, int z) {return int3{ x, y, z }; },
            [](int2 const& xy, int z) {return int3{ xy.x, xy.y, z }; },
            [](int x, int2 const& yz) {return int3{ x, yz.x, yz.y }; }
        ),
        "x", &int3::x,
        "y", &int3::y,
        "z", &int3::z
        );

    lua_support::LuaState::registerType<int4>(
        "int4", 
        lua_support::ListOfFactories::make_initializer(
            [](int x, int y, int z, int w) {return int4{ x, y, z, w }; },
            [](int2 const& xy, int z, int w) {return int4{ xy.x, xy.y, z, w }; },
            [](int2 const& xy, int2 const& zw) {return int4{ xy.x, xy.y, zw.x, zw.y }; },
            [](int x, int2 const& yz, int w) {return int4{ x, yz.x, yz.y, w }; },
            [](int x, int y, int2 const& zw) {return int4{ x, y, zw.x, zw.y }; }
        ),
        "x", &int4::x,
        "y", &int4::y,
        "z", &int4::z,
        "w", &int4::w
        );


    lua_support::LuaState::registerType<uint2>(
        "uint2", 
        lua_support::ListOfFactories::make_initializer(
            [](unsigned int x, unsigned int y) {return uint2{ x, y }; }
        ),
        "x", &uint2::x,
        "y", &uint2::y
        );

    lua_support::LuaState::registerType<uint3>(
        "uint3", 
        lua_support::ListOfFactories::make_initializer(
            [](unsigned int x, unsigned int y, unsigned int z) {return uint3{ x, y, z }; },
            [](uint2 const& xy, unsigned int z) {return uint3{ xy.x, xy.y, z }; },
            [](unsigned int x, uint2 const& yz) {return uint3{ x, yz.x, yz.y }; }
        ),
        "x", &uint3::x,
        "y", &uint3::y,
        "z", &uint3::z
        );

    lua_support::LuaState::registerType<uint4>(
        "uint4", 
        lua_support::ListOfFactories::make_initializer(
            [](unsigned int x, unsigned int y, unsigned int z, unsigned int w) {return uint4{ x, y, z, w }; },
            [](uint2 const& xy, unsigned int z, unsigned int w) {return uint4{ xy.x, xy.y, z, w }; },
            [](uint2 const& xy, uint2 const& zw) {return uint4{ xy.x, xy.y, zw.x, zw.y }; },
            [](unsigned int x, uint2 const& yz, unsigned int w) {return uint4{ x, yz.x, yz.y, w }; },
            [](unsigned int x, unsigned int y, uint2 const& zw) {return uint4{ x, y, zw.x, zw.y }; }
        ),
        "x", &uint4::x,
        "y", &uint4::y,
        "z", &uint4::z,
        "w", &uint4::w
        );


    lua_support::LuaState::registerType<mat2x2>(
        "mat2x2",
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, 
                float m21, float m22)
            {
                return mat2x2{ 
                    m11, m12, 
                    m21, m22 };
            },
            [](float2 const& r1, float2 const& r2)
            {
                return mat2x2{ 
                    r1.x, r1.y,
                    r2.x, r2.y };
            }
        ),
        "m11", &mat2x2::_11,
        "m12", &mat2x2::_12,
        "m21", &mat2x2::_21,
        "m22", &mat2x2::_22
        );

    lua_support::LuaState::registerType<mat2x3>(
        "mat2x3", 
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, float m13,
                float m21, float m22, float m23)
            {
                return mat2x3{ 
                    m11, m12, m13, 
                    m21, m22, m23 };
            },
            [](float3 const& r1, float3 const& r2)
            {
                return mat2x3{ 
                    r1.x, r1.y, r1.z,
                    r2.x, r2.y, r2.z };
            }
        ),
        "m11", &mat2x3::_11,
        "m12", &mat2x3::_12,
        "m13", &mat2x3::_13,
        "m21", &mat2x3::_21,
        "m22", &mat2x3::_22,
        "m23", &mat2x3::_23
        );

    lua_support::LuaState::registerType<mat2x4>(
        "mat2x4", 
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, float m13, float m14,
                float m21, float m22, float m23, float m24)
            {
                return mat2x4{
                    m11, m12, m13, m14,
                    m21, m22, m23, m24 };
            },
            [](float4 const& r1, float4 const& r2)
            {
                return mat2x4{
                    r1.x, r1.y, r1.z, r1.w,
                    r2.x, r2.y, r2.z, r2.w };
            }
        ),
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
        "mat3x2", 
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12,
                float m21, float m22,
                float m31, float m32)
            {
                return mat3x2{
                    m11, m12,
                    m21, m22,
                    m31, m32 };
            },
            
            [](float2 const& r1, float2 const& r2, float2 const& r3)
            {
                return mat3x2{
                    r1.x, r1.y,
                    r2.x, r2.y,
                    r3.x, r3.y };
            }
        ),

        "m11", &mat3x2::_11,
        "m12", &mat3x2::_12,
        "m21", &mat3x2::_21,
        "m22", &mat3x2::_22,
        "m31", &mat3x2::_31,
        "m32", &mat3x2::_32
        );

    lua_support::LuaState::registerType<mat3x3>(
        "mat3x3",
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, float m13,
                float m21, float m22, float m23,
                float m31, float m32, float m33)
            {
                return mat3x3{
                    m11, m12, m13,
                    m21, m22, m23,
                    m31, m32, m33 };
            },

            [](float3 const& r1, float3 const& r2, float3 const& r3)
            {
                return mat3x3{
                    r1.x, r1.y, r1.z,
                    r2.x, r2.y, r2.z,
                    r3.x, r3.y, r3.z };
            }
        ),
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
        "mat3x4", 
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, float m13, float m14,
                float m21, float m22, float m23, float m24,
                float m31, float m32, float m33, float m34)
            {
                return mat3x4{
                    m11, m12, m13, m14,
                    m21, m22, m23, m24,
                    m31, m32, m33, m34 };
            },

            [](float4 const& r1, float4 const& r2, float4 const& r3)
            {
                return mat3x4{
                    r1.x, r1.y, r1.z, r1.w,
                    r2.x, r2.y, r2.z, r2.w,
                    r3.x, r3.y, r3.z, r3.w };
            }
        ),

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
        "mat4x2", 
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, 
                float m21, float m22,
                float m31, float m32,
                float m41, float m42)
            {
                return mat4x2{
                    m11, m12,
                    m21, m22,
                    m31, m32,
                    m41, m42 };
            },

            [](float2 const& r1, float2 const& r2, float2 const& r3, float2 const& r4)
            {
                return mat4x2{
                    r1.x, r1.y,
                    r2.x, r2.y,
                    r3.x, r3.y,
                    r4.x, r4.y };
            }
        ),

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
        "mat4x3", 
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, float m13,
                float m21, float m22, float m23,
                float m31, float m32, float m33,
                float m41, float m42, float m43)
            {
                return mat4x3{
                    m11, m12, m13,
                    m21, m22, m23,
                    m31, m32, m33,
                    m41, m42, m43 };
            },

            [](float3 const& r1, float3 const& r2, float3 const& r3, float3 const& r4)
            {
                return mat4x3{
                    r1.x, r1.y, r1.z,
                    r2.x, r2.y, r2.z,
                    r3.x, r3.y, r3.z,
                    r4.x, r4.y, r4.z };
            }
        ),

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
        "mat4x4", 
        lua_support::ListOfFactories::make_initializer(
            [](float m11, float m12, float m13, float m14,
                float m21, float m22, float m23, float m24,
                float m31, float m32, float m33, float m34,
                float m41, float m42, float m43, float m44)
            {
                return mat4x4{
                    m11, m12, m13, m14,
                    m21, m22, m23, m24,
                    m31, m32, m33, m34,
                    m41, m42, m43, m44 };
            },

            [](float4 const& r1, float4 const& r2, float4 const& r3, float4 const& r4)
            {
                return mat4x4{
                    r1.x, r1.y, r1.z, r1.w,
                    r2.x, r2.y, r2.z, r2.w,
                    r3.x, r3.y, r3.z, r3.w,
                    r4.x, r4.y, r4.z, r4.w };
            }
        ),

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

    lua_support::LuaState::registerType<RaEntityId>(
        "RaEntityId", lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "owningThread", &RaEntityId::owningThread,
        "toString", &RaEntityId::toString
        );

    lua_support::LuaState::registerType<RaEntity>(
        "RaEntity", lua_support::NoConstructor::make_initializer(),
        "getUniqueIdentifier", &RaEntity::getUniqueIdentifier,
        "getStringName", &RaEntity::getStringName,
        "setStringName", &RaEntity::setStringName
        );
}

void luaRegisterRayPayloadTypes()
{
    lua_support::LuaState::registerType<RaRayRadiancePayload>(
        "RaRayRadiancePayload",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "spectral_radiance", [](RaRayRadiancePayload* p)
    {
        std::array<float2, constants::max_spectra_pairs_supported> rv{};
        for (uint32_t i = 0; i < constants::max_spectra_pairs_supported; ++i)
            rv[i] = p->spectral_radiance[i];
        return rv;
    },
        "depth", &RaRayRadiancePayload::depth,
        "tracing_depth_and_aux", &RaRayRadiancePayload::tracing_depth_and_aux
        );

    lua_support::LuaState::registerType<RaRayRadiancePayloadSimple>(
        "RaRayRadiancePayloadSimple",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "spectral_radiance", &RaRayRadiancePayloadSimple::spectral_radiance,
        "depth", &RaRayRadiancePayloadSimple::depth,
        "tracing_depth_and_aux", &RaRayRadiancePayloadSimple::tracing_depth_and_aux
        );

    lua_support::LuaState::registerType<RaRayRadiancePayloadMonochromatic>(
        "RaRayRadiancePayloadMonochromatic",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "spectral_radiance", &RaRayRadiancePayloadMonochromatic::spectral_radiance,
        "depth", &RaRayRadiancePayloadMonochromatic::depth,
        "tracing_depth_and_aux", &RaRayRadiancePayloadMonochromatic::tracing_depth_and_aux
        );

    lua_support::LuaState::registerType<RaRayOcclusionPayload>(
        "RaRayOcclusionPayload",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "is_occluded", &RaRayOcclusionPayload::is_occluded,
        "tracing_depth", &RaRayOcclusionPayload::tracing_depth,
        "depth", &RaRayOcclusionPayload::depth
        );

    lua_support::LuaState::registerEnum(
        "RaRayPayloadType",
        "radiance", RaRayPayloadType::radiance,
        "radiance_simple", RaRayPayloadType::radiance_simple,
        "monochromatic", RaRayPayloadType::monochromatic,
        "occlusion", RaRayPayloadType::occlusion
    );

    lua_support::LuaState::registerEnum(
        "RaRayType",
        "unknown", RaRayType::unknown,
        "shadow", RaRayType::shadow,
        "scattered", RaRayType::scattered,
        "reflected", RaRayType::reflected
    );
}

void luaRegisterShaderTypes()
{
    lua_support::LuaState::registerEnum(
        "RaProgramSource",
        "file", RaProgram::Source::file,
        "string", RaProgram::Source::string
    );

    lua_support::LuaState::registerType<RaObjectHandle>(
        "RaObjectHandle",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "native", &RaObjectHandle::p_native
        );

    lua_support::LuaState::registerType<RaProgramId>(
        "RaProgramId",
        lua_support::ListOfConstructors::make_initializer(lua_support::Constructor<void>{}),
        "native", &RaProgramId::native
        );

    lua_support::LuaState::registerSubType<RaProgram>(
        "RaProgram",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](std::string const& source, RaProgram::Source source_type, std::string const& program_name)
            {
                return p_basic_factory_instance->createProgram(source, source_type, program_name);
            }
        ),

        "getId", &RaProgram::getId,

        "setVariableValue", lua_support::ListOfOverloads::make_initializer(
            static_cast<void(RaProgram::*)(std::string const&, float)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, float2 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, float3 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, float4 const&)>(&RaProgram::setVariableValue),

            static_cast<void(RaProgram::*)(std::string const&, int)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, int2 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, int3 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, int4 const&)>(&RaProgram::setVariableValue),

            static_cast<void(RaProgram::*)(std::string const&, unsigned int)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, uint2 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, uint3 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, uint4 const&)>(&RaProgram::setVariableValue),

            static_cast<void(RaProgram::*)(std::string const&, mat2x2 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, mat2x3 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, mat2x4 const&)>(&RaProgram::setVariableValue),

            static_cast<void(RaProgram::*)(std::string const&, mat3x2 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, mat3x3 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, mat3x4 const&)>(&RaProgram::setVariableValue),

            static_cast<void(RaProgram::*)(std::string const&, mat4x2 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, mat4x3 const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, mat4x4 const&)>(&RaProgram::setVariableValue),

            static_cast<void(RaProgram::*)(std::string const&, RaObjectHandle const&)>(&RaProgram::setVariableValue),
            static_cast<void(RaProgram::*)(std::string const&, RaProgramId const&)>(&RaProgram::setVariableValue)
        ),

        "getVariableValue1f", [](RaProgram* p, std::string const& name) { float val; p->getVariableValue(name, val); return val; },
        "getVariableValue2f", [](RaProgram* p, std::string const& name) { float2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3f", [](RaProgram* p, std::string const& name) { float3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4f", [](RaProgram* p, std::string const& name) { float4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue1i", [](RaProgram* p, std::string const& name) { int val; p->getVariableValue(name, val); return val; },
        "getVariableValue2i", [](RaProgram* p, std::string const& name) { int2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3i", [](RaProgram* p, std::string const& name) { int3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4i", [](RaProgram* p, std::string const& name) { int4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue1ui", [](RaProgram* p, std::string const& name) { unsigned int val; p->getVariableValue(name, val); return val; },
        "getVariableValue2ui", [](RaProgram* p, std::string const& name) { uint2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3ui", [](RaProgram* p, std::string const& name) { uint3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4ui", [](RaProgram* p, std::string const& name) { uint4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue2x2", [](RaProgram* p, std::string const& name) { mat2x2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue2x3", [](RaProgram* p, std::string const& name) { mat2x3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue2x4", [](RaProgram* p, std::string const& name) { mat2x4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue3x2", [](RaProgram* p, std::string const& name) { mat3x2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3x3", [](RaProgram* p, std::string const& name) { mat3x3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue3x4", [](RaProgram* p, std::string const& name) { mat3x4 val; p->getVariableValue(name, val); return val; },

        "getVariableValue4x2", [](RaProgram* p, std::string const& name) { mat4x2 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4x3", [](RaProgram* p, std::string const& name) { mat4x3 val; p->getVariableValue(name, val); return val; },
        "getVariableValue4x4", [](RaProgram* p, std::string const& name) { mat4x4 val; p->getVariableValue(name, val); return val; },

        "getVariableValueObj", [](RaProgram* p, std::string const& name) { RaObjectHandle val; p->getVariableValue(name, val); return val; },


        "isValid", &RaProgram::isValid,
        "assignBuffer", &RaProgram::assignBuffer,
        "assignProgram", &RaProgram::assignProgram,
        "checkVariableExistance", &RaProgram::checkVariableExistance
        );
}


template<typename T>
void writeDataToBuffer(RaAbstractBuffer* p_buffer, size_t first_element, std::vector<T> const& data)
{
    T* p_buffer_data = static_cast<T*>(p_buffer->map(RaBufferMapKind::write)) + first_element;
    memcpy(p_buffer_data, data.data(), sizeof(T)*data.size());
    p_buffer->unmap();
}

template<typename T>
void readDataFromBuffer(RaAbstractBuffer* p_buffer, size_t first_element, size_t num_elements, std::vector<T>& out_data)
{
    T* p_out_data = static_cast<T*>(p_buffer->map(RaBufferMapKind::read)) + first_element;
    out_data.resize(num_elements);
    memcpy(out_data.data(), p_out_data, sizeof(T)*num_elements);
    p_buffer->unmap();
}



void luaRegisterBufferTypes()
{
    lua_support::LuaState::registerEnum("RaBufferFormat",
        "FLOAT", RaBasicBufferFormat::FLOAT,
        "FLOAT2", RaBasicBufferFormat::FLOAT2,
        "FLOAT3", RaBasicBufferFormat::FLOAT3,
        "FLOAT4", RaBasicBufferFormat::FLOAT4,

        "INT", RaBasicBufferFormat::INT,
        "INT2", RaBasicBufferFormat::INT2,
        "INT3", RaBasicBufferFormat::INT3,
        "INT4", RaBasicBufferFormat::INT4,

        "UINT", RaBasicBufferFormat::UINT,
        "UINT2", RaBasicBufferFormat::UINT2,
        "UINT3", RaBasicBufferFormat::UINT3,
        "UINT4", RaBasicBufferFormat::UINT4,

        "CHAR", RaBasicBufferFormat::CHAR,
        "CHAR2", RaBasicBufferFormat::CHAR2,
        "CHAR3", RaBasicBufferFormat::CHAR3,
        "CHAR4", RaBasicBufferFormat::CHAR4,

        "CUHAR", RaBasicBufferFormat::UCHAR,
        "UCHAR2", RaBasicBufferFormat::UCHAR2,
        "UCHAR3", RaBasicBufferFormat::UCHAR3,
        "UCHAR4", RaBasicBufferFormat::UCHAR4,

        "RAY_RADIANCE_PAYLOAD", RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD,
        "RAY_RADIANCE_PAYLOAD_SIMPLE", RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE,
        "RAY_RADIANCE_PAYLOAD_MONOCHROMATIC", RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC,
        "RAY_OCCLUSION_PAYLOAD", RaBasicBufferFormat::RAY_OCCLUSION_PAYLOAD);

    lua_support::LuaState::registerEnum(
        "RaBufferKind",
        "input", RaBufferKind::input,
        "output", RaBufferKind::output,
        "input_output", RaBufferKind::input_output
        );

    lua_support::LuaState::registerEnum(
        "RaBufferDimension",
        "_1D", RaBufferDimension::_1D,
        "_2D", RaBufferDimension::_2D,
        "_3D", RaBufferDimension::_3D
        );

    lua_support::LuaState::registerSubType<RaAbstractBuffer>(
        "RaBuffer",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width)
            {
                return p_basic_factory_instance->createBuffer(buffer_format, 
                    buffer_kind, width);
            },

            [](RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width, size_t height)
            {
                return p_basic_factory_instance->createBuffer(buffer_format, 
                    buffer_kind, width, height);
            },

            [](RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width, size_t height, size_t depth)
            {
                return p_basic_factory_instance->createBuffer(buffer_format, 
                    buffer_kind, width, height, depth);
            }
        ),

        "getId", &RaAbstractBuffer::getId,

        "writeData", lua_support::ListOfOverloads::make_initializer(
            &writeDataToBuffer<float>, &writeDataToBuffer<float2>, &writeDataToBuffer<float3>, &writeDataToBuffer<float4>,
            &writeDataToBuffer<unsigned int>, &writeDataToBuffer<uint2>, &writeDataToBuffer<uint3>, &writeDataToBuffer<uint4>,
            &writeDataToBuffer<int>, &writeDataToBuffer<int2>, &writeDataToBuffer<int3>, &writeDataToBuffer<int4>,
            &writeDataToBuffer<unsigned char>, &writeDataToBuffer<uchar2>, &writeDataToBuffer<uchar3>, &writeDataToBuffer<uchar4>,
            &writeDataToBuffer<char>, &writeDataToBuffer<char2>, &writeDataToBuffer<char3>, &writeDataToBuffer<char4>,
            &writeDataToBuffer<RaRayRadiancePayload>, &writeDataToBuffer<RaRayRadiancePayloadSimple>,
            &writeDataToBuffer<RaRayRadiancePayloadMonochromatic>, &writeDataToBuffer<RaRayOcclusionPayload>
        ),
        "readData", lua_support::ListOfOverloads::make_initializer(
            &readDataFromBuffer<float>, &readDataFromBuffer<float2>, &readDataFromBuffer<float3>, &readDataFromBuffer<float4>,
            &readDataFromBuffer<unsigned int>, &readDataFromBuffer<uint2>, &readDataFromBuffer<uint3>, &readDataFromBuffer<uint4>,
            &readDataFromBuffer<int>, &readDataFromBuffer<int2>, &readDataFromBuffer<int3>, &readDataFromBuffer<int4>,
            &readDataFromBuffer<unsigned char>, &readDataFromBuffer<uchar2>, &readDataFromBuffer<uchar3>, &readDataFromBuffer<uchar4>,
            &readDataFromBuffer<char>, &readDataFromBuffer<char2>, &readDataFromBuffer<char3>, &readDataFromBuffer<char4>,
            &readDataFromBuffer<RaRayRadiancePayload>, &readDataFromBuffer<RaRayRadiancePayloadSimple>,
            &readDataFromBuffer<RaRayRadiancePayloadMonochromatic>, &readDataFromBuffer<RaRayOcclusionPayload>
        ),
        "RaBufferDimension", &RaAbstractBuffer::getDimension,
        "getWidth", &RaAbstractBuffer::getWidth,
        "getHeight", &RaAbstractBuffer::getHeight,
        "getDepth", &RaAbstractBuffer::getDepth,
        "getElementSize", &RaAbstractBuffer::getElementSize,
        "getCapacityInBytes", &RaAbstractBuffer::getCapacityInBytes,
        "isValid", &RaAbstractBuffer::isValid
    );
}

void luaRegisterMaterialTypes()
{
    lua_support::LuaState::registerSubType<RaMaterial>(
        "RaMaterial",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](RaProgram closest_hit_shader, RaProgram any_hit_shader, 
                lua_support::LuaTable::table_type const& supported_ray_types)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, any_hit_shader, 
                        lua_support::LuaTable::toStaticVector<RaRayTypeCollection::value_type, RaRayTypeCollection::capacity()>(supported_ray_types));
            },

            [](sol::nil_t, RaProgram any_hit_shader, 
                lua_support::LuaTable::table_type const& supported_ray_types)
            {
                return p_basic_factory_instance->
                    createMaterial(util::Optional<RaProgram>{}, any_hit_shader, 
                        lua_support::LuaTable::toStaticVector<RaRayTypeCollection::value_type, RaRayTypeCollection::capacity()>(supported_ray_types));
            },

            [](RaProgram closest_hit_shader, sol::nil_t, 
                lua_support::LuaTable::table_type const& supported_ray_types)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, util::Optional<RaProgram>{}, 
                        lua_support::LuaTable::toStaticVector<RaRayTypeCollection::value_type, RaRayTypeCollection::capacity()>(supported_ray_types));
            },

            [](RaProgram closest_hit_shader, RaProgram any_hit_shader)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, any_hit_shader);
            },

            [](sol::nil_t, RaProgram any_hit_shader)
            {
                return p_basic_factory_instance->
                    createMaterial(util::Optional<RaProgram>{}, any_hit_shader);
            },

            [](RaProgram closest_hit_shader, sol::nil_t)
            {
                return p_basic_factory_instance->
                    createMaterial(closest_hit_shader, util::Optional<RaProgram>{});
            }
        ),

        "getClosestHitShader",
            [](RaMaterial* p)
            {
                util::Optional<RaProgram> rv = p->getClosestHitShader();
                if (rv.isValid()) return sol::optional<RaProgram>{static_cast<RaProgram&>(rv)};
                else return sol::optional<RaProgram>{};
            },

        "getAnyHitShader",
            [](RaMaterial* p)
            {
                util::Optional<RaProgram> rv = p->getAnyHitShader();
                if (rv.isValid()) return sol::optional<RaProgram>{static_cast<RaProgram&>(rv)};
                else return sol::optional<RaProgram>{};
            },

        "supportedRayTypes",
            [](RaMaterial* p) -> lua_support::LuaTable::table_type
            {
                auto supported_ray_types = p->supportedRayTypes();
                return lua_support::LuaTable::staticVectorToTable(supported_ray_types);
            },

        "supportsRayType", &RaMaterial::supportsRayType,

        "isValid", &RaMaterial::isValid
    );


    lua_support::LuaState::registerSubType<RaMaterialAssembly>(
        "RaMaterialAssembly",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](lua_support::LuaTable::table_type const& materials)
            {
                return p_basic_factory_instance->createMaterialAssembly(
                    lua_support::LuaTable::toVector<RaMaterial>(materials));
            },

            [](void)
            {
                return p_basic_factory_instance->createDummyMaterialAssembly();
            }
        ),

        "getMaterialById",
            [](RaMaterialAssembly* p, RaEntityId id)
        {
            util::Optional<RaMaterial> rv = p->getMaterialById(id);
            if (rv.isValid()) return sol::optional<RaMaterial>{static_cast<RaMaterial&>(rv)};
            else return sol::optional<RaMaterial>{};
        },

        "getMaterialByName",
            [](RaMaterialAssembly* p, std::string const& name)
        {
            util::Optional<RaMaterial> rv = p->getMaterialByName(name);
            if (rv.isValid()) return sol::optional<RaMaterial>{static_cast<RaMaterial&>(rv)};
            else return sol::optional<RaMaterial>{};
        },

        "getMaterialByRayType",
            [](RaMaterialAssembly* p, RaRayType ray_type)
        {
            util::Optional<RaMaterial> rv = p->getMaterialByRayType(ray_type);
            if (rv.isValid()) return sol::optional<RaMaterial>{static_cast<RaMaterial&>(rv)};
            else return sol::optional<RaMaterial>{};
        },

        "getMaterialCount", &RaMaterialAssembly::getMaterialCount,
        "isValid", &RaMaterialAssembly::isValid
    );
}

void luaRegisterGeometryTypes()
{
    lua_support::LuaState::registerSubType<RaGeometry>(
        "RaGeometry",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfConstructors::make_initializer(
            lua_support::Constructor<RaProgram const&, RaProgram const&>{},
            lua_support::Constructor<RaProgram const&, RaProgram const&, RaMaterialAssembly const&>{}
        ),

        "getMaterialAssembly", 
            [](RaGeometry* p)
        {
            util::Optional<RaMaterialAssembly> rv = p->getMaterialAssembly();
            if (rv.isValid()) return sol::optional<RaMaterialAssembly>{static_cast<RaMaterialAssembly&>(rv)};
            else return sol::optional<RaMaterialAssembly>{};
        },

        "setMaterialAssembly", &RaGeometry::setMaterialAssembly,
        "getAABBShader", &RaGeometry::getAABBShader,
        "getIntersectionShader", &RaGeometry::getIntersectionShader,
        "getPrimitiveCount", &RaGeometry::getPrimitiveCount,
        "isValid", &RaGeometry::isValid
    );

    lua_support::LuaState::registerEnum(
        "RaBVHAlgorithm", 
        "trbvh", RaBVHAlgorithm::trbvh,
        "sbvh", RaBVHAlgorithm::sbvh,
        "bvh", RaBVHAlgorithm::bvh,
        "none", RaBVHAlgorithm::none
    );

    lua_support::LuaState::registerType<RaTransformable>(
        "RaTransformable",
        lua_support::NoConstructor::make_initializer(),
        "applyTransform", &RaTransformable::applyTransform,
        "getTransform", &RaTransformable::getTransform
    );

    lua_support::LuaState::registerSubType<RaGeometryGroup>(
        "RaGeometryGroup",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{},
            lua_support::BaseClass<RaTransformable>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](RaBVHAlgorithm acceleration_structure_construction_algorithm)
            {
                return p_basic_factory_instance->createGeometryGroup(acceleration_structure_construction_algorithm);
            }
        ),

        "getNumberOfGeometries", &RaGeometryGroup::getNumberOfGeometries,

        "beginConstruction", &RaGeometryGroup::beginConstruction,
        "addGeometry", &RaGeometryGroup::addGeometry,
        "endConstruction", &RaGeometryGroup::endConstruction,

        "geometries", &RaGeometryGroup::geometries,

        "isValid", &RaGeometryGroup::isValid
    );
}

void luaRegisterMissShaderTypes()
{
    lua_support::LuaState::registerSubType<RaMissShader>(
        "RaMissShader",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](RaProgram const& miss_shader,
                lua_support::LuaTable::table_type const& supported_ray_types)
            {
                return p_basic_factory_instance->createMissShader(miss_shader, 
                    lua_support::LuaTable::toStaticVector<RaRayTypeCollection::value_type, RaRayTypeCollection::capacity()>(supported_ray_types));
            },

            [](RaProgram const& miss_shader)
            {
                return p_basic_factory_instance->createMissShader(miss_shader);
            }
        ),

        "getProgram", &RaMissShader::getProgram,
        
        "supportedRayTypes",
        [](RaMissShader* p)
        {
            auto supported_ray_types = p->supportedRayTypes();
            return lua_support::LuaTable::staticVectorToTable(supported_ray_types);
        },

        "supportsRayType", &RaMissShader::supportsRayType,

        "isValid", &RaMissShader::isValid
    );

    lua_support::LuaState::registerSubType<RaMissShaderAssembly>(
        "RaMissShaderAssembly",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](lua_support::LuaTable::table_type const& table)
            {
                return p_basic_factory_instance->createMissShaderAssembly(
                    lua_support::LuaTable::toVector<RaMissShader>(table)
                );
            }
        ),

        "getMissShaderById",
            [](RaMissShaderAssembly* p, RaEntityId const& id)
        {
            util::Optional<RaMissShader> rv = p->getMissShaderById(id);
            if (rv.isValid()) return sol::optional<RaMissShader>{static_cast<RaMissShader&>(rv)};
            else return sol::optional<RaMissShader>{};
        },

        "getMissShaderByName",
            [](RaMissShaderAssembly* p, std::string const& name)
        {
            util::Optional<RaMissShader> rv = p->getMissShaderByName(name);
            if (rv.isValid()) return sol::optional<RaMissShader>{static_cast<RaMissShader&>(rv)};
            else return sol::optional<RaMissShader>{};
        },

        "getMissShaderByRayType",
            [](RaMissShaderAssembly* p, RaRayType ray_type)
        {
            util::Optional<RaMissShader> rv = p->getMissShaderByRayType(ray_type);
            if (rv.isValid()) return sol::optional<RaMissShader>{static_cast<RaMissShader&>(rv)};
            else return sol::optional<RaMissShader>{};
        },

        "isValid", &RaMissShaderAssembly::isValid
    );
   
}

void luaRegisterRayGeneratorTypes()
{
    lua_support::LuaState::registerSubType<RaRayGeneratorWithOutputBuffer>(
        "RaRayGenerator",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfConstructors::make_initializer(
            lua_support::Constructor<RaProgram const&, RaAbstractBuffer const&,
                std::string const&, uint32_t, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<RaProgram const&, RaAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<RaProgram const&, RaAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t>{},
            lua_support::Constructor<RaProgram const&, RaAbstractBuffer const&,
            std::string const&, uint32_t>{},

            lua_support::Constructor<RaProgram const&, 
                RaMissShaderAssembly const&, RaAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<RaProgram const&,
            RaMissShaderAssembly const&, RaAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t, uint32_t>{},
            lua_support::Constructor<RaProgram const&,
            RaMissShaderAssembly const&, RaAbstractBuffer const&,
            std::string const&, uint32_t, uint32_t>{},
            lua_support::Constructor<RaProgram const&,
            RaMissShaderAssembly const&, RaAbstractBuffer const&,
            std::string const&, uint32_t>{}
        ),

        "getRayGenerationShader", &RaRayGenerator::getRayGenerationShader,
        "getMissShaderAssembly", 
            [](RaRayGeneratorWithOutputBuffer* p)
            {
                util::Optional<RaMissShaderAssembly> rv = p->getMissShaderAssembly();
                if (rv.isValid()) return sol::optional<RaMissShaderAssembly>{static_cast<RaMissShaderAssembly&>(rv)};
                else return sol::optional<RaMissShaderAssembly>{};
            },
        "setMissShaderAssembly", &RaRayGenerator::setMissShaderAssembly,
        "isValid", &RaRayGenerator::isValid,
        "getGeneratorDimensions", &RaRayGenerator::getGeneratorDimensions,
        "numberOfRays", &RaRayGenerator::numberOfRays,
        "outputBuffer", &RaRayGeneratorWithOutputBuffer::outputBuffer
    );
}

void luaRegisterSceneTypes()
{
    lua_support::LuaState::registerSubType<RaSceneSection>(
        "RaSceneSection",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{},
            lua_support::BaseClass<RaTransformable>{}
        ),

        lua_support::ListOfFactories::make_initializer(
            [](RaBVHAlgorithm bvh_algorithm)
            {
                return p_basic_factory_instance->createSceneSection(bvh_algorithm); 
            }
        ),

        "beginConstruction", &RaSceneSection::beginConstruction,
        "addGeometryGroup", &RaSceneSection::addGeometryGroup,
        "addSceneSection", &RaSceneSection::addSceneSection,
        "endConstruction", &RaSceneSection::endConstruction,

        "sceneSections", &RaSceneSection::sceneSections,
        "geometryGroups", &RaSceneSection::geometryGroups,

        "isValid", &RaSceneSection::isValid,
        "trace", &RaSceneSection::trace
    );

    lua_support::LuaState::registerSubType<RaScene>(
        "RaScene",

        lua_support::ListOfBaseClasses::make_initializer(
            lua_support::BaseClass<RaEntity>{}
        ),

        lua_support::ListOfConstructors::make_initializer(
            lua_support::Constructor<void>{}
        ),

        "addSceneSection", &RaScene::addSceneSection,
        "isValid", &RaScene::isValid,
        "trace", &RaScene::trace
    );
}

}

RaBasicFactory* ra::RaBasicFactory::initialize(RaContext const& context)
{
    if (!p_basic_factory_instance)
        p_basic_factory_instance = new RaBasicFactory{ context };

    return p_basic_factory_instance;
}

void RaBasicFactory::shutdown()
{
    if (p_basic_factory_instance)
    {
        delete p_basic_factory_instance;
        p_basic_factory_instance = nullptr;
    }
}

RaBasicFactory* RaBasicFactory::retrieve()
{
    return p_basic_factory_instance;
}

bool RaBasicFactory::isValid() const
{
    return true;
}

RaContext const& RaBasicFactory::context() const
{
    return m_context;
}

RaProgram RaBasicFactory::createProgram(std::string const& source, RaProgram::Source source_type, std::string const& program_name) const
{
    return m_context.createProgram(source, source_type, program_name);
}

RaAbstractBuffer RaBasicFactory::createBuffer(RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width) const
{
    switch (buffer_format)
    {
    case RaBasicBufferFormat::FLOAT:
        return m_context.createBuffer<float>(buffer_kind, width);
    case RaBasicBufferFormat::FLOAT2:
        return m_context.createBuffer<float2>(buffer_kind, width);
    case RaBasicBufferFormat::FLOAT3:
        return m_context.createBuffer<float3>(buffer_kind, width);
    case RaBasicBufferFormat::FLOAT4:
        return m_context.createBuffer<float4>(buffer_kind, width);
    case RaBasicBufferFormat::INT:
        return m_context.createBuffer<int>(buffer_kind, width);
    case RaBasicBufferFormat::INT2:
        return m_context.createBuffer<int2>(buffer_kind, width);
    case RaBasicBufferFormat::INT3:
        return m_context.createBuffer<int3>(buffer_kind, width);
    case RaBasicBufferFormat::INT4:
        return m_context.createBuffer<int4>(buffer_kind, width);
    case RaBasicBufferFormat::UINT:
        return m_context.createBuffer<unsigned int>(buffer_kind, width);
    case RaBasicBufferFormat::UINT2:
        return m_context.createBuffer<uint2>(buffer_kind, width);
    case RaBasicBufferFormat::UINT3:
        return m_context.createBuffer<uint3>(buffer_kind, width);
    case RaBasicBufferFormat::UINT4:
        return m_context.createBuffer<uint4>(buffer_kind, width);
    case RaBasicBufferFormat::CHAR:
        return m_context.createBuffer<char>(buffer_kind, width);
    case RaBasicBufferFormat::CHAR2:
        return m_context.createBuffer<char2>(buffer_kind, width);
    case RaBasicBufferFormat::CHAR3:
        return m_context.createBuffer<char3>(buffer_kind, width);
    case RaBasicBufferFormat::CHAR4:
        return m_context.createBuffer<char3>(buffer_kind, width);
    case RaBasicBufferFormat::UCHAR:
        return m_context.createBuffer<unsigned char>(buffer_kind, width);
    case RaBasicBufferFormat::UCHAR2:
        return m_context.createBuffer<uchar2>(buffer_kind, width);
    case RaBasicBufferFormat::UCHAR3:
        return m_context.createBuffer<uchar3>(buffer_kind, width);
    case RaBasicBufferFormat::UCHAR4:
        return m_context.createBuffer<uchar4>(buffer_kind, width);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        return m_context.createBuffer<RaRayRadiancePayload>(buffer_kind, width);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        return m_context.createBuffer<RaRayRadiancePayloadSimple>(buffer_kind, width);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        return m_context.createBuffer<RaRayRadiancePayloadMonochromatic>(buffer_kind, width);
    case RaBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        return m_context.createBuffer<RaRayOcclusionPayload>(buffer_kind, width);
    default:
        THROW_OX_WRAPPER_ERROR("Unknown buffer format");
    }
}

RaAbstractBuffer RaBasicFactory::createBuffer(RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width, size_t height) const
{
    switch (buffer_format)
    {
    case RaBasicBufferFormat::FLOAT:
        return m_context.createBuffer<float>(buffer_kind, width, height);
    case RaBasicBufferFormat::FLOAT2:
        return m_context.createBuffer<float2>(buffer_kind, width, height);
    case RaBasicBufferFormat::FLOAT3:
        return m_context.createBuffer<float3>(buffer_kind, width, height);
    case RaBasicBufferFormat::FLOAT4:
        return m_context.createBuffer<float4>(buffer_kind, width, height);
    case RaBasicBufferFormat::INT:
        return m_context.createBuffer<int>(buffer_kind, width, height);
    case RaBasicBufferFormat::INT2:
        return m_context.createBuffer<int2>(buffer_kind, width, height);
    case RaBasicBufferFormat::INT3:
        return m_context.createBuffer<int3>(buffer_kind, width, height);
    case RaBasicBufferFormat::INT4:
        return m_context.createBuffer<int4>(buffer_kind, width, height);
    case RaBasicBufferFormat::UINT:
        return m_context.createBuffer<unsigned int>(buffer_kind, width, height);
    case RaBasicBufferFormat::UINT2:
        return m_context.createBuffer<uint2>(buffer_kind, width, height);
    case RaBasicBufferFormat::UINT3:
        return m_context.createBuffer<uint3>(buffer_kind, width, height);
    case RaBasicBufferFormat::UINT4:
        return m_context.createBuffer<uint4>(buffer_kind, width, height);
    case RaBasicBufferFormat::CHAR:
        return m_context.createBuffer<char>(buffer_kind, width, height);
    case RaBasicBufferFormat::CHAR2:
        return m_context.createBuffer<char2>(buffer_kind, width, height);
    case RaBasicBufferFormat::CHAR3:
        return m_context.createBuffer<char3>(buffer_kind, width, height);
    case RaBasicBufferFormat::CHAR4:
        return m_context.createBuffer<char3>(buffer_kind, width, height);
    case RaBasicBufferFormat::UCHAR:
        return m_context.createBuffer<unsigned char>(buffer_kind, width, height);
    case RaBasicBufferFormat::UCHAR2:
        return m_context.createBuffer<uchar2>(buffer_kind, width, height);
    case RaBasicBufferFormat::UCHAR3:
        return m_context.createBuffer<uchar3>(buffer_kind, width, height);
    case RaBasicBufferFormat::UCHAR4:
        return m_context.createBuffer<uchar4>(buffer_kind, width, height);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        return m_context.createBuffer<RaRayRadiancePayload>(buffer_kind, width, height);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        return m_context.createBuffer<RaRayRadiancePayloadSimple>(buffer_kind, width, height);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        return m_context.createBuffer<RaRayRadiancePayloadMonochromatic>(buffer_kind, width, height);
    case RaBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        return m_context.createBuffer<RaRayOcclusionPayload>(buffer_kind, width, height);
    default:
        THROW_OX_WRAPPER_ERROR("Unknown buffer format");
    }
}

RaAbstractBuffer RaBasicFactory::createBuffer(RaBasicBufferFormat buffer_format, RaBufferKind buffer_kind, size_t width, size_t height, size_t depth) const
{
    switch (buffer_format)
    {
    case RaBasicBufferFormat::FLOAT:
        return m_context.createBuffer<float>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::FLOAT2:
        return m_context.createBuffer<float2>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::FLOAT3:
        return m_context.createBuffer<float3>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::FLOAT4:
        return m_context.createBuffer<float4>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::INT:
        return m_context.createBuffer<int>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::INT2:
        return m_context.createBuffer<int2>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::INT3:
        return m_context.createBuffer<int3>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::INT4:
        return m_context.createBuffer<int4>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UINT:
        return m_context.createBuffer<unsigned int>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UINT2:
        return m_context.createBuffer<uint2>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UINT3:
        return m_context.createBuffer<uint3>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UINT4:
        return m_context.createBuffer<uint4>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::CHAR:
        return m_context.createBuffer<char>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::CHAR2:
        return m_context.createBuffer<char2>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::CHAR3:
        return m_context.createBuffer<char3>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::CHAR4:
        return m_context.createBuffer<char3>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UCHAR:
        return m_context.createBuffer<unsigned char>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UCHAR2:
        return m_context.createBuffer<uchar2>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UCHAR3:
        return m_context.createBuffer<uchar3>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::UCHAR4:
        return m_context.createBuffer<uchar4>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD:
        return m_context.createBuffer<RaRayRadiancePayload>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_SIMPLE:
        return m_context.createBuffer<RaRayRadiancePayloadSimple>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::RAY_RADIANCE_PAYLOAD_MONOCHROMATIC:
        return m_context.createBuffer<RaRayRadiancePayloadMonochromatic>(buffer_kind, width, height, depth);
    case RaBasicBufferFormat::RAY_OCCLUSION_PAYLOAD:
        return m_context.createBuffer<RaRayOcclusionPayload>(buffer_kind, width, height, depth);
    default:
        THROW_OX_WRAPPER_ERROR("Unknown buffer format");
    }
}

RaMaterial RaBasicFactory::createMaterial(
    util::Optional<RaProgram> const& closest_hit_shader, 
    util::Optional<RaProgram> const& any_hit_shader, 
    RaRayTypeCollection const& supported_ray_types) const
{
    return RaMaterial{ closest_hit_shader, any_hit_shader, supported_ray_types };
}

RaMaterialAssembly RaBasicFactory::createMaterialAssembly(std::vector<RaMaterial> const& materials) const
{
    return RaMaterialAssembly{ materials };
}

RaMaterialAssembly RaBasicFactory::createDummyMaterialAssembly() const
{
    return RaMaterialAssembly{ m_context };
}

RaGeometry RaBasicFactory::createGeometry(RaProgram const& intersection_shader, RaProgram const& aabb_shader) const
{
    return RaGeometry{ intersection_shader, aabb_shader };
}

RaGeometry RaBasicFactory::createGeometry(RaProgram const& intersection_shader, 
    RaProgram const& aabb_shader, RaMaterialAssembly const& material_assembly) const
{
    return RaGeometry{ intersection_shader, aabb_shader, material_assembly };
}

RaGeometryGroup RaBasicFactory::createGeometryGroup(RaBVHAlgorithm acceleration_structure_construction_algorithm) const
{
    return RaGeometryGroup{ m_context, acceleration_structure_construction_algorithm };
}

RaMissShader RaBasicFactory::createMissShader(RaProgram const& miss_shader, 
    RaRayTypeCollection const& supported_ray_types/* = RaRayTypeCollection{ RaRayType::unknown }*/) const
{
    return RaMissShader{ miss_shader, supported_ray_types };
}

RaMissShaderAssembly RaBasicFactory::createMissShaderAssembly(std::vector<RaMissShader> const& miss_shaders) const
{
    return RaMissShaderAssembly{ miss_shaders };
}

RaRayGeneratorWithOutputBuffer RaBasicFactory::createRayGenerator(RaProgram const& ray_generation_shader, 
    RaAbstractBuffer const& output_buffer, std::string const& output_buffer_binding_name, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z, uint32_t entry_point_index) const
{
    return RaRayGeneratorWithOutputBuffer{ ray_generation_shader, output_buffer, output_buffer_binding_name,
    num_rays_x, num_rays_y, num_rays_z, entry_point_index };
}

RaRayGeneratorWithOutputBuffer RaBasicFactory::createRayGenerator(RaProgram const& ray_generation_shader, 
    RaMissShaderAssembly const& miss_shader_assembly, RaAbstractBuffer const& output_buffer, 
    std::string const& output_buffer_binding_name, 
    uint32_t num_rays_x, uint32_t num_rays_y, uint32_t num_rays_z,
    uint32_t entry_point_index) const
{
    return RaRayGeneratorWithOutputBuffer{ ray_generation_shader, miss_shader_assembly, output_buffer,
    output_buffer_binding_name, num_rays_x, num_rays_y, num_rays_z, entry_point_index };
}

RaSceneSection RaBasicFactory::createSceneSection(RaBVHAlgorithm acceleration_structure_construction_algorithm) const
{
    return RaSceneSection{ m_context, acceleration_structure_construction_algorithm };
}

RaScene RaBasicFactory::createScene() const
{
    return RaScene{};
}

RaBasicFactory::RaBasicFactory(RaContext const& context):
    m_context{ context }
{
    setStringName("RaBasicFactory");

    // Register factory functions in Lua
    {
        // general ra types
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
