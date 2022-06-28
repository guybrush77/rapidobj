// clang-format off

#if defined(__clang__)

#define BEGIN_DISABLE_WARNINGS \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"") \

#define END_DISABLE_WARNINGS _Pragma("clang diagnostic pop")

#elif _MSC_VER

#define BEGIN_DISABLE_WARNINGS

#define END_DISABLE_WARNINGS

#elif defined(__GNUC__)

#define BEGIN_DISABLE_WARNINGS

#define END_DISABLE_WARNINGS

#endif

// clang-format on

#include "serializer/serializer.hpp"

#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

BEGIN_DISABLE_WARNINGS

#define XXH_STATIC_LINKING_ONLY
#define XXH_IMPLEMENTATION
#include "xxhash.h"

END_DISABLE_WARNINGS

static std::string ToBase16(XXH128_hash_t hash)
{
    std::stringstream ss;

    ss.width(16);
    ss.fill('0');

    ss << std::uppercase << std::hex << hash.high64 << hash.low64;

    return ss.str();
}

namespace rapidobj {

//
// Helper functions used to serialize rapidobj::Result
//
template <typename Archive, typename T>
void save(Archive& archive, const Array<T>& arr)
{
    archive(static_cast<uint64_t>(arr.size()), cereal::binary_data(arr.data(), sizeof(T) * arr.size()));
}

template <typename Archive, typename T>
void load(Archive& archive, Array<T>& arr)
{
    auto size = uint64_t{};
    archive(size);
    auto temp = Array<T>(size);
    archive(cereal::binary_data(temp.data(), sizeof(T) * temp.size()));
    arr = std::move(temp);
}

template <typename Archive>
void serialize(Archive& archive, Index& index)
{
    archive(index.position_index, index.texcoord_index, index.normal_index);
}

template <typename Archive>
void serialize(Archive& archive, Mesh& mesh)
{
    archive(mesh.indices, mesh.num_face_vertices, mesh.material_ids, mesh.smoothing_group_ids);
}

template <typename Archive>
void serialize(Archive& archive, Shape& shape)
{
    archive(shape.name, shape.mesh);
}

template <typename Archive>
void serialize(Archive& archive, Attributes& attributes)
{
    archive(attributes.positions, attributes.texcoords, attributes.normals);
}

template <typename Archive>
void serialize(Archive& archive, TextureOption& texture_option)
{
    archive(
        texture_option.type,
        texture_option.sharpness,
        texture_option.brightness,
        texture_option.contrast,
        texture_option.origin_offset,
        texture_option.scale,
        texture_option.turbulence,
        texture_option.texture_resolution,
        texture_option.clamp,
        texture_option.imfchan,
        texture_option.blendu,
        texture_option.blendv,
        texture_option.bump_multiplier);
}

template <typename Archive>
void serialize(Archive& archive, Material& material)
{
    archive(
        material.name,
        material.ambient,
        material.diffuse,
        material.specular,
        material.transmittance,
        material.emission,
        material.shininess,
        material.ior,
        material.dissolve,
        material.illum,
        material.ambient_texname,
        material.diffuse_texname,
        material.specular_texname,
        material.specular_highlight_texname,
        material.bump_texname,
        material.displacement_texname,
        material.alpha_texname,
        material.reflection_texname,
        material.ambient_texopt,
        material.diffuse_texopt,
        material.specular_texopt,
        material.specular_highlight_texopt,
        material.bump_texopt,
        material.displacement_texopt,
        material.alpha_texopt,
        material.reflection_texopt,
        material.roughness,
        material.metallic,
        material.sheen,
        material.clearcoat_thickness,
        material.clearcoat_roughness,
        material.anisotropy,
        material.anisotropy_rotation,
        material.roughness_texname,
        material.metallic_texname,
        material.sheen_texname,
        material.emissive_texname,
        material.normal_texname,
        material.roughness_texopt,
        material.metallic_texopt,
        material.sheen_texopt,
        material.emissive_texopt,
        material.normal_texopt);
}

template <typename Archive>
void serialize(Archive& archive, Error& error)
{
    archive(error.code.value(), error.line, error.line_num);
}

template <typename Archive>
void serialize(Archive& archive, Result& result)
{
    archive(result.attributes, result.shapes, result.materials, result.error);
}

namespace serializer {

std::string ComputeFileHash(std::filesystem::path filepath)
{
    auto oss = std::ostringstream();
    auto ifs = std::ifstream(filepath, std::ios::binary);

    if (ifs.bad()) {
        throw std::runtime_error("Failed to open file.");
    }

    oss << ifs.rdbuf();

    auto data = oss.str();

    return ToBase16(XXH3_128bits(data.data(), data.size()));
}

void Serialize(const rapidobj::Result& result, const std::filesystem::path& filepath)
{
    auto ofs     = std::ofstream(filepath, std::ios::binary);
    auto archive = cereal::PortableBinaryOutputArchive(ofs);

    archive(result);
}

Result Deserialize(const std::filesystem::path& filepath)
{
    auto ifs     = std::ifstream(filepath, std::ios::binary);
    auto archive = cereal::PortableBinaryInputArchive(ifs);
    auto result  = Result{};

    archive(result);

    return result;
}

} // namespace serializer

} // namespace rapidobj
