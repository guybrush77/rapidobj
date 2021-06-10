#include "compare-test/compare-test.hpp"

#include "serializer/serializer.hpp"

namespace rapidobj::test {

bool ParseTest::IsTestValid() const
{
    namespace fs = std::filesystem;

    if (!fs::exists(m_test_file) || !fs::is_regular_file(m_test_file)) {
        return false;
    }

    auto file = std::ifstream(m_test_file);

    if (file.bad()) {
        return false;
    }

    auto input = std::array<std::string, 5>();

    for (size_t i = 0; i != 5; ++i) {
        file >> input[i];
    }

    auto& [filename, filehash, separator, refname, refhash] = input;

    if (filename.empty() || refname.empty() || filehash.length() != 32 || refhash.length() != 32 ||
        separator != std::string(32, '-')) {
        return false;
    }

    auto filepath = m_test_file.parent_path() / filename;
    auto refpath  = m_test_file.parent_path() / refname;

    if (!fs::exists(filepath) || !fs::exists(refpath) || !fs::is_regular_file(filepath) ||
        !fs::is_regular_file(refpath)) {
        return false;
    }

    if (rapidobj::serializer::ComputeFileHash(filepath) != filehash ||
        rapidobj::serializer::ComputeFileHash(refpath) != refhash) {
        return false;
    }

    return true;
}

bool ParseTest::IsEqualToReference() const
{
    auto file = std::ifstream(m_test_file);

    auto input = std::array<std::string, 5>();

    for (size_t i = 0; i != 5; ++i) {
        file >> input[i];
    }

    auto& [filename, filehash, separator, refname, refhash] = input;

    auto filepath = m_test_file.parent_path() / filename;
    auto refpath  = m_test_file.parent_path() / refname;

    auto ref_result = rapidobj::serializer::Deserialize(refpath);

    auto result = rapidobj::ParseFile(filepath);

    if (m_triangulate == Triangulate::Yes) {
        rapidobj::Triangulate(result);
    }

    return result == ref_result;
}

} // namespace rapidobj::test
