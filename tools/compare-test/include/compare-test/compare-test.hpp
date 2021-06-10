#include <filesystem>

namespace rapidobj::test {

enum class Triangulate { No, Yes };

class ParseTest final {
  public:
    ParseTest(std::filesystem::path test_file, Triangulate triangulate)
        : m_test_file(test_file), m_triangulate(triangulate)
    {}

    bool IsTestValid() const;

    bool IsEqualToReference() const;

  private:
    std::filesystem::path m_test_file;
    Triangulate           m_triangulate;
};

} // namespace rapidobj::test
