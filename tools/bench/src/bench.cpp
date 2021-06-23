#ifdef _MSC_VER

#define BEGIN_DISABLE_WARNINGS \
    __pragma(warning(push)) \
    __pragma(warning(disable : 4244)) /* conversion from 'T1' to 'T2', possible loss of data */ \
    __pragma(warning(disable : 4701)) /* potentially uninitialized local variable used */

#define END_DISABLE_WARNINGS __pragma(warning(pop))

#elif defined(__clang__)

#define BEGIN_DISABLE_WARNINGS

#define END_DISABLE_WARNINGS

#elif defined(__GNUC__)

// clang-format off

#define BEGIN_DISABLE_WARNINGS \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wsign-conversion\"") \
    _Pragma("GCC diagnostic ignored \"-Wconversion\"") \
    _Pragma("GCC diagnostic ignored \"-Wsign-compare\"") \
    _Pragma("GCC diagnostic ignored \"-Wfloat-conversion\"") \
    _Pragma("GCC diagnostic ignored \"-Wunused-value\"") \
    _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")

// clang-format on

#define END_DISABLE_WARNINGS _Pragma("GCC diagnostic pop")

#endif

BEGIN_DISABLE_WARNINGS

#include "OBJ_Loader.h"
#include "cxxopts.hpp"

END_DISABLE_WARNINGS

#include "rapidobj/rapidobj.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

int Parse(int argc, char* argv[])
{
    using cxxopts::value;
    namespace fs = std::filesystem;

    auto options = cxxopts::Options(argv[0], "This tool is used to measure the time to load and parse an .obj file.");

    options.positional_help("input-file");

    options.add_options()("p,parser", "Which parser to use.", value<std::string>(), "obj|rapid|tiny");
    options.add_options()("h,help", "Show help.");
    options.add_options()("input-file", "", value<std::string>());

    options.parse_positional("input-file");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << '\n';
        return EXIT_SUCCESS;
    }

    if (0 == result.count("parser")) {
        std::cout << "Error: parser not specified\n";
        return EXIT_FAILURE;
    }

    const auto parser = result["parser"].as<std::string>();

    if (parser != "obj" && parser != "rapid" && parser != "tiny") {
        std::cout << "Error: parser not recognized (must be: obj, tiny, or rapid)\n";
        return EXIT_FAILURE;
    }

    if (0 == result.count("input-file")) {
        std::cout << "Error: input-file missing\n";
        return EXIT_FAILURE;
    }

    const auto input_file = fs::path(result["input-file"].as<std::string>());

    if (!fs::exists(input_file)) {
        std::cout << "File " << input_file << " does not exist.\n";
        return EXIT_FAILURE;
    }

    auto input_filepath = fs::canonical(input_file);

    if (!fs::is_regular_file(input_filepath)) {
        std::cout << "Path " << input_filepath << " is not a file.\n";
        return EXIT_FAILURE;
    }

    auto duration = std::chrono::milliseconds();

    if (parser == "obj") {
        auto t1 = std::chrono::system_clock::now();

        auto loader = objl::Loader();

        bool success = loader.LoadFile(input_file.string());

        if (!success) {
            std::cout << "Obj-Loader Error\n";
            return EXIT_FAILURE;
        }

        auto t2 = std::chrono::system_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    }

    if (parser == "rapid") {
        auto t1 = std::chrono::system_clock::now();

        auto rapid_result = rapidobj::ParseFile(input_filepath);

        if (rapid_result.error) {
            std::cout << rapid_result.error.code.message() << "\n";
            return EXIT_FAILURE;
        }

        rapidobj::Triangulate(rapid_result);

        if (rapid_result.error) {
            std::cout << rapid_result.error.code.message() << "\n";
            return EXIT_FAILURE;
        }

        auto t2 = std::chrono::system_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    }

    if (parser == "tiny") {
        auto tiny_reader = tinyobj::ObjReader();
        auto tiny_config = tinyobj::ObjReaderConfig();

        auto t1 = std::chrono::system_clock::now();

        bool success = tiny_reader.ParseFromFile(input_filepath.string(), tiny_config);

        if (!success) {
            std::cout << tiny_reader.Error() << "\n";
            return EXIT_FAILURE;
        }

        auto t2 = std::chrono::system_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    }

    std::cout << "Parse time [ms]:" << duration.count() << "\n";

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
