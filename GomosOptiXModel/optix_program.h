#ifndef OX_WRAPPER_OPTIX_PROGRAM_H
#define OX_WRAPPER_OPTIX_PROGRAM_H

#include <string>
#include <map>

#include "matrix_types.h"
#include "optix.h"
#include "hashed_string.h"
#include "log.h"
#include "optix_buffer.h"
#include "has_contract_with_optix_context.h"

#include "../CUDA/v9.0/include/vector_types.h"
#include "ox_wrapper_fwd.h"


namespace ox_wrapper {

template<typename T>
class OptiXProgramAttorney;

//! Generic wrapper over OptiX program object
class OptiXProgram final : public HasContractWithOptiXContext
{
    friend class OptiXProgramAttorney<OptiXContext>;
    friend class HasContractWithOptiXProgram;

public:
    enum class Source { string, file };

private:
    OptiXProgram(OptiXContext const& optix_context,
        std::string const& source, Source source_type, std::string const& program_name);
    
public:

    virtual ~OptiXProgram() = default;

    void declareVariable(std::string const& name, float value);
    void declareVariable(std::string const& name, float2 const& value);
    void declareVariable(std::string const& name, float3 const& value);
    void declareVariable(std::string const& name, float4 const& value);

    void declareVariable(std::string const& name, int value);
    void declareVariable(std::string const& name, int2 const& value);
    void declareVariable(std::string const& name, int3 const& value);
    void declareVariable(std::string const& name, int4 const& value);

    void declareVariable(std::string const& name, unsigned int value);
    void declareVariable(std::string const& name, uint2 const& value);
    void declareVariable(std::string const& name, uint3 const& value);
    void declareVariable(std::string const& name, uint4 const& value);

    void declareVariable(std::string const& name, mat2x2 const& value);
    void declareVariable(std::string const& name, mat2x3 const& value);
    void declareVariable(std::string const& name, mat2x4 const& value);

    void declareVariable(std::string const& name, mat3x2 const& value);
    void declareVariable(std::string const& name, mat3x3 const& value);
    void declareVariable(std::string const& name, mat3x4 const& value);

    void declareVariable(std::string const& name, mat4x2 const& value);
    void declareVariable(std::string const& name, mat4x3 const& value);
    void declareVariable(std::string const& name, mat4x4 const& value);


    void setVariableValue(std::string const& name, float value);
    void setVariableValue(std::string const& name, float2 const& value);
    void setVariableValue(std::string const& name, float3 const& value);
    void setVariableValue(std::string const& name, float4 const& value);

    void setVariableValue(std::string const& name, int value);
    void setVariableValue(std::string const& name, int2 const& value);
    void setVariableValue(std::string const& name, int3 const& value);
    void setVariableValue(std::string const& name, int4 const& value);

    void setVariableValue(std::string const& name, unsigned int value);
    void setVariableValue(std::string const& name, uint2 const& value);
    void setVariableValue(std::string const& name, uint3 const& value);
    void setVariableValue(std::string const& name, uint4 const& value);

    void setVariableValue(std::string const& name, mat2x2 const& value);
    void setVariableValue(std::string const& name, mat2x3 const& value);
    void setVariableValue(std::string const& name, mat2x4 const& value);

    void setVariableValue(std::string const& name, mat3x2 const& value);
    void setVariableValue(std::string const& name, mat3x3 const& value);
    void setVariableValue(std::string const& name, mat3x4 const& value);

    void setVariableValue(std::string const& name, mat4x2 const& value);
    void setVariableValue(std::string const& name, mat4x3 const& value);
    void setVariableValue(std::string const& name, mat4x4 const& value);


    template<typename T>
    void assignBuffer(std::string const& name, OptiXBuffer<T> const& buffer)
    {
        declare_variable_object(name);
        rtVariableSetObject(m_program_variable_cache[name], buffer.native());
    }

private:
    std::string m_program_name;
    std::shared_ptr<RTprogram_api> m_native_optix_program;
    std::map<HashedString, RTvariable> m_program_variable_cache;

    void declare_variable_object(std::string const& name);
};


template<> class OptiXProgramAttorney<OptiXContext>
{
    friend class OptiXContext;

    static OptiXProgram createOptiXProgram(OptiXContext const& optix_context, std::string const& source, 
        OptiXProgram::Source source_type, std::string const& program_name)
    {
        return OptiXProgram{ optix_context, source, source_type, program_name };
    }
};


}

#endif
