#ifndef OX_WRAPPER_PROGRAM_H
#define OX_WRAPPER_PROGRAM_H

#include <string>
#include <map>

#include "vector_types.h"
#include "optix.h"

#include "ra/ra_fwd.h"
#include "ra/buffer.h"
#include "ra/contract_with_context.h"
#include "ra/entity.h"
#include "ra/util/matrix_types.h"
#include "ra/util/log.h"


namespace ra {

struct OxProgramId
{
    int native;
};

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

    OxProgramId getId() const;

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
    void setVariableValue(std::string const& name, OxProgramId const& program_id);

    void getVariableValue(std::string const& name, float& value);
    void getVariableValue(std::string const& name, float2& value);
    void getVariableValue(std::string const& name, float3& value);
    void getVariableValue(std::string const& name, float4& value);

    void getVariableValue(std::string const& name, int& value);
    void getVariableValue(std::string const& name, int2& value);
    void getVariableValue(std::string const& name, int3& value);
    void getVariableValue(std::string const& name, int4& value);

    void getVariableValue(std::string const& name, unsigned int& value);
    void getVariableValue(std::string const& name, uint2& value);
    void getVariableValue(std::string const& name, uint3& value);
    void getVariableValue(std::string const& name, uint4& value);

    void getVariableValue(std::string const& name, util::mat2x2& value);
    void getVariableValue(std::string const& name, util::mat2x3& value);
    void getVariableValue(std::string const& name, util::mat2x4& value);

    void getVariableValue(std::string const& name, util::mat3x2& value);
    void getVariableValue(std::string const& name, util::mat3x3& value);
    void getVariableValue(std::string const& name, util::mat3x4& value);

    void getVariableValue(std::string const& name, util::mat4x2& value);
    void getVariableValue(std::string const& name, util::mat4x3& value);
    void getVariableValue(std::string const& name, util::mat4x4& value);

    void getVariableValue(std::string const& name, OxObjectHandle& value);


    // required by OxEntity interface
    bool isValid() const override;

    void assignBuffer(std::string const& name, OxAbstractBuffer const& buffer);
    void assignProgram(std::string const& name, OxProgram const& program);

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
