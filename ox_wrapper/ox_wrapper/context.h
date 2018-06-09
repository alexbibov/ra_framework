#ifndef OX_WRAPPER_CONTEXT_H
#define OX_WRAPPER_CONTEXT_H

#include <string>
#include <memory>
#include <vector>

#include "optix.h"
#include "program.h"
#include "buffer.h"
#include "entity.h"

namespace ox_wrapper {

//! Simple wrapper over the OptiX context
class OxContext final : public OxEntity
{
    friend class OxContractWithOxContext;

public:

    OxContext(std::vector<std::string> const& asset_directories, uint32_t num_entry_points = 1U);
    OxContext(OxContext const&) = delete;
    OxContext(OxContext&&) = delete;
    OxContext& operator=(OxContext const&) = delete;
    OxContext& operator=(OxContext&&) = delete;
    ~OxContext();

    //! Sets stack size to be used by the context, returns 'true' on success and false on failure
    void setStackSize(size_t size_in_bytes) const;

    //! Creates new OptiX program
    OxProgram createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const;

    //! Creates new 1D OptiX buffer
    template<typename T>
    OxBuffer<T> createBuffer(OxBufferKind buffer_kind, size_t width) const
    {
        return OxBufferAttorney<OxContext>::createOptiXBuffer<T>(*this, buffer_kind, width);
    }

    //! Creates new 2D OptiX buffer
    template<typename T>
    OxBuffer<T> createBuffer(OxBufferKind buffer_kind, size_t width, size_t height) const
    {
        return OxBufferAttorney<OxContext>::createOptiXBuffer<T>(*this, buffer_kind, width, height);
    }

    //! Creates new 3D OptiX buffer
    template<typename T>
    OxBuffer<T> createBuffer(OxBufferKind buffer_kind, size_t width, size_t height, size_t depth) const
    {
        return OxBufferAttorney<OxContext>::createOptiXBuffer<T>(*this, buffer_kind, width, height, depth);
    }

    //! Returns 'true' in case if OptiX context is NOT in error state; returns 'false' otherwise
    operator bool() const;

    //! Returns 'true' in case if OptiX context IS in erroneous state; returns 'false' otherwise
    bool hasErrors() const;

    //! required by OxEntity interface
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
