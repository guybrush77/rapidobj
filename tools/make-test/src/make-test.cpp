#ifdef _MSC_VER

// clang-format off

#define BEGIN_DISABLE_WARNINGS \
    __pragma(warning(push)) \
    __pragma(warning(disable:4244)) /* conversion from 'T1' to 'T2', possible loss of data */ \
    __pragma(warning(disable:4701)) /* potentially uninitialized local variable used */

#define END_DISABLE_WARNINGS __pragma(warning(pop))

#elif defined(__clang__)

#define BEGIN_DISABLE_WARNINGS

#define END_DISABLE_WARNINGS

#elif defined(__GNUC__)

#define BEGIN_DISABLE_WARNINGS \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wconversion\"")

// clang-format on

#define END_DISABLE_WARNINGS _Pragma("GCC diagnostic pop")

#endif

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

BEGIN_DISABLE_WARNINGS

#include "cxxopts.hpp"

END_DISABLE_WARNINGS

#include "serializer/serializer.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

std::ostream& operator<<(std::ostream& os, const rapidobj::Index& index)
{
    using std::to_string;

    const auto& [iposition, itex, inormal] = index;

    auto out = std::string("{ " + to_string(iposition) + ", " + to_string(itex) + ", " + to_string(inormal) + " }");

    os << out;

    return os;
}

std::ostream& operator<<(std::ostream& os, const tinyobj::index_t& index)
{
    using std::to_string;

    const auto& [ivertex, inormal, itex] = index;

    os << std::string("{ " + to_string(ivertex) + ", " + to_string(itex) + ", " + to_string(inormal) + " }");

    return os;
}

std::ostream& operator<<(std::ostream& os, const rapidobj::Float3& f3)
{
    using std::to_string;

    os << std::string("{ " + to_string(f3[0]) + ", " + to_string(f3[1]) + ", " + to_string(f3[2]) + " }");

    return os;
}

std::ostream& operator<<(std::ostream& os, const float (&f3)[3])
{
    using std::to_string;

    os << std::string("{ " + to_string(f3[0]) + ", " + to_string(f3[1]) + ", " + to_string(f3[2]) + " }");

    return os;
}

bool operator==(const rapidobj::Float3& lhs, const float (&rhs)[3])
{
    return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2];
}

bool operator!=(const rapidobj::Float3& lhs, const float (&rhs)[3])
{
    return !operator==(lhs, rhs);
}

bool operator==(const rapidobj::TextureOption& lhs, const tinyobj::texture_option_t& rhs)
{
    return static_cast<int>(lhs.type) == static_cast<int>(rhs.type) && lhs.sharpness == rhs.sharpness &&
           lhs.brightness == rhs.brightness && lhs.contrast == rhs.contrast && lhs.origin_offset == rhs.origin_offset &&
           lhs.scale == rhs.scale && lhs.turbulence == rhs.turbulence &&
           lhs.texture_resolution == rhs.texture_resolution && lhs.clamp == rhs.clamp && lhs.imfchan == rhs.imfchan &&
           lhs.blendu == rhs.blendu && lhs.blendv == rhs.blendv && lhs.bump_multiplier == rhs.bump_multiplier;
}

