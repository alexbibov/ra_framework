#ifndef RA_RA_FWD_H
#define RA_RA_FWD_H

namespace ra {
class RaEntity;
class RaEntityId;
class RaContext;
class RaContractWithRaContext;
class RaProgram;
class RaContractWithRaPrograms;
class RaAbstractBuffer;
template<typename T> class RaBuffer;
template<typename T> class RaBufferMapSentry;
class RaScene;
class RaSceneSection;
class RaGeometryGroup;
class OptiXVariableGeometryGroup;
class RaGeometry;
class RaTransform;
class RaTransformable;
class RaMaterial;
class RaMaterialAssembly;
class RaRayGenerator;
class RaMissShader;
class RaMissShaderAssembly;
class RaFactoryInitializerSentinel;
class RaBasicFactory;
class RaRayGeneratorWithOutputBuffer;
class RaTraverseBackupBuffer;
class RaTexture;
template<typename T> class RaTypedTexture;
template<typename T> class RaTexture2D;
}

#endif