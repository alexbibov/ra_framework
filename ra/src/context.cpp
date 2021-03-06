#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "context.h"
#include "texture2d.h"
#include "exception.h"
#include "ray_payloads.h"
#include "util/optional.h"
#include "util/misc.h"


using namespace ra;

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


RaContext::RaContext(std::vector<std::string> const& asset_directories, uint32_t num_entry_points/* = 1U*/):
    m_asset_directories{ asset_directories },
    m_default_asset_directories_cache_size{ m_asset_directories.size() },
    m_error_state{ RT_SUCCESS }
{
    RTresult optix_rc = rtContextCreate(&m_optix_context);

    if (optix_rc == RT_ERROR_NO_DEVICE)
    {
        THROW_RA_ERROR("OptiX is not supported by the GPU installed in the host system");
    }

    if (optix_rc == RT_ERROR_INVALID_VALUE)
    {
        THROW_RA_ERROR("OptiX context cannot be initialized: invalid value");
    } 

    LOG_OPTIX_ERROR(m_optix_context, rtContextSetEntryPointCount(m_optix_context, num_entry_points));
    LOG_OPTIX_ERROR(m_optix_context, rtContextSetRayTypeCount(m_optix_context, static_cast<unsigned int>(RaRayType::count)));
    LOG_OPTIX_ERROR(m_optix_context, rtContextSetPrintEnabled(m_optix_context, true));
    LOG_OPTIX_ERROR(m_optix_context, rtContextSetPrintBufferSize(m_optix_context, 1024));
}

RaContext::~RaContext()
{
    LOG_OPTIX_ERROR(m_optix_context, rtContextDestroy(m_optix_context));
}

RaProgram RaContext::createProgram(std::string const& source, RaProgram::Source source_type, std::string const& program_name) const
{
    if (source_type == RaProgram::Source::file)
    {
        auto path_to_asset = getPathToAsset(source, m_asset_directories);
        if (!path_to_asset.isValid())
            THROW_RA_ERROR("Unable to locate asset \"" + source + "\"");

        return RaProgramAttorney<RaContext>::createOptiXProgram(*this, path_to_asset, RaProgram::Source::file, program_name);
    }

    return RaProgramAttorney<RaContext>::createOptiXProgram(*this, source, source_type, program_name);
}

std::shared_ptr<RaTexture> RaContext::createTextureFromFile(std::string& filename, bool* is_hdr_texture, uint8_t* number_of_channels)
{
    bool is_hdr = stbi_is_hdr(filename.c_str()) != 0;
    if (is_hdr_texture)
    {
        *is_hdr_texture = is_hdr;
    }

    int width{}, height{}, num_channels{};
    

    std::shared_ptr<RaTexture> texture{};
    size_t data_chunk_size{};
    void* src_data{};
    if (!is_hdr)
    {
        src_data = stbi_load(filename.c_str(), &width, &height, &num_channels, 0);
        if (!src_data) return nullptr;

        switch (num_channels)
        {
        case 1:
            texture.reset(new RaTexture2D<unsigned char>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        case 2:
            texture.reset(new RaTexture2D<uchar2>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        case 3:
            texture.reset(new RaTexture2D<uchar3>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        case 4:
            texture.reset(new RaTexture2D<uchar4>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        default:
            __assume(false);
        }

        data_chunk_size = num_channels * width * height;
    }
    else
    {
        src_data = stbi_loadf(filename.c_str(), &width, &height, &num_channels, 0);
        if (!src_data) return nullptr;

        switch (num_channels)
        {
        case 1:
            texture.reset(new RaTexture2D<float>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        case 2:
            texture.reset(new RaTexture2D<float2>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        case 3:
            texture.reset(new RaTexture2D<float3>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        case 4:
            texture.reset(new RaTexture2D<float4>{ *this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
            break;

        default:
            __assume(false);
        }

        data_chunk_size = sizeof(float) * num_channels * width * height;
    }
    
    if (number_of_channels)
    {
        *number_of_channels = static_cast<uint8_t>(num_channels);
    }


    RaBufferMapSentry<void> mapping{ texture->buffer(), RaBufferMapKind::write };
    memcpy(mapping.address(), src_data, data_chunk_size);
}

RaContext::operator bool() const
{
    return !hasErrors();
}

bool RaContext::hasErrors() const
{
    return m_error_state != RT_SUCCESS;
}

bool ra::RaContext::isValid() const
{
    RTresult res;
    LOG_OPTIX_ERROR(m_optix_context, res = rtContextValidate(m_optix_context));
    return res == RT_SUCCESS;
}

std::string RaContext::retrieveStringAsset(std::string const& source) const
{
    auto path_to_asset = getPathToAsset(source, m_asset_directories);
    if (path_to_asset.isValid())
    {
        return util::misc::readAsciiTextFromSourceFile(path_to_asset);
    }
    else
        THROW_RA_ERROR("Unable to retrieve asset \"" + source + "\"");
}

void RaContext::addAssetCustomLookUpDirectory(std::string const& directory)
{
    m_asset_directories.push_back(directory);
}

void RaContext::clearAssetCustomDirectoryLookUpCache()
{
    m_asset_directories.erase(m_asset_directories.begin() + m_default_asset_directories_cache_size, m_asset_directories.end());
}

