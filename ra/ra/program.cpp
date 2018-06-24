#include "program.h"
#include "context.h"
#include "exception.h"

using namespace ra;

RaProgram::RaProgram(RaContext const& optix_context,
    std::string const& source, Source source_type, std::string const& entry_point):
    RaContractWithRaContext{ optix_context }
{
    setStringName(entry_point.c_str());

    RTprogram native_handle{};

    switch (source_type)
    {
    case Source::string:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtProgramCreateFromPTXString(nativeOptiXContextHandle(), source.c_str(), entry_point.c_str(), &native_handle));
        break;
    case Source::file:
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtProgramCreateFromPTXFile(nativeOptiXContextHandle(), source.c_str(), entry_point.c_str(), &native_handle));
        break;
    }

    m_native_optix_program.reset(native_handle, 
        [this](RTprogram h) -> void
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtProgramDestroy(h));
    });
}

RaProgramId RaProgram::getId() const
{
    int id;
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtProgramGetId(m_native_optix_program.get(), &id));

    return RaProgramId{ id };
}

void RaProgram::setVariableValue(std::string const& name, float value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet1f(native_variable_handle, value));
}

void RaProgram::setVariableValue(std::string const& name, float2 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet2f(native_variable_handle, value.x, value.y));
}

void RaProgram::setVariableValue(std::string const& name, float3 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet3f(native_variable_handle, value.x, value.y, value.z));
}

void RaProgram::setVariableValue(std::string const& name, float4 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet4f(native_variable_handle, value.x, value.y, value.z, value.w));
}

void RaProgram::setVariableValue(std::string const& name, int value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet1i(native_variable_handle, value));
}

void RaProgram::setVariableValue(std::string const& name, int2 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet2i(native_variable_handle, value.x, value.y));
}

void RaProgram::setVariableValue(std::string const& name, int3 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet3i(native_variable_handle, value.x, value.y, value.z));
}

void RaProgram::setVariableValue(std::string const& name, int4 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet4i(native_variable_handle, value.x, value.y, value.z, value.w));
}

void RaProgram::setVariableValue(std::string const& name, unsigned int value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet1ui(native_variable_handle, value));
}

void RaProgram::setVariableValue(std::string const& name, uint2 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet2ui(native_variable_handle, value.x, value.y));
}

void RaProgram::setVariableValue(std::string const& name, uint3 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet3ui(native_variable_handle, value.x, value.y, value.z));
}

void RaProgram::setVariableValue(std::string const& name, uint4 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet4ui(native_variable_handle, value.x, value.y, value.z, value.w));
}

void RaProgram::setVariableValue(std::string const& name, util::mat2x2 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12,
            value._21, value._22
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix2x2fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat2x3 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix2x3fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat2x4 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix2x4fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat3x2 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12,
            value._21, value._22,
            value._31, value._32
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix3x2fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat3x3 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23,
            value._31, value._32, value._33
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix3x3fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat3x4 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24,
            value._31, value._32, value._33, value._34
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix3x4fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat4x2 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12,
            value._21, value._22,
            value._31, value._32,
            value._41, value._42,
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix4x2fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat4x3 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12, value._13,
            value._21, value._22, value._23,
            value._31, value._32, value._33,
            value._41, value._42, value._43
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix4x3fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, util::mat4x4 const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    {
        float mdata[] = {
            value._11, value._12, value._13, value._14,
            value._21, value._22, value._23, value._24,
            value._31, value._32, value._33, value._34,
            value._41, value._42, value._43, value._44
        };
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetMatrix4x4fv(native_variable_handle, 0, mdata));
    }
}

void RaProgram::setVariableValue(std::string const& name, RaObjectHandle const& value)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSetObject(native_variable_handle, static_cast<RTobject>(value.p_native)));
}

void RaProgram::setVariableValue(std::string const& name, RaProgramId const& program_id)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableSet1i(native_variable_handle, program_id.native));
}

