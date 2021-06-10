#include "doctest/doctest.h"

#include "compare-test/compare-test.hpp"

#ifndef TEST_DATA_DIR
#error Cannot find test files; TEST_DATA_DIR is not defined
#endif

#define Q(x)     #x
#define QUOTE(x) Q(x)

static auto MakeTest(const std::string& path)
{
    static const std::string basepath = QUOTE(TEST_DATA_DIR);

    return rapidobj::test::ParseTest(basepath + "/" + path, rapidobj::test::Triangulate::No);
}

static auto MakeTestTriangulated(const std::string& path)
{
    static const std::string basepath = QUOTE(TEST_DATA_DIR);

    return rapidobj::test::ParseTest(basepath + "/" + path, rapidobj::test::Triangulate::Yes);
}

TEST_CASE("teapot")
{
    auto test = MakeTest("teapot/teapot.obj.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("teapot-triangulated")
{
    auto test = MakeTestTriangulated("teapot/teapot.obj.tri.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}
