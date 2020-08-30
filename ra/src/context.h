#ifndef RA_CONTEXT_H
#define RA_CONTEXT_H

#include <string>
#include <memory>
#include <vector>

#include "optix.h"
#include "program.h"
#include "buffer.h"
#include "entity.h"
#include "texture.h"

namespace ra {

//! Simple wrapper over the OptiX context
class RaContext final : public RaEntity
{
    friend class RaContractWithRaContext;

public:

    RaContext(std::vector<std::string> const& asset_directories, uint32_t num_entry_points = 1U);
    RaContext(RaContext const&) = delete;
    RaContext(RaContext&&) = delete;
    RaContext& operator=(RaContext const&) = delete;
    RaContext& operator=(RaContext&&) = delete;
    ~RaContext();

    //! Creates new OptiX program
    RaProgram createProgram(std::string const& source, RaProgram::Source source_type, std::string const& program_name) const;

    //! Creates new 1D OptiX buffer
    template<typename T>
    RaBuffer<T> createBuffer(RaBufferKind buffer_kind, size_t width) const
    {
        return RaBufferAttorney<RaContext>::createOptiXBuffer<T>(*this, buffer_kind, width);
    }

    //! Creates new 2D OptiX buffer
    template<typename T>
    RaBuffer<T> createBuffer(RaBufferKind buffer_kind, size_t width, size_t height) const
    {
        return RaBufferAttorney<RaContext>::createOptiXBuffer<T>(*this, buffer_kind, width, height);
    }

    //! Creates new 3D OptiX buffer
    template<typename T>
    RaBuffer<T> createBuffer(RaBufferKind buffer_kind, size_t width, size_t height, size_t depth) const
    {
        return RaBufferAttorney<RaContext>::createOptiXBuffer<T>(*this, buffer_kind, width, height, depth);
    }

    //! Creates texture from given image file
    std::shared_ptr<RaTexture> createTextureFromFile(std::string& filename, bool* is_hdr_texture, uint8_t *number_of_channels);

    //! Returns 'true' in case if OptiX context is NOT in error state; returns 'false' otherwise
    operator bool() const;

    //! Returns 'true' in case if OptiX context IS in erroneous state; returns 'false' otherwise
    bool hasErrors() const;

    //! required by RaEntity interface
    bool isValid() const override;

    //! reads requested string asset
    std::string retrieveStringAsset(std::string const& source) const;

    //! adds custom asset look-up directory
    void addAssetCustomLookUpDirectory(std::string const& directory);

    //! clear custom asset directory look-up cache
    void clearAssetCustomDirectoryLookUpCache();

private:
    std::vector<std::string> m_asset_directories;
    size_t m_default_asset_directories_cache_size;
    size_t m_first_custom_asset_directory_idx;
    RTcontext m_optix_context;
    mutable RTresult m_error_state;
};

}

#endif
