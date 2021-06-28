// clang-format off

#if defined(__clang__)

#define BEGIN_DISABLE_WARNINGS \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wsign-conversion\"") \
    _Pragma("clang diagnostic ignored \"-Wconversion\"") \
    _Pragma("clang diagnostic ignored \"-Wsign-compare\"") \
    _Pragma("clang diagnostic ignored \"-Wfloat-conversion\"") \
    _Pragma("clang diagnostic ignored \"-Wunused-value\"")

#define END_DISABLE_WARNINGS _Pragma("clang diagnostic pop")

#elif _MSC_VER

#define BEGIN_DISABLE_WARNINGS \
    __pragma(warning(push)) \
    __pragma(warning(disable : 4244)) /* conversion from 'T1' to 'T2', possible loss of data */ \
    __pragma(warning(disable : 4701)) /* potentially uninitialized local variable used */ \
    __pragma(warning(disable : 4996)) /* fopen: This function or variable may be unsafe */

#define END_DISABLE_WARNINGS __pragma(warning(pop))

#elif defined(__GNUC__)

#define BEGIN_DISABLE_WARNINGS \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wsign-conversion\"") \
    _Pragma("GCC diagnostic ignored \"-Wconversion\"") \
    _Pragma("GCC diagnostic ignored \"-Wsign-compare\"") \
    _Pragma("GCC diagnostic ignored \"-Wfloat-conversion\"") \
    _Pragma("GCC diagnostic ignored \"-Wunused-value\"") \
    _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")

#define END_DISABLE_WARNINGS _Pragma("GCC diagnostic pop")

#endif

// clang-format on

BEGIN_DISABLE_WARNINGS

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "cxxopts.hpp"

END_DISABLE_WARNINGS

#include "rapidobj/rapidobj.hpp"

int Parse(int argc, char* argv[])
{
    using cxxopts::value;
    namespace fs = std::filesystem;

    auto options = cxxopts::Options(argv[0], "This tool is used to measure the time to load and parse an .obj file.");

    options.positional_help("input-file");

    options.add_options()("p,parser", "Which parser to use.", value<std::string>(), "fast|rapid|tiny");
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

    if (parser != "fast" && parser != "rapid" && parser != "tiny") {
        std::cout << "Error: parser not recognized (must be: fast, rapid, or tiny)\n";
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

    if (parser == "fast") {
        auto t1 = std::chrono::system_clock::now();

        auto* fast_obj = fast_obj_read(input_filepath.string().c_str());

        if (!fast_obj) {
            std::cout << "fast_obj error\n";
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

        auto t2 = std::chrono::system_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    }

    if (parser == "tiny") {
        auto tiny_reader = tinyobj::ObjReader();
        auto tiny_config = tinyobj::ObjReaderConfig();

        tiny_config.triangulate = false;

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
