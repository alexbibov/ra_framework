#include "program.h"
#include "context.h"
#include "exception.h"

using namespace ox_wrapper;

OxProgram::OxProgram(OxContext const& optix_context,
    std::string const& source, Source source_type, std::string const& entry_point):
    OxContractWithOxContext{ optix_context }
{
    setStringName(entry_point.c_str());

    RTprogram native_handle{};

    switch (source_type)
    {
    case Source::string:
        throwOptiXContextError(rtProgramCreateFromPTXString(nativeOptiXContextHandle(), source.c_str(), entry_point.c_str(), &native_handle));
        break;
    case Source::file:
        throwOptiXContextError(rtProgramCreateFromPTXFile(nativeOptiXContextHandle(), source.c_str(), entry_point.c_str(), &native_handle));
        break;
    }

    m_native_optix_program.reset(native_handle, 
        [this](RTprogram h) -> void
    {
        logOptiXContextError(rtProgramDestroy(h));
    });
}

void OxProgram::declareVariable(std::string const& name, float value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet1f(new_variable_handle, value));
}

void OxProgram::declareVariable(std::string const& name, float2 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet2f(new_variable_handle, value.x, value.y));
}

void OxProgram::declareVariable(std::string const& name, float3 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet3f(new_variable_handle, value.x, value.y, value.z));
}

void OxProgram::declareVariable(std::string const& name, float4 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet4f(new_variable_handle, value.x, value.y, value.z, value.w));
}

void OxProgram::declareVariable(std::string const& name, int value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet1i(new_variable_handle, value));
}

void OxProgram::declareVariable(std::string const& name, int2 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet2i(new_variable_handle, value.x, value.y));
}

void OxProgram::declareVariable(std::string const& name, int3 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet3i(new_variable_handle, value.x, value.y, value.z));
}

void OxProgram::declareVariable(std::string const& name, int4 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet4i(new_variable_handle, value.x, value.y, value.z, value.w));
}

void OxProgram::declareVariable(std::string const& name, unsigned int value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet1ui(new_variable_handle, value));
}

void OxProgram::declareVariable(std::string const& name, uint2 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet2ui(new_variable_handle, value.x, value.y));
}

void OxProgram::declareVariable(std::string const& name, uint3 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet3ui(new_variable_handle, value.x, value.y, value.z));
}

void OxProgram::declareVariable(std::string const& name, uint4 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSet4ui(new_variable_handle, value.x, value.y, value.z, value.w));
}

