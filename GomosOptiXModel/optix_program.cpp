#include "optix_program.h"
#include "optix_context.h"
#include "ox_exception.h"

using namespace ox_wrapper;

OptiXProgram::OptiXProgram(OptiXContext const& optix_context,
    std::string const& source, Source source_type, std::string const& program_name):
    HasContractWithOptiXContext{ optix_context },
    m_program_name{ program_name }
{
    RTprogram native_handle;

    switch (source_type)
    {
    case Source::string:
        logOptiXContextError(rtProgramCreateFromPTXString(nativeOptiXContextHandle(), source.c_str(), program_name.c_str(), &native_handle));
        break;
    case Source::file:
        logOptiXContextError(rtProgramCreateFromPTXFile(nativeOptiXContextHandle(), source.c_str(), program_name.c_str(), &native_handle));
        break;
    }

    m_native_optix_program.reset(native_handle, 
        [this](RTprogram h) -> void
    {
        logOptiXContextError(rtProgramDestroy(h));
    });
}

void OptiXProgram::declareVariable(std::string const& name, float value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet1f(m_program_variable_cache[name], value));
}

void OptiXProgram::declareVariable(std::string const& name, float2 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet2f(m_program_variable_cache[name], value.x, value.y));
}

void OptiXProgram::declareVariable(std::string const& name, float3 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet3f(m_program_variable_cache[name], value.x, value.y, value.z));
}

void OptiXProgram::declareVariable(std::string const& name, float4 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet4f(m_program_variable_cache[name], value.x, value.y, value.z, value.w));
}

void OptiXProgram::declareVariable(std::string const& name, int value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet1i(m_program_variable_cache[name], value));
}

void OptiXProgram::declareVariable(std::string const& name, int2 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet2i(m_program_variable_cache[name], value.x, value.y));
}

void OptiXProgram::declareVariable(std::string const& name, int3 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet3i(m_program_variable_cache[name], value.x, value.y, value.z));
}

void OptiXProgram::declareVariable(std::string const& name, int4 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet4i(m_program_variable_cache[name], value.x, value.y, value.z, value.w));
}

void OptiXProgram::declareVariable(std::string const& name, unsigned int value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet1ui(m_program_variable_cache[name], value));
}

void OptiXProgram::declareVariable(std::string const& name, uint2 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet2ui(m_program_variable_cache[name], value.x, value.y));
}

void OptiXProgram::declareVariable(std::string const& name, uint3 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet3ui(m_program_variable_cache[name], value.x, value.y, value.z));
}

void OptiXProgram::declareVariable(std::string const& name, uint4 const& value)
{
    declare_variable_object(name);
    logOptiXContextError(rtVariableSet4ui(m_program_variable_cache[name], value.x, value.y, value.z, value.w));
}

