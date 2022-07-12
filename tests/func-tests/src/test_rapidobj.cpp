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

TEST_CASE("color")
{
    auto test = MakeTest("color/color.obj.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("color-triangulated")
{
    auto test = MakeTestTriangulated("color/color.obj.tri.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("cube")
{
    auto test = MakeTest("cube/cube.obj.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("cube-triangulated")
{
    auto test = MakeTestTriangulated("cube/cube.obj.tri.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("mario")
{
    auto test = MakeTest("mario/mario.obj.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("mario-triangulated")
{
    auto test = MakeTestTriangulated("mario/mario.obj.tri.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("primitives")
{
    auto test = MakeTest("primitives/primitives.obj.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("primitives-triangulated")
{
    auto test = MakeTestTriangulated("primitives/primitives.obj.tri.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("sponza")
{
    auto test = MakeTest("sponza/sponza.obj.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
}

TEST_CASE("sponza-triangulated")
{
    auto test = MakeTestTriangulated("sponza/sponza.obj.tri.test");

    CHECK(test.IsTestValid());

    CHECK(test.IsEqualToReference());
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