bool operator==(const rapidobj::Material& lhs, const tinyobj::material_t& rhs)
{
    return lhs.name == rhs.name && lhs.ambient == rhs.ambient && lhs.diffuse == rhs.diffuse &&
           lhs.specular == rhs.specular && lhs.transmittance == rhs.transmittance && lhs.emission == rhs.emission &&
           lhs.shininess == rhs.shininess && lhs.ior == rhs.ior && lhs.dissolve == rhs.dissolve &&
           lhs.illum == rhs.illum && lhs.ambient_texname == rhs.ambient_texname &&
           lhs.diffuse_texname == rhs.diffuse_texname && lhs.specular_texname == rhs.specular_texname &&
           lhs.specular_highlight_texname == rhs.specular_highlight_texname && lhs.bump_texname == rhs.bump_texname &&
           lhs.displacement_texname == rhs.displacement_texname && lhs.alpha_texname == rhs.alpha_texname &&
           lhs.reflection_texname == rhs.reflection_texname && lhs.ambient_texopt == rhs.ambient_texopt &&
           lhs.diffuse_texopt == rhs.diffuse_texopt && lhs.specular_texopt == rhs.specular_texopt &&
           lhs.specular_highlight_texopt == rhs.specular_highlight_texopt && lhs.bump_texopt == rhs.bump_texopt &&
           lhs.displacement_texopt == rhs.displacement_texopt && lhs.alpha_texopt == rhs.alpha_texopt &&
           lhs.reflection_texopt == rhs.reflection_texopt && lhs.roughness == rhs.roughness &&
           lhs.metallic == rhs.metallic && lhs.sheen == rhs.sheen &&
           lhs.clearcoat_thickness == rhs.clearcoat_thickness && lhs.clearcoat_roughness == rhs.clearcoat_roughness &&
           lhs.anisotropy == rhs.anisotropy && lhs.anisotropy_rotation == rhs.anisotropy_rotation &&
           lhs.roughness_texname == rhs.roughness_texname && lhs.metallic_texname == rhs.metallic_texname &&
           lhs.sheen_texname == rhs.sheen_texname && lhs.emissive_texname == rhs.emissive_texname &&
           lhs.normal_texname == rhs.normal_texname && lhs.roughness_texopt == rhs.roughness_texopt &&
           lhs.metallic_texopt == rhs.metallic_texopt && lhs.sheen_texopt == rhs.sheen_texopt &&
           lhs.emissive_texopt == rhs.emissive_texopt && lhs.normal_texopt == rhs.normal_texopt;
}

bool operator!=(const rapidobj::Material& lhs, const tinyobj::material_t& rhs)
{
    return !operator==(lhs, rhs);
}

bool operator==(const rapidobj::Index lhs, const tinyobj::index_t rhs)
{
    return lhs.position_index == rhs.vertex_index && lhs.texcoord_index == rhs.texcoord_index &&
           lhs.normal_index == rhs.normal_index;
}

bool operator!=(const rapidobj::Index lhs, const tinyobj::index_t rhs)
{
    return !operator==(lhs, rhs);
}

template <typename R, typename T, typename TA>
bool operator==(const rapidobj::Array<R>& lhs, const std::vector<T, TA>& rhs)
{
    auto cmp = [](const R& lhs, const T& rhs) { return lhs == rhs; };

    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin(), cmp);
}

bool operator==(const rapidobj::Mesh& lhs, const tinyobj::mesh_t& rhs)
{
    return lhs.indices == rhs.indices && lhs.num_face_vertices == rhs.num_face_vertices &&
           lhs.material_ids == rhs.material_ids && lhs.smoothing_group_ids == rhs.smoothing_group_ids;
}

bool operator==(const rapidobj::Lines& lhs, const tinyobj::lines_t& rhs)
{
    return lhs.indices == rhs.indices && lhs.num_line_vertices == rhs.num_line_vertices;
}

bool operator==(const rapidobj::Points& lhs, const tinyobj::points_t& rhs)
{
    return lhs.indices == rhs.indices;
}

bool operator==(const rapidobj::Shape& lhs, const tinyobj::shape_t& rhs)
{
    return lhs.name == rhs.name && lhs.mesh == rhs.mesh && lhs.lines == rhs.lines && lhs.points == rhs.points;
}

template <typename R, typename RA, typename T, typename TA>
bool operator==(const std::vector<R, RA>& lhs, const std::vector<T, TA>& rhs)
{
    return (lhs.size() == rhs.size()) &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin(), [](const R& lhs, const T& rhs) { return lhs == rhs; });
}

enum class Silent { False, True };