void OptiXProgram::declareVariable(std::string const & name, mat2x2 const & value)
{
    declare_variable_object(name);
    float matrix_data[4] = {
        value._11, value._12,
        value._21, value._22
    };
    logOptiXContextError(rtVariableSetMatrix2x2fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const& name, mat2x3 const& value)
{
    declare_variable_object(name);
    float matrix_data[6] = {
        value._11, value._12, value._13,
        value._21, value._22, value._23
    };
    logOptiXContextError(rtVariableSetMatrix2x3fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const & name, mat2x4 const & value)
{
    declare_variable_object(name);
    float matrix_data[8] = {
        value._11, value._12, value._13, value._14,
        value._21, value._22, value._23, value._24
    };
    logOptiXContextError(rtVariableSetMatrix2x4fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const& name, mat3x2 const& value)
{
    declare_variable_object(name);
    float matrix_data[6] = {
        value._11, value._12,
        value._21, value._22,
        value._31, value._32
    };
    logOptiXContextError(rtVariableSetMatrix3x2fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const& name, mat3x3 const& value)
{
    declare_variable_object(name);
    float matrix_data[9] = {
        value._11, value._12, value._13,
        value._21, value._22, value._23,
        value._31, value._32, value._33
    };
    logOptiXContextError(rtVariableSetMatrix3x3fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const& name, mat3x4 const& value)
{
    declare_variable_object(name);
    float matrix_data[12] = {
        value._11, value._12, value._13, value._14,
        value._21, value._22, value._23, value._24,
        value._31, value._32, value._33, value._34
    };
    logOptiXContextError(rtVariableSetMatrix3x4fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const& name, mat4x2 const& value)
{
    declare_variable_object(name);
    float matrix_data[8] = {
        value._11, value._12,
        value._21, value._22,
        value._31, value._32,
        value._41, value._42
    };
    logOptiXContextError(rtVariableSetMatrix4x2fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const& name, mat4x3 const& value)
{
    declare_variable_object(name);
    float matrix_data[12] = {
        value._11, value._12, value._13,
        value._21, value._22, value._23,
        value._31, value._32, value._33,
        value._41, value._42, value._43
    };
    logOptiXContextError(rtVariableSetMatrix4x3fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::declareVariable(std::string const& name, mat4x4 const& value)
{
    declare_variable_object(name);
    float matrix_data[16] = {
        value._11, value._12, value._13, value._14,
        value._21, value._22, value._23, value._24,
        value._31, value._32, value._33, value._34,
        value._41, value._42, value._43, value._44
    };
    logOptiXContextError(rtVariableSetMatrix4x4fv(m_program_variable_cache[name], false, matrix_data));
}

void OptiXProgram::setVariableValue(std::string const& name, float value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet1f(native_variable_handle, value));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, float2 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet2f(native_variable_handle, value.x, value.y));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, float3 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet3f(native_variable_handle, value.x, value.y, value.z));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, float4 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet4f(native_variable_handle, value.x, value.y, value.z, value.w));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, int value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet1i(native_variable_handle, value));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, int2 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet2i(native_variable_handle, value.x, value.y));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, int3 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet3i(native_variable_handle, value.x, value.y, value.z));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, int4 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet4i(native_variable_handle, value.x, value.y, value.z, value.w));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, unsigned int value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet1ui(native_variable_handle, value));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, uint2 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet2ui(native_variable_handle, value.x, value.y));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, uint3 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet3ui(native_variable_handle, value.x, value.y, value.z));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, uint4 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        logOptiXContextError(rtVariableSet4ui(native_variable_handle, value.x, value.y, value.z, value.w));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat2x2 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12,
            value._21, value._22
        };
        logOptiXContextError(rtVariableSetMatrix2x2fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat2x3 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23
        };
        logOptiXContextError(rtVariableSetMatrix2x3fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat2x4 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24
        };
        logOptiXContextError(rtVariableSetMatrix2x4fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat3x2 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12,
            value._21, value._22,
            value._31, value._32
        };
        logOptiXContextError(rtVariableSetMatrix3x2fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat3x3 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23,
            value._31, value._32, value._33
        };
        logOptiXContextError(rtVariableSetMatrix3x3fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat3x4 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24,
            value._31, value._32, value._33, value._34
        };
        logOptiXContextError(rtVariableSetMatrix3x4fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat4x2 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12,
            value._21, value._22,
            value._31, value._32,
            value._41, value._42,
        };
        logOptiXContextError(rtVariableSetMatrix4x2fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat4x3 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23,
            value._31, value._32, value._33,
            value._41, value._42, value._43
        };
        logOptiXContextError(rtVariableSetMatrix4x3fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::setVariableValue(std::string const& name, mat4x4 const& value)
{
    std::map<HashedString, RTvariable>::iterator i;
    if ((i = m_program_variable_cache.find(HashedString{ name })) != m_program_variable_cache.end())
    {
        RTvariable native_variable_handle = i->second;
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24,
            value._31, value._32, value._33, value._34,
            value._41, value._42, value._43, value._44
        };
        logOptiXContextError(rtVariableSetMatrix4x4fv(native_variable_handle, 0, mdata));
    }
    else
    {
        Log::retrieve()->out("WARNING: variable \"" + name + "\" does not exist", LogMessageType::exclamation);
    }
}

void OptiXProgram::declare_variable_object(std::string const& name)
{
    if(m_program_variable_cache.find(HashedString{ name }) == m_program_variable_cache.end())
    {
        RTvariable new_variable{ nullptr };
        RTresult optix_rc;
        logOptiXContextError(optix_rc = rtProgramDeclareVariable(m_native_optix_program.get(), name.c_str(), &new_variable));

        if (optix_rc != RT_SUCCESS)
            throw OxException{ (R"**(Unable to create OptiX variable for program ")**" + m_program_name + R"**(")**").c_str() };

        m_program_variable_cache.insert(std::make_pair(HashedString{ name }, new_variable));;
    }
}
