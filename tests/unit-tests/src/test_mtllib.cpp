#include <doctest/doctest.h>
#include <rapidobj/rapidobj.hpp>

#include <iostream>

using namespace rapidobj;

#ifndef TEST_DATA_DIR
#error Cannot find test files; TEST_DATA_DIR is not defined
#endif

#define Q(x)     #x
#define QUOTE(x) Q(x)

static const std::string objpath = QUOTE(TEST_DATA_DIR) "/mtllib/cube.obj";

static constexpr auto purple_materials = R"(

newmtl foo
Kd 1 0 1

newmtl bar
Kd 1 0 1

newmtl baz
Kd 1 0 1

)";

static const auto kSuccess = std::error_code();

static const auto kWhite  = Float3{ 1, 1, 1 };
static const auto kYellow = Float3{ 1, 1, 0 };
static const auto kPurple = Float3{ 1, 0, 1 };
static const auto kRed    = Float3{ 1, 0, 0 };
static const auto kGreen  = Float3{ 0, 1, 0 };
static const auto kBlue   = Float3{ 0, 0, 1 };

static bool IDsOkay(const Array<int32_t>& material_ids)
{
    if (6 == material_ids.size() && 0 == material_ids[0] && 0 == material_ids[1] && 1 == material_ids[2] &&
        1 == material_ids[3] && 2 == material_ids[4] && 2 == material_ids[5]) {
        return true;
    }
    return false;
}

TEST_CASE("rapidobj::MaterialLibrary")
{
    // Default MaterialLibrary implicit
    {
        auto result = ParseFile(objpath);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Default MaterialLibrary explicit
    {
        auto mtllib = MaterialLibrary::Default();

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Default MaterialLibrary optional load
    {
        auto mtllib = MaterialLibrary::Default(Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Search path MaterialLibrary
    {
        auto mtllib = MaterialLibrary::SearchPath("red");

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kRed == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Search path MaterialLibrary optional
    {
        auto mtllib = MaterialLibrary::SearchPath("red", Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kRed == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Search path MaterialLibrary bad path
    {
        auto mtllib = MaterialLibrary::SearchPath("bad/path");

        auto result = ParseFile(objpath, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
        CHECK(result.shapes.empty());
    }

    // Search path MaterialLibrary bad path, optional loading
    {
        auto mtllib = MaterialLibrary::SearchPath("bad/path", Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Search path MaterialLibrary material file override
    {
        auto mtllib = MaterialLibrary::SearchPath("yellow.mtl");

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Search path MaterialLibrary material file override, optional loading
    {
        auto mtllib = MaterialLibrary::SearchPath("yellow.mtl", Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Multiple search paths MaterialLibrary
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "blue", "green" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kBlue == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Multiple search paths MaterialLibrary optional loading
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "blue", "green" }, Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kBlue == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Multiple search paths MaterialLibrary, mix of good and bad paths
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad/path", "bad.mtl", ".", "red" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Multiple search paths MaterialLibrary, mix of good and bad paths, optional loading
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad/path", "bad.mtl", "green", "." }, Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kGreen == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Multiple search paths MaterialLibrary, material file override
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad.mtl", "yellow.mtl", "red" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // Multiple search paths MaterialLibrary, bad paths
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad.mtl", "bad/path/1", "bad/path/2" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
        CHECK(result.shapes.empty());
    }

    // Multiple search paths MaterialLibrary, bad paths, optional loading
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad.mtl", "bad/path/1", "bad/path/2" }, Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // String MaterialLibrary
    {
        auto mtllib = MaterialLibrary::String(purple_materials);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kPurple == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // String MaterialLibrary, empty string
    {
        auto mtllib = MaterialLibrary::String("");

        auto result = ParseFile(objpath, mtllib);

        CHECK(rapidobj_errc::MaterialNotFoundError == result.error.code);

        CHECK(result.materials.empty());
    }

    // Ignore MaterialLibrary
    {
        auto mtllib = MaterialLibrary::Ignore();

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());
        CHECK(result.shapes.front().mesh.material_ids.empty());
    }
}