void OxProgram::declareVariable(std::string const & name, util::mat2x2 const & value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[4] = {
        value._11, value._12,
        value._21, value._22
    };
    throwOptiXContextError(rtVariableSetMatrix2x2fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const& name, util::mat2x3 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[6] = {
        value._11, value._12, value._13,
        value._21, value._22, value._23
    };
    throwOptiXContextError(rtVariableSetMatrix2x3fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const & name, util::mat2x4 const & value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[8] = {
        value._11, value._12, value._13, value._14,
        value._21, value._22, value._23, value._24
    };
    throwOptiXContextError(rtVariableSetMatrix2x4fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const& name, util::mat3x2 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[6] = {
        value._11, value._12,
        value._21, value._22,
        value._31, value._32
    };
    throwOptiXContextError(rtVariableSetMatrix3x2fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const& name, util::mat3x3 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[9] = {
        value._11, value._12, value._13,
        value._21, value._22, value._23,
        value._31, value._32, value._33
    };
    throwOptiXContextError(rtVariableSetMatrix3x3fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const& name, util::mat3x4 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[12] = {
        value._11, value._12, value._13, value._14,
        value._21, value._22, value._23, value._24,
        value._31, value._32, value._33, value._34
    };
    throwOptiXContextError(rtVariableSetMatrix3x4fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const& name, util::mat4x2 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[8] = {
        value._11, value._12,
        value._21, value._22,
        value._31, value._32,
        value._41, value._42
    };
    throwOptiXContextError(rtVariableSetMatrix4x2fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const& name, util::mat4x3 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[12] = {
        value._11, value._12, value._13,
        value._21, value._22, value._23,
        value._31, value._32, value._33,
        value._41, value._42, value._43
    };
    throwOptiXContextError(rtVariableSetMatrix4x3fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const& name, util::mat4x4 const& value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    float matrix_data[16] = {
        value._11, value._12, value._13, value._14,
        value._21, value._22, value._23, value._24,
        value._31, value._32, value._33, value._34,
        value._41, value._42, value._43, value._44
    };
    throwOptiXContextError(rtVariableSetMatrix4x4fv(new_variable_handle, false, matrix_data));
}

void OxProgram::declareVariable(std::string const & name, OxObjectHandle const & value)
{
    RTvariable new_variable_handle = declare_variable_object(name);
    throwOptiXContextError(rtVariableSetObject(new_variable_handle, static_cast<RTobject>(value.p_native)));
}

void OxProgram::setVariableValue(std::string const& name, float value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet1f(native_variable_handle, value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, float2 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet2f(native_variable_handle, value.x, value.y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, float3 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet3f(native_variable_handle, value.x, value.y, value.z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, float4 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet4f(native_variable_handle, value.x, value.y, value.z, value.w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, int value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet1i(native_variable_handle, value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, int2 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet2i(native_variable_handle, value.x, value.y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, int3 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet3i(native_variable_handle, value.x, value.y, value.z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, int4 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet4i(native_variable_handle, value.x, value.y, value.z, value.w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, unsigned int value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet1ui(native_variable_handle, value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, uint2 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet2ui(native_variable_handle, value.x, value.y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, uint3 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet3ui(native_variable_handle, value.x, value.y, value.z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, uint4 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSet4ui(native_variable_handle, value.x, value.y, value.z, value.w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat2x2 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12,
            value._21, value._22
        };
        throwOptiXContextError(rtVariableSetMatrix2x2fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat2x3 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23
        };
        throwOptiXContextError(rtVariableSetMatrix2x3fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat2x4 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24
        };
        throwOptiXContextError(rtVariableSetMatrix2x4fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat3x2 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12,
            value._21, value._22,
            value._31, value._32
        };
        throwOptiXContextError(rtVariableSetMatrix3x2fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat3x3 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23,
            value._31, value._32, value._33
        };
        throwOptiXContextError(rtVariableSetMatrix3x3fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat3x4 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24,
            value._31, value._32, value._33, value._34
        };
        throwOptiXContextError(rtVariableSetMatrix3x4fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat4x2 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12,
            value._21, value._22,
            value._31, value._32,
            value._41, value._42,
        };
        throwOptiXContextError(rtVariableSetMatrix4x2fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat4x3 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23,
            value._31, value._32, value._33,
            value._41, value._42, value._43
        };
        throwOptiXContextError(rtVariableSetMatrix4x3fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, util::mat4x4 const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24,
            value._31, value._32, value._33, value._34,
            value._41, value._42, value._43, value._44
        };
        throwOptiXContextError(rtVariableSetMatrix4x4fv(native_variable_handle, 0, mdata));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::setVariableValue(std::string const& name, OxObjectHandle const& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        throwOptiXContextError(rtVariableSetObject(native_variable_handle, static_cast<RTobject>(value.p_native)));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exits", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, float* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet1f(native_variable_handle, value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, float2* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet2f(native_variable_handle, &value->x, &value->y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, float3* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet3f(native_variable_handle, &value->x, &value->y, &value->z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, float4* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet4f(native_variable_handle, &value->x, &value->y, &value->z, &value->w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, int* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet1i(native_variable_handle, value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, int2* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet2i(native_variable_handle, &value->x, &value->y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, int3* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet3i(native_variable_handle, &value->x, &value->y, &value->z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, int4* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet4i(native_variable_handle, &value->x, &value->y, &value->z, &value->w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, unsigned int* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet1ui(native_variable_handle, value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, uint2* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet2ui(native_variable_handle, &value->x, &value->y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, uint3* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet3ui(native_variable_handle, &value->x, &value->y, &value->z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, uint4* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        logOptiXContextError(rtVariableGet4ui(native_variable_handle, &value->x, &value->y, &value->z, &value->w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat2x2* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[4];
        logOptiXContextError(rtVariableGetMatrix2x2fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1];
        value->_21 = m[2]; value->_22 = m[3];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat2x3* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[6];
        logOptiXContextError(rtVariableGetMatrix2x3fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1]; value->_13 = m[2];
        value->_21 = m[3]; value->_22 = m[4]; value->_23 = m[5];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat2x4* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[8];
        logOptiXContextError(rtVariableGetMatrix2x4fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1]; value->_13 = m[2]; value->_14 = m[3];
        value->_21 = m[4]; value->_22 = m[5]; value->_23 = m[6]; value->_24 = m[7];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat3x2* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[6];
        logOptiXContextError(rtVariableGetMatrix3x2fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1]; 
        value->_21 = m[2]; value->_22 = m[3];
        value->_31 = m[4]; value->_32 = m[5];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat3x3* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[9];
        logOptiXContextError(rtVariableGetMatrix3x3fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1]; value->_13 = m[2];
        value->_21 = m[3]; value->_22 = m[4]; value->_23 = m[5];
        value->_31 = m[6]; value->_32 = m[7]; value->_33 = m[8];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat3x4* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[12];
        logOptiXContextError(rtVariableGetMatrix3x4fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1]; value->_13 = m[2];  value->_14 = m[3];
        value->_21 = m[4]; value->_22 = m[5]; value->_23 = m[6];  value->_24 = m[7];
        value->_31 = m[8]; value->_32 = m[9]; value->_33 = m[10]; value->_34 = m[11];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat4x2* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[8];
        logOptiXContextError(rtVariableGetMatrix4x2fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1];
        value->_21 = m[2]; value->_22 = m[3];
        value->_31 = m[4]; value->_32 = m[5];
        value->_41 = m[6]; value->_42 = m[7];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat4x3* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[12];
        logOptiXContextError(rtVariableGetMatrix4x3fv(native_variable_handle, 0, m));
        value->_11 = m[0]; value->_12 = m[1];  value->_13 = m[2];
        value->_21 = m[3]; value->_22 = m[4];  value->_23 = m[5];
        value->_31 = m[6]; value->_32 = m[7];  value->_33 = m[8];
        value->_41 = m[9]; value->_42 = m[10]; value->_43 = m[11];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, util::mat4x4* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[16];
        logOptiXContextError(rtVariableGetMatrix4x4fv(native_variable_handle, 0, m));
        value->_11 = m[0];  value->_12 = m[1];  value->_13 = m[2];  value->_14 = m[3];
        value->_21 = m[4];  value->_22 = m[5];  value->_23 = m[6];  value->_24 = m[7];
        value->_31 = m[8];  value->_32 = m[9];  value->_33 = m[10]; value->_34 = m[11];
        value->_41 = m[12]; value->_42 = m[13]; value->_43 = m[14]; value->_44 = m[15];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void OxProgram::getVariableValue(std::string const& name, OxObjectHandle* value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        RTobject native_object_handle;
        logOptiXContextError(rtVariableGetObject(native_variable_handle, &native_object_handle));
        value->p_native = native_object_handle;
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}


bool OxProgram::isValid() const
{
     RTresult res = rtProgramValidate(m_native_optix_program.get());
     logOptiXContextError(res);
     return res == RT_SUCCESS;
}

bool OxProgram::checkVariableExistance(std::string const& name) const
{
    RTvariable v{ nullptr };
    throwOptiXContextError(rtProgramQueryVariable(m_native_optix_program.get(), name.c_str(), &v));
    return v != nullptr;
}

RTvariable ox_wrapper::OxProgram::fetch_variable(std::string const& name)
{
    RTvariable rv{ nullptr };
    logOptiXContextError(rtProgramQueryVariable(m_native_optix_program.get(), name.c_str(), &rv));

    return rv;
}

RTvariable OxProgram::declare_variable_object(std::string const& name)
{
    RTvariable rv{ nullptr };
    if(!(rv = fetch_variable(name)))
    {
        RTresult optix_rc;
        throwOptiXContextError(optix_rc = rtProgramDeclareVariable(m_native_optix_program.get(), name.c_str(), &rv));

        if (optix_rc != RT_SUCCESS)
            throw OxException{ (R"**(Unable to create OptiX variable ")**" + name
                + R"**(" for program ")**"
                + getStringName() + R"**(")**").c_str() };
    }

    return rv;
}
