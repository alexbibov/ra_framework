#ifndef OX_WRAPPER_PROGRAM_H
#define OX_WRAPPER_PROGRAM_H

#include <string>
#include <map>

#include "fwd.h"
#include "optix.h"
#include "buffer.h"
#include "contract_with_context.h"
#include "entity.h"
#include "util/matrix_types.h"
#include "util/log.h"

#include "../../CUDA/v9.0/include/vector_types.h"


namespace ox_wrapper {

template<typename T>
class OxProgramAttorney;

//! Generic wrapper over OptiX program object
class OxProgram final : public OxContractWithOxContext, public OxEntity
{
    friend class OxProgramAttorney<OxContext>;
    friend class OxContractWithOxPrograms;

public:
    enum class Source { string, file };

private:
    OxProgram(OxContext const& optix_context,
        std::string const& source, Source source_type, std::string const& entry_point);
    
public:

    virtual ~OxProgram() = default;

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

    void declareVariable(std::string const& name, util::mat2x2 const& value);
    void declareVariable(std::string const& name, util::mat2x3 const& value);
    void declareVariable(std::string const& name, util::mat2x4 const& value);

    void declareVariable(std::string const& name, util::mat3x2 const& value);
    void declareVariable(std::string const& name, util::mat3x3 const& value);
    void declareVariable(std::string const& name, util::mat3x4 const& value);

    void declareVariable(std::string const& name, util::mat4x2 const& value);
    void declareVariable(std::string const& name, util::mat4x3 const& value);
    void declareVariable(std::string const& name, util::mat4x4 const& value);

    void declareVariable(std::string const& name, OxObjectHandle const& value);


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

    void setVariableValue(std::string const& name, util::mat2x2 const& value);
    void setVariableValue(std::string const& name, util::mat2x3 const& value);
    void setVariableValue(std::string const& name, util::mat2x4 const& value);

    void setVariableValue(std::string const& name, util::mat3x2 const& value);
    void setVariableValue(std::string const& name, util::mat3x3 const& value);
    void setVariableValue(std::string const& name, util::mat3x4 const& value);

    void setVariableValue(std::string const& name, util::mat4x2 const& value);
    void setVariableValue(std::string const& name, util::mat4x3 const& value);
    void setVariableValue(std::string const& name, util::mat4x4 const& value);

    void setVariableValue(std::string const& name, OxObjectHandle const& value);

    // required by OxEntity interface
    bool isValid() const override;

    template<typename T>
    void assignBuffer(std::string const& name, OxBuffer<T> const& buffer)
    {
        RTvariable native_variable_handle = declare_variable_object(name);
        rtVariableSetObject(native_variable_handle, buffer.native());
    }

    bool checkVariableExistance(std::string const& name) const;

private:
    std::shared_ptr<RTprogram_api> m_native_optix_program;

    RTvariable fetch_variable(std::string const& name);
    RTvariable declare_variable_object(std::string const& name);
};


template<> class OxProgramAttorney<OxContext>
{
    friend class OxContext;

    static OxProgram createOptiXProgram(OxContext const& optix_context, std::string const& source, 
        OxProgram::Source source_type, std::string const& program_name)
    {
        return OxProgram{ optix_context, source, source_type, program_name };
    }
};


}

#endif
