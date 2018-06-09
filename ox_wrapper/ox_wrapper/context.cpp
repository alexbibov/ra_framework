#include "context.h"
#include "exception.h"
#include "ray_payloads.h"
#include "util/optional.h"
#include "util/misc.h"

#include <fstream>

using namespace ox_wrapper;

namespace
{

std::string getPathAnchor(std::string const& path)
{
    if (path.length())
    {
        char last_symbol = path[path.length() - 1];
        if (last_symbol == '/' || last_symbol == '\\')
            return "";

        size_t anchor_pos = path.find_last_of("/\\");
        if (anchor_pos == std::string::npos) return path;
        return std::string(path.begin() + anchor_pos + 1, path.end());
    }
    else
        return "";
}

util::Optional<std::string> getPathToAsset(std::string const& asset_name, std::vector<std::string> const& asset_search_directories)
{
    std::ifstream ifile;
    std::string target_path;

    ifile.open(asset_name);
    if (ifile)
    {
        ifile.close();
        return util::Optional<std::string>{asset_name};
    }

    std::string anchor = '/' + getPathAnchor(asset_name);
    bool asset_found{ false };
    for (auto const& asset_dir : asset_search_directories)
    {
        target_path = asset_dir + anchor;
        ifile.open(target_path, std::ios::in);
        if (ifile)
        {
            ifile.close();
            asset_found = true;
            break;
        }
    }

    if (asset_found)
        return util::Optional<std::string>{ target_path };
    else 
        return util::Optional<std::string>{};
}

}


OxContext::OxContext(std::vector<std::string> const& asset_directories, uint32_t num_entry_points/* = 1U*/):
    m_asset_directories{ asset_directories },
    m_default_asset_directories_cache_size{ m_asset_directories.size() },
    m_error_state{ RT_SUCCESS }
{
    RTresult optix_rc = rtContextCreate(&m_optix_context);

    if (optix_rc == RT_ERROR_NO_DEVICE)
    {
        THROW_OX_WRAPPER_ERROR("OptiX is not supported by the GPU installed in the host system");
    }

    if (optix_rc == RT_ERROR_INVALID_VALUE)
    {
        THROW_OX_WRAPPER_ERROR("OptiX context cannot be initialized: invalid value");
    }

    LOG_OPTIX_ERROR(m_optix_context, rtContextSetEntryPointCount(m_optix_context, num_entry_points));
    LOG_OPTIX_ERROR(m_optix_context, rtContextSetRayTypeCount(m_optix_context, static_cast<unsigned int>(OxRayType::count)));
    isValid();
}

OxContext::~OxContext()
{
    LOG_OPTIX_ERROR(m_optix_context, rtContextDestroy(m_optix_context));
}

void OxContext::setStackSize(size_t size_in_bytes) const
{
    LOG_OPTIX_ERROR(m_optix_context,
        rtContextSetStackSize(m_optix_context, static_cast<RTsize>(size_in_bytes)));
}

OxProgram OxContext::createProgram(std::string const& source, OxProgram::Source source_type, std::string const& program_name) const
{
    if (source_type == OxProgram::Source::file)
    {
        auto path_to_asset = getPathToAsset(source, m_asset_directories);
        if (!path_to_asset.isValid())
            THROW_OX_WRAPPER_ERROR("Unable to locate asset \"" + source + "\"");

        return OxProgramAttorney<OxContext>::createOptiXProgram(*this, path_to_asset, OxProgram::Source::file, program_name);
    }

    return OxProgramAttorney<OxContext>::createOptiXProgram(*this, source, source_type, program_name);
}

OxContext::operator bool() const
{
    return !hasErrors();
}

bool OxContext::hasErrors() const
{
    return m_error_state != RT_SUCCESS;
}

bool ox_wrapper::OxContext::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(m_optix_context, res = rtContextValidate(m_optix_context));
    return res == RT_SUCCESS;
}

std::string OxContext::retrieveStringAsset(std::string const& source) const
{
    auto path_to_asset = getPathToAsset(source, m_asset_directories);
    if (path_to_asset.isValid())
    {
        return util::misc::readAsciiTextFromSourceFile(path_to_asset);
    }
    else
        THROW_OX_WRAPPER_ERROR("Unable to retrieve asset \"" + source + "\"");
}

void OxContext::addAssetCustomLookUpDirectory(std::string const& directory)
{
    m_asset_directories.push_back(directory);
}

void OxContext::clearAssetCustomDirectoryLookUpCache()
{
    m_asset_directories.erase(m_asset_directories.begin() + m_default_asset_directories_cache_size, m_asset_directories.end());
}

