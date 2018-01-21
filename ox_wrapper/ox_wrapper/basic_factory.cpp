#include "basic_factory.h"
#include "util/lua_support.h"
#include "context.h"

using namespace ox_wrapper;
using namespace ox_wrapper::util;


namespace {
static OxBasicFactory* p_basic_factory_instance{ nullptr };
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

OxBasicFactory::OxBasicFactory(OxContext const& context):
    m_context{ context }
{
    setStringName("OxBasicFactory");

    // Register factory functions in Lua
    {
        // General types
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
        }

        // OxProgram support
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

            lua_support::LuaState::registerType<OxProgram>(
                "OxProgram",
                lua_support::ListOfFactories::make_initializer(
                    [this](std::string const& source, OxProgram::Source source_type, std::string const& program_name)
                    {
                        return createProgram(source, source_type, program_name);
                    }
                ),

                "getUniqueIdentifier", &OxEntity::getUniqueIdentifier,
                "getStringName", &OxEntity::getStringName,

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

    }



}
