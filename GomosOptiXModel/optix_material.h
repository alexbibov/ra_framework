#ifndef OX_WRAPPER_OPTIX_MATERIAL_H
#define OX_WRAPPER_OPTIX_MATERIAL_H

#include "ox_wrapper_fwd.h"
#include "optix.h"

#include <string>

namespace ox_wrapper {

template<typename T>
class OptiXMaterialAttorney;

class OptiXMaterial
{
    friend class OptiXMaterialAttorney<OptiXGeometry>;

public:
    OptiXMaterial(std::string const& name, OptiXProgram const& hit_shader);
    OptiXMaterial(std::string const& name, OptiXProgram const& hit_shader, OptiXProgram const& miss_shader);

    RTmaterial native() const;
    std::string name() const;
    void setName(std::string const& name);

private:
    OptiXProgram const& getHitShader() const;
    OptiXProgram const& getMissShader() const;

private:
    static uint64_t material_id_counter;

    uint64_t material_id;
    std::string name;
    OptiXProgram hit_shader;
    OptiXProgram miss_shader;
    RTmaterial m_native_material;
};

template<>
class OptiXMaterialAttorney<OptiXGeometry>
{
    friend OptiXGeometry;


};

}

#endif