template <typename T, typename U>
void PrintDifferences(T lhs, size_t lhs_size, U rhs, size_t rhs_size)
{
    static constexpr auto kMaxDiffs = 5;

    using std::cout;
    using std::setw;

    if (lhs_size != rhs_size) {
        cout << "    Size: " << lhs_size << " (rapidobj) vs " << rhs_size << " (tinyobj)\n";
    }

    cout << "    {\n";

    auto ndiffs       = 0;
    auto size         = std::min(lhs_size, rhs_size);
    auto num_element  = std::string();
    bool print_header = true;

    for (size_t i = 0; i != size; ++i) {
        if (*lhs != *rhs) {
            if (print_header) {
                cout << setw(9) << "index";
                cout << setw(35) << "rapidobj";
                cout << setw(35) << "tinyobj";
                cout << "\n\n";
                print_header = false;
            }
            cout << setw(9) << i;
            cout << setw(35);
            if constexpr (std::is_convertible_v<decltype(*lhs), const char>) {
                cout << static_cast<int>(*lhs);
            } else {
                cout << *lhs;
            }
            cout << setw(35);
            if constexpr (std::is_constructible_v<decltype(*rhs), const char>) {
                cout << static_cast<int>(*rhs);
            } else {
                cout << *rhs;
            }
            cout << '\n';
            ++ndiffs;
            if (ndiffs >= kMaxDiffs) {
                cout << setw(9) << "...";
                cout << setw(35) << "...";
                cout << setw(35) << "...";
                cout << '\n';
                break;
            }
        }
        ++lhs, ++rhs;
    }
    cout << "    }\n";
}

template <typename T, typename U>
bool Compare(std::string_view intro, const T& lhs, const U& rhs, Silent silent)
{
    if (silent == Silent::False) {
        std::cout << intro;
    }

    if (lhs == rhs) {
        if (silent == Silent::False) {
            std::cout << "Equal\n";
        }
        return true;
    }

    if (silent == Silent::False) {
        std::cout << "Not Equal\n";

        PrintDifferences(lhs.begin(), lhs.size(), rhs.begin(), rhs.size());
    }

    return false;
}

bool CompareColors(
    std::string_view              intro,
    const rapidobj::Array<float>& lhs,
    const std::vector<float>&     rhs,
    Silent                        silent)
{
    if (silent == Silent::False) {
        std::cout << intro;
    }

    bool all_white = rhs.end() == std::find_if(rhs.begin(), rhs.end(), [](float f) { return f != 1.0f; });
    bool is_equal  = (all_white && lhs.empty()) || (lhs == rhs);

    if (is_equal) {
        if (silent == Silent::False) {
            std::cout << "Equal\n";
        }
        return true;
    }

    if (silent == Silent::False) {
        std::cout << "Not Equal\n";

        PrintDifferences(lhs.begin(), lhs.size(), rhs.begin(), rhs.size());
    }

    return false;
}

