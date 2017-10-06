#ifndef OX_WRAPPER_OPTIX_PROGRAM_H
#define OX_WRAPPER_OPTIX_PROGRAM_H

#include <string>
#include <map>

#include "matrix_types.h"
#include "optix.h"
#include "hashed_string.h"
#include "log.h"
#include "optix_buffer.h"

#include "../CUDA/v9.0/include/vector_types.h"

namespace ox_wrapper {

class OptiXContext;

//! Generic wrapper over OptiX program object
class OptiXProgram {
    friend class OptiXContext;

public:

    enum class Source { string, file };

protected:

    OptiXProgram(OptiXContext const& optix_context, std::string const& source, 
        Source source_type, std::string const& program_name);
    
public:

    virtual ~OptiXProgram();

    OptiXContext const& getOptiXContext() const;

    RTprogram native() const;


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
    OptiXContext const& m_optix_context;
    std::string m_program_name;
    RTprogram m_optix_program;
    std::map<HashedString, RTvariable> m_program_variable_cache;

    void declare_variable_object(std::string const& name);
};

}

#endif
