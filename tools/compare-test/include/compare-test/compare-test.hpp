#include "rapidobj/rapidobj.hpp"

namespace rapidobj::test {

enum class Triangulate { No, Yes };

class ParseTest final {
  public:
    ParseTest(std::filesystem::path test_file);

    bool IsTestValid() const;

    bool ParseFile(Triangulate) const;

    bool ParseStream(Triangulate) const;

  private:
    std::filesystem::path m_input_file;
    rapidobj::Result      m_reference;
};

} // namespace rapidobj::test