template <>
void PrintDifferences(
    std::vector<rapidobj::Shape>::const_iterator  lhs,
    size_t                                        lhs_size,
    std::vector<tinyobj::shape_t>::const_iterator rhs,
    size_t                                        rhs_size)
{
    std::cout << '\n';

    if (lhs_size != rhs_size) {
        std::cout << "Different number of shapes: " << lhs_size << " (rapidobj) vs " << rhs_size << " (tinyobj)\n\n";
    }

    auto num_shapes = std::min(lhs_size, rhs_size);

    for (size_t i = 0; i != num_shapes; ++i) {
        bool names_equal        = lhs->name == rhs->name;
        bool mesh_indices_equal = Compare("", lhs->mesh.indices, rhs->mesh.indices, Silent::True);
        bool mesh_num_face_vertices_equal =
            Compare("", lhs->mesh.num_face_vertices, rhs->mesh.num_face_vertices, Silent::True);
        bool mesh_material_ids_equal = Compare("", lhs->mesh.material_ids, rhs->mesh.material_ids, Silent::True);
        bool mesh_smoothing_group_ids_equal =
            Compare("", lhs->mesh.smoothing_group_ids, rhs->mesh.smoothing_group_ids, Silent::True);
        bool lines_indices_equal = Compare("", lhs->lines.indices, rhs->lines.indices, Silent::True);
        bool lines_num_line_vertices =
            Compare("", lhs->lines.num_line_vertices, rhs->lines.num_line_vertices, Silent::True);
        bool points_indices_equal = Compare("", lhs->points.indices, rhs->points.indices, Silent::True);

        auto mesh_equal = mesh_indices_equal && mesh_num_face_vertices_equal && mesh_material_ids_equal &&
                          mesh_smoothing_group_ids_equal;

        auto lines_equal = lines_indices_equal && lines_num_line_vertices;

        auto points_equal = points_indices_equal;

        if (names_equal && mesh_equal && lines_equal && points_equal) {
            ++lhs, ++rhs;
            continue;
        }

        std::cout << "Shape " << (i + 1) << '\n';

        if (names_equal) {
            std::cout << "  Name:                  Equal '" << lhs->name << "'\n";
        } else {
            std::cout << "  Name:                  Not Equal '" << lhs->name << "' (rapidobj) vs '" << rhs->name
                      << "' (tinyobj)\n";
        }

        if (mesh_equal) {
            std::cout << "  Mesh:                  Equal\n";
        } else {
            std::cout << "  Mesh:                  Not Equal\n";

            Compare("    Indices:             ", lhs->mesh.indices, rhs->mesh.indices, Silent::False);

            Compare(
                "    Num Face Vertices:   ",
                lhs->mesh.num_face_vertices,
                rhs->mesh.num_face_vertices,
                Silent::False);

            Compare("    Material Ids:        ", lhs->mesh.material_ids, rhs->mesh.material_ids, Silent::False);

            Compare(
                "    Smoothing Group Ids: ",
                lhs->mesh.smoothing_group_ids,
                rhs->mesh.smoothing_group_ids,
                Silent::False);
        }

        if (lines_equal) {
            std::cout << "  Lines:                 Equal\n";
        } else {
            std::cout << "  Lines:                 Not Equal\n\n";

            Compare("    Indices:             ", lhs->lines.indices, rhs->lines.indices, Silent::False);

            Compare(
                "    Num Line Vertices:   ",
                lhs->lines.num_line_vertices,
                rhs->lines.num_line_vertices,
                Silent::False);
        }

        if (points_equal) {
            std::cout << "  Points:                Equal\n";
        } else {
            std::cout << "  Points:                Not Equal\n\n";

            Compare("    Indices:             ", lhs->points.indices, rhs->points.indices, Silent::False);
        }

        ++lhs, ++rhs;

        std::cout << '\n';
    }
}

template <typename R, typename T>
void PrintField(const std::string& field, const R& lhs, const T& rhs)
{
    if (lhs != rhs) {
        std::cout << "    " << field << '\n';
        std::cout << std::setw(39) << lhs;
        std::cout << std::setw(39) << rhs;
        std::cout << '\n';
    }
}

void PrintField(const std::string& field, const rapidobj::TextureOption& lhs, const tinyobj::texture_option_t& rhs)
{
    PrintField(field + "type", static_cast<int>(lhs.type), static_cast<int>(rhs.type));
    PrintField(field + "sharpness", lhs.sharpness, rhs.sharpness);
    PrintField(field + "brightness", lhs.brightness, rhs.brightness);
    PrintField(field + "contrast", lhs.contrast, rhs.contrast);
    PrintField(field + "origin_offset", lhs.origin_offset, rhs.origin_offset);
    PrintField(field + "scale", lhs.scale, rhs.scale);
    PrintField(field + "turbulence", lhs.turbulence, rhs.turbulence);
    PrintField(field + "texture_resolution", lhs.texture_resolution, rhs.texture_resolution);
    PrintField(field + "clamp", lhs.clamp, rhs.clamp);
    PrintField(field + "imfchan", lhs.imfchan, rhs.imfchan);
    PrintField(field + "blendu", lhs.blendu, rhs.blendu);
    PrintField(field + "blendv", lhs.blendv, rhs.blendv);
    PrintField(field + "bump_multiplier", lhs.bump_multiplier, rhs.bump_multiplier);
}

