#include "compare-test/compare-test.hpp"

#include "serializer/serializer.hpp"

#include <optional>

namespace rapidobj::test {

struct Files {
    std::filesystem::path input;
    std::filesystem::path reference;
};

std::optional<Files> GetFiles(const std::filesystem::path& test_file)
{
    namespace fs = std::filesystem;

    if (!fs::exists(test_file) || !fs::is_regular_file(test_file)) {
        return std::nullopt;
    }

    auto file = std::ifstream(test_file);

    if (file.bad()) {
        return std::nullopt;
    }

    auto input = std::array<std::string, 5>();

    for (size_t i = 0; i != 5; ++i) {
        file >> input[i];
    }

    auto& [filename, filehash, separator, refname, refhash] = input;

    if (filename.empty() || refname.empty() || filehash.length() != 32 || refhash.length() != 32 ||
        separator != std::string(32, '-')) {
        return std::nullopt;
    }

    auto filepath = test_file.parent_path() / filename;
    auto refpath  = test_file.parent_path() / refname;

    if (!fs::exists(filepath) || !fs::exists(refpath) || !fs::is_regular_file(filepath) ||
        !fs::is_regular_file(refpath)) {
        return std::nullopt;
    }

    if (rapidobj::serializer::ComputeFileHash(filepath) != filehash ||
        rapidobj::serializer::ComputeFileHash(refpath) != refhash) {
        return std::nullopt;
    }

    return Files{ filepath, refpath };
}

ParseTest::ParseTest(std::filesystem::path test_file)
{
    if (auto files = GetFiles(test_file)) {
        m_input_file = files->input;
        m_reference  = rapidobj::serializer::Deserialize(files->reference);
    }
}

bool ParseTest::IsTestValid() const
{
    return !m_input_file.empty();
}

bool ParseTest::ParseFile(Triangulate triangulate) const
{
    auto result = rapidobj::ParseFile(m_input_file);

    if (triangulate == Triangulate::Yes) {
        rapidobj::Triangulate(result);
    }

    return result == m_reference;
}

bool ParseTest::ParseStream(Triangulate triangulate) const
{
    auto stream = std::ifstream(m_input_file);

    auto mtllib = MaterialLibrary::SearchPath(m_input_file.parent_path());

    auto result = rapidobj::ParseStream(stream, mtllib);

    if (triangulate == Triangulate::Yes) {
        rapidobj::Triangulate(result);
    }

    return result == m_reference;
}

} // namespace rapidobj::test