void RaProgram::getVariableValue(std::string const& name, float& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet1f(native_variable_handle, &value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, float2& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet2f(native_variable_handle, &value.x, &value.y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, float3& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet3f(native_variable_handle, &value.x, &value.y, &value.z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, float4& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet4f(native_variable_handle, &value.x, &value.y, &value.z, &value.w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, int& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet1i(native_variable_handle, &value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, int2& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet2i(native_variable_handle, &value.x, &value.y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, int3& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet3i(native_variable_handle, &value.x, &value.y, &value.z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, int4& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet4i(native_variable_handle, &value.x, &value.y, &value.z, &value.w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, unsigned int& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet1ui(native_variable_handle, &value));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, uint2& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet2ui(native_variable_handle, &value.x, &value.y));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, uint3& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet3ui(native_variable_handle, &value.x, &value.y, &value.z));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, uint4& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGet4ui(native_variable_handle, &value.x, &value.y, &value.z, &value.w));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat2x2& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[4];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix2x2fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1];
        value._21 = m[2]; value._22 = m[3];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat2x3& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[6];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix2x3fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1]; value._13 = m[2];
        value._21 = m[3]; value._22 = m[4]; value._23 = m[5];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat2x4& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[8];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix2x4fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1]; value._13 = m[2]; value._14 = m[3];
        value._21 = m[4]; value._22 = m[5]; value._23 = m[6]; value._24 = m[7];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat3x2& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[6];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix3x2fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1]; 
        value._21 = m[2]; value._22 = m[3];
        value._31 = m[4]; value._32 = m[5];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat3x3& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[9];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix3x3fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1]; value._13 = m[2];
        value._21 = m[3]; value._22 = m[4]; value._23 = m[5];
        value._31 = m[6]; value._32 = m[7]; value._33 = m[8];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat3x4& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[12];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix3x4fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1]; value._13 = m[2];  value._14 = m[3];
        value._21 = m[4]; value._22 = m[5]; value._23 = m[6];  value._24 = m[7];
        value._31 = m[8]; value._32 = m[9]; value._33 = m[10]; value._34 = m[11];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat4x2& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[8];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix4x2fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1];
        value._21 = m[2]; value._22 = m[3];
        value._31 = m[4]; value._32 = m[5];
        value._41 = m[6]; value._42 = m[7];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat4x3& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[12];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix4x3fv(native_variable_handle, 0, m));
        value._11 = m[0]; value._12 = m[1];  value._13 = m[2];
        value._21 = m[3]; value._22 = m[4];  value._23 = m[5];
        value._31 = m[6]; value._32 = m[7];  value._33 = m[8];
        value._41 = m[9]; value._42 = m[10]; value._43 = m[11];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, util::mat4x4& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        float m[16];
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetMatrix4x4fv(native_variable_handle, 0, m));
        value._11 = m[0];  value._12 = m[1];  value._13 = m[2];  value._14 = m[3];
        value._21 = m[4];  value._22 = m[5];  value._23 = m[6];  value._24 = m[7];
        value._31 = m[8];  value._32 = m[9];  value._33 = m[10]; value._34 = m[11];
        value._41 = m[12]; value._42 = m[13]; value._43 = m[14]; value._44 = m[15];
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}

void RaProgram::getVariableValue(std::string const& name, RaObjectHandle& value)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        RTobject native_object_handle;
        LOG_OPTIX_ERROR(nativeOptiXContextHandle(), rtVariableGetObject(native_variable_handle, &native_object_handle));
        value.p_native = native_object_handle;
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to get value of variable \"" + name + "\": variable does not exist", util::LogMessageType::exclamation);
    }
}


bool RaProgram::isValid() const
{
     RTresult res = rtProgramValidate(m_native_optix_program.get());
     LOG_OPTIX_ERROR(nativeOptiXContextHandle(), res);
     return res == RT_SUCCESS;
}

void RaProgram::assignBuffer(std::string const& name, RaAbstractBuffer const& buffer)
{
    RTvariable native_variable_handle = declare_variable_object(name);
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), 
        rtVariableSetObject(native_variable_handle, RaAbstractBufferAttorney<RaProgram>::getNativeOptiXBufferHandle(buffer)));
}

void RaProgram::assignProgram(std::string const & name, RaProgram const & program)
{
    RTvariable native_variable_handle{};
    if (native_variable_handle = fetch_variable(name))
    {
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(),
            rtVariableSetObject(native_variable_handle, program.m_native_optix_program.get()));
    }
    else
    {
        util::Log::retrieve()->out("WARNING: unable to assign program object to variable \"" + name + "\". Variable with this name does not exist", util::LogMessageType::exclamation);
    }
}

bool RaProgram::checkVariableExistance(std::string const& name) const
{
    RTvariable v{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtProgramQueryVariable(m_native_optix_program.get(), name.c_str(), &v));
    return v != nullptr;
}

RTvariable ra::RaProgram::fetch_variable(std::string const& name)
{
    RTvariable rv{ nullptr };
    THROW_OPTIX_ERROR(nativeOptiXContextHandle(), rtProgramQueryVariable(m_native_optix_program.get(), name.c_str(), &rv));

    return rv;
}

RTvariable RaProgram::declare_variable_object(std::string const& name)
{
    RTvariable rv{ nullptr };
    if(!(rv = fetch_variable(name)))
    {
        RTresult optix_rc;
        THROW_OPTIX_ERROR(nativeOptiXContextHandle(), optix_rc = rtProgramDeclareVariable(m_native_optix_program.get(), name.c_str(), &rv));

        if (optix_rc != RT_SUCCESS)
            THROW_OX_WRAPPER_ERROR(R"**(Unable to declare OptiX variable ")**" + name
                + R"**(" for program ")**"
                + getStringName() + R"**(")**");
    }

    return rv;
}