template <>
void PrintDifferences(
    std::vector<rapidobj::Material>::const_iterator  lhs,
    size_t                                           lhs_size,
    std::vector<tinyobj::material_t>::const_iterator rhs,
    size_t                                           rhs_size)
{
    using std::cout;
    using std::setw;

    cout << '\n';

    if (lhs_size != rhs_size) {
        std::cout << "Different number of materials: " << lhs_size << " (rapidobj) vs " << rhs_size << " (tinyobj)\n\n";
    }

    auto num_materials = std::min(lhs_size, rhs_size);

    for (size_t i = 0; i != num_materials; ++i) {
        if (*lhs != *rhs) {
            cout << "  Material " << i << "\n";

            cout << setw(39) << "rapidobj";
            cout << setw(39) << "tinyobj";
            cout << "\n";

            cout << "    name\n";
            cout << setw(39) << lhs->name;
            cout << setw(39) << rhs->name;
            cout << '\n';

            PrintField("ambient", lhs->ambient, rhs->ambient);
            PrintField("diffuse", lhs->diffuse, rhs->diffuse);
            PrintField("specular", lhs->specular, rhs->specular);
            PrintField("transmittance", lhs->transmittance, rhs->transmittance);
            PrintField("emission", lhs->emission, rhs->emission);
            PrintField("shininess", lhs->shininess, rhs->shininess);
            PrintField("ior", lhs->ior, rhs->ior);
            PrintField("dissolve", lhs->dissolve, rhs->dissolve);
            PrintField("illum", lhs->illum, rhs->illum);
            PrintField("ambient_texname", lhs->ambient_texname, rhs->ambient_texname);
            PrintField("diffuse_texname", lhs->diffuse_texname, rhs->diffuse_texname);
            PrintField("specular_texname", lhs->specular_texname, rhs->specular_texname);
            PrintField("specular_highlight_texname", lhs->specular_highlight_texname, rhs->specular_highlight_texname);
            PrintField("bump_texname", lhs->bump_texname, rhs->bump_texname);
            PrintField("displacement_texname", lhs->displacement_texname, rhs->displacement_texname);
            PrintField("alpha_texname", lhs->alpha_texname, rhs->alpha_texname);
            PrintField("reflection_texname", lhs->reflection_texname, rhs->reflection_texname);
            PrintField("ambient_texopt.", lhs->ambient_texopt, rhs->ambient_texopt);
            PrintField("diffuse_texopt.", lhs->diffuse_texopt, rhs->diffuse_texopt);
            PrintField("specular_texopt.", lhs->specular_texopt, rhs->specular_texopt);
            PrintField("specular_highlight_texopt.", lhs->specular_highlight_texopt, rhs->specular_highlight_texopt);
            PrintField("bump_texopt.", lhs->bump_texopt, rhs->bump_texopt);
            PrintField("displacement_texopt.", lhs->displacement_texopt, rhs->displacement_texopt);
            PrintField("alpha_texopt.", lhs->alpha_texopt, rhs->alpha_texopt);
            PrintField("reflection_texopt.", lhs->reflection_texopt, rhs->reflection_texopt);
            PrintField("roughness", lhs->roughness, rhs->roughness);
            PrintField("metallic", lhs->metallic, rhs->metallic);
            PrintField("sheen", lhs->sheen, rhs->sheen);
            PrintField("clearcoat_thickness", lhs->clearcoat_thickness, rhs->clearcoat_thickness);
            PrintField("clearcoat_roughness", lhs->clearcoat_roughness, rhs->clearcoat_roughness);
            PrintField("anisotropy", lhs->anisotropy, rhs->anisotropy);
            PrintField("anisotropy_rotation", lhs->anisotropy_rotation, rhs->anisotropy_rotation);
            PrintField("roughness_texname", lhs->roughness_texname, rhs->roughness_texname);
            PrintField("metallic_texname", lhs->metallic_texname, rhs->metallic_texname);
            PrintField("sheen_texname", lhs->sheen_texname, rhs->sheen_texname);
            PrintField("emissive_texname", lhs->emissive_texname, rhs->emissive_texname);
            PrintField("normal_texname", lhs->normal_texname, rhs->normal_texname);
            PrintField("roughness_texopt.", lhs->roughness_texopt, rhs->roughness_texopt);
            PrintField("metallic_texopt.", lhs->metallic_texopt, rhs->metallic_texopt);
            PrintField("sheen_texopt.", lhs->sheen_texopt, rhs->sheen_texopt);
            PrintField("emissive_texopt.", lhs->emissive_texopt, rhs->emissive_texopt);
            PrintField("normal_texopt.", lhs->normal_texopt, rhs->normal_texopt);
        }

        ++lhs, ++rhs;
    }
}

void GenerateReferenceFile(const rapidobj::Result& result, const std::filesystem::path& filepath)
{
    rapidobj::serializer::Serialize(result, filepath);
}

void ReportError(const rapidobj::Error& error)
{
    std::cout << error.code.message() << "\n";
    if (!error.line.empty()) {
        std::cout << "On line " << error.line_num << ": \"" << error.line << "\"\n";
    }
}

int Parse(int argc, char* argv[])
{
    using cxxopts::value;
    namespace fs = std::filesystem;

    auto options = cxxopts::Options(argv[0], "This tool is used to generate test files.");

    options.positional_help("input-file");

    options.add_options()("o,out-dir", "Use to specify output folder for test files.", value<std::string>(), "path");
    options.add_options()("s,skip-xref", "Do not cross-reference parser results.");
    options.add_options()("x,only-xref", "Compare parser results but do not generate test files.");
    options.add_options()("t,triangulate", "Triangulate meshes after parsing.");
    options.add_options()("h,help", "Show help.");
    options.add_options()("input-file", "", value<std::string>());

    options.parse_positional("input-file");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << '\n';
        return EXIT_SUCCESS;
    }

    if (0 == result.count("input-file")) {
        std::cout << "Error: input-file missing\n";
        return EXIT_FAILURE;
    }

    const auto& out_dir     = result.count("out-dir") ? fs::path(result["out-dir"].as<std::string>()) : fs::path();
    const bool  skip_xref   = result.count("skip-xref");
    const bool  only_xref   = result.count("only-xref");
    const bool  triangulate = result.count("triangulate");
    const auto& input_file  = fs::path(result["input-file"].as<std::string>());

    if (!fs::exists(input_file)) {
        std::cout << "File " << input_file << " does not exist.\n";
        return EXIT_FAILURE;
    }

    auto input_filepath = fs::canonical(input_file);

    if (!fs::is_regular_file(input_filepath)) {
        std::cout << "Path " << input_filepath << " is not a file.\n";
        return EXIT_FAILURE;
    }

    auto out_dirpath = out_dir;

    if (out_dirpath.empty()) {
        out_dirpath = input_filepath.parent_path().empty() ? "." : input_filepath.parent_path();
    }

    out_dirpath = fs::canonical(out_dirpath);

    if (!fs::exists(out_dirpath)) {
        std::cout << "Directory " << out_dirpath << " does not exist.\n";
        return EXIT_FAILURE;
    }

    if (!fs::is_directory(out_dirpath)) {
        std::cout << "Path " << out_dirpath << " is not a directory.\n";
        return EXIT_FAILURE;
    }

    if (skip_xref && only_xref) {
        std::cout << "Options --skip-xref and --only-xref are mutually exclusive.\n";
        return EXIT_FAILURE;
    }

    if (!out_dir.empty() && only_xref) {
        std::cout << "Warning: --out-dir will have no effect because --only-xref is specified.\n";
    }

    if (input_filepath.extension() != ".obj") {
        std::cout << "Warning: input file has non-standard extension " << input_filepath.extension() << ".\n";
    }

    std::cout << "\n";

    std::cout << "Processing file:\n" << input_filepath << "\n\n";

    std::cout << "With rapidobj:\n";

    auto rapid_result = rapidobj::Result{};
    {
        auto t1 = std::chrono::system_clock::now();

        rapid_result = rapidobj::ParseFile(input_filepath);

        if (rapid_result.error.code) {
            ReportError(rapid_result.error);
            return EXIT_FAILURE;
        }

        auto t2 = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

        std::cout << "Parsed in: " << duration.count() << "ms\n";
    }

    if (triangulate) {
        auto t1 = std::chrono::system_clock::now();

        bool success = rapidobj::Triangulate(rapid_result);

        if (!success) {
            ReportError(rapid_result.error);
            return EXIT_FAILURE;
        }

        auto t2 = std::chrono::system_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

        std::cout << "Triangulated in: " << duration.count() << "ms\n";
    }

    std::cout << "\n";

    if (skip_xref) {
        std::cout << "Skipping parse results comparison.\n";
    } else {
        std::cout << "With tinyobj:\n";

        auto tiny_reader = tinyobj::ObjReader();
        auto tiny_config = tinyobj::ObjReaderConfig();

        tiny_config.triangulate = triangulate;

        {
            auto t1 = std::chrono::system_clock::now();

            bool success = tiny_reader.ParseFromFile(input_filepath.string(), tiny_config);

            if (!success) {
                std::cout << tiny_reader.Error() << "\n";
                return EXIT_FAILURE;
            }

            auto t2 = std::chrono::system_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            if (triangulate) {
                std::cout << "Parsed and triangulated in: " << duration.count() << "ms\n";
            } else {
                std::cout << "Parsed in: " << duration.count() << "ms\n";
            }
        }

        std::cout << "\n";

        std::cout.imbue(std::locale(""));

        const auto& tiny_vertices  = tiny_reader.GetAttrib().vertices;
        const auto& tiny_texcoords = tiny_reader.GetAttrib().texcoords;
        const auto& tiny_normals   = tiny_reader.GetAttrib().normals;
        const auto& tiny_colors    = tiny_reader.GetAttrib().colors;
        const auto& tiny_shapes    = tiny_reader.GetShapes();
        const auto& tiny_materials = tiny_reader.GetMaterials();

        bool positions_equal =
            Compare("Position attributes:    ", rapid_result.attributes.positions, tiny_vertices, Silent::False);
        bool texcoords_equal =
            Compare("Texcoord attributes:    ", rapid_result.attributes.texcoords, tiny_texcoords, Silent::False);
        bool normals_equal =
            Compare("Normal attributes:      ", rapid_result.attributes.normals, tiny_normals, Silent::False);
        bool colors_equal =
            CompareColors("Color attributes:       ", rapid_result.attributes.colors, tiny_colors, Silent::False);
        bool mesh_shapes_all_equal =
            Compare("Shapes:                 ", rapid_result.shapes, tiny_shapes, Silent::False);
        bool materials_all_equal =
            Compare("Materials:              ", rapid_result.materials, tiny_materials, Silent::False);

        std::cout << "\n";

        bool all_equal = positions_equal && texcoords_equal && normals_equal && colors_equal && mesh_shapes_all_equal &&
                         materials_all_equal;

        if (!all_equal) {
            if (only_xref) {
                std::cout << "Parse results did not compare equal." << '\n';
            } else {
                std::cout << "Parse results did not compare equal. No test files have been generated." << '\n';
            }
            return EXIT_FAILURE;
        }

        std::cout << "Parse results compared equal.\n";
    }

    if (!only_xref) {
        auto input_filename = input_filepath.filename().string();

        auto ref_filename = input_filename + (triangulate ? ".tri.ref" : ".ref");
        auto ref_filepath = out_dirpath / ref_filename;

        auto testfile = std::ostringstream();

        auto obj_filehash = rapidobj::serializer::ComputeFileHash(input_filepath);

        std::cout << "\n";

        std::cout << "Generating reference file:\n" << ref_filepath << '\n';

        GenerateReferenceFile(rapid_result, ref_filepath);

        auto ref_filehash = rapidobj::serializer::ComputeFileHash(ref_filepath);

        auto test_filename = input_filename + (triangulate ? ".tri.test" : ".test");
        auto test_filepath = out_dirpath / test_filename;

        std::cout << "\n";

        std::cout << "Generating test file:\n" << test_filepath << '\n';

        auto ofs = std::ofstream(test_filepath);

        auto separator = std::string(32, '-');

        ofs << input_filename << '\n';
        ofs << obj_filehash << '\n';
        ofs << separator << '\n';
        ofs << ref_filename << '\n';
        ofs << ref_filehash << '\n';

        std::cout << "\n";
        std::cout << "Done.\n";
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    auto rc = EXIT_SUCCESS;

    try {
        rc = Parse(argc, argv);
    } catch (const cxxopts::OptionException& e) {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return rc;
}