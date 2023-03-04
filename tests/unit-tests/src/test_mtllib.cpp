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

static const std::string objpath_mtllib_missing = QUOTE(TEST_DATA_DIR) "/mtllib/cube_mtllib_missing.obj";

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

namespace fs = std::filesystem;

static bool IDsOkay(const Array<int32_t>& material_ids)
{
    if (6 == material_ids.size() && 0 == material_ids[0] && 0 == material_ids[1] && 1 == material_ids[2] &&
        1 == material_ids[3] && 2 == material_ids[4] && 2 == material_ids[5]) {
        return true;
    }
    return false;
}

TEST_CASE("rapidobj::ParseFile(MaterialLibrary)")
{
    // ParseFile, implicit MaterialLibrary::Default
    {
        auto result = ParseFile(objpath);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, explicit MaterialLibrary::Default
    {
        auto mtllib = MaterialLibrary::Default();

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, explicit MaterialLibrary::Default, loading optional
    {
        auto mtllib = MaterialLibrary::Default(Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, explicit MaterialLibrary::Default, loading mandatory
    {
        auto mtllib = MaterialLibrary::Default(Load::Mandatory);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, implicit MaterialLibrary::Default, loading mandatory, missing mtllib
    {
        auto result = ParseFile(objpath_mtllib_missing);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseFile, explicit MaterialLibrary::Default, loading mandatory, missing mtllib
    {
        auto mtllib = MaterialLibrary::Default();

        auto result = ParseFile(objpath_mtllib_missing, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseFile, explicit MaterialLibrary::Default, loading optional, missing mtllib
    {
        auto mtllib = MaterialLibrary::Default(Load::Optional);

        auto result = ParseFile(objpath_mtllib_missing, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, explicit MaterialLibrary::Default, loading mandatory, missing mtllib
    {
        auto mtllib = MaterialLibrary::Default(Load::Mandatory);

        auto result = ParseFile(objpath_mtllib_missing, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseFile, MaterialLibrary::SearchPath, loading mandatory
    {
        auto mtllib = MaterialLibrary::SearchPath("red");

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kRed == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPath, loading optional
    {
        auto mtllib = MaterialLibrary::SearchPath("red", Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kRed == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPath, loading mandatory, missing file
    {
        auto mtllib = MaterialLibrary::SearchPath("bad/path");

        auto result = ParseFile(objpath, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseFile, MaterialLibrary::SearchPath, loading optional, missing file
    {
        auto mtllib = MaterialLibrary::SearchPath("bad/path", Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPath, loading mandatory, material file override
    {
        auto mtllib = MaterialLibrary::SearchPath("yellow.mtl");

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPath, loading optional, material file override
    {
        auto mtllib = MaterialLibrary::SearchPath("yellow.mtl", Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPaths, loading mandatory
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "blue", "green" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kBlue == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPaths, loading optional
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "blue", "green" }, Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kBlue == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPaths, no paths
    {
        auto mtllib = MaterialLibrary::SearchPaths({}, Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(rapidobj_errc::InvalidArgumentsError == result.error.code);
    }

    // ParseFile, MaterialLibrary::SearchPaths, loading mandatory, mix of good and bad paths
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad/path", "bad.mtl", ".", "red" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPaths, loading optional, mix of good and bad paths
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad/path", "bad.mtl", "green", "." }, Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kGreen == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPaths, loading optional, mix of good and bad paths, material file override
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad.mtl", "yellow.mtl", "red" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::SearchPaths, loading mandatory, bad paths
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad.mtl", "bad/path/1", "bad/path/2" });

        auto result = ParseFile(objpath, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
        CHECK(result.shapes.empty());
    }

    // ParseFile, MaterialLibrary::SearchPaths, loading optional, bad paths
    {
        auto mtllib = MaterialLibrary::SearchPaths({ "bad.mtl", "bad/path/1", "bad/path/2" }, Load::Optional);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::String
    {
        auto mtllib = MaterialLibrary::String(purple_materials);

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kPurple == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseFile, MaterialLibrary::String, empty string
    {
        auto mtllib = MaterialLibrary::String("");

        auto result = ParseFile(objpath, mtllib);

        CHECK(rapidobj_errc::MaterialNotFoundError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseFile, MaterialLibrary::Ignore
    {
        auto mtllib = MaterialLibrary::Ignore();

        auto result = ParseFile(objpath, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());
        CHECK(result.shapes.front().mesh.material_ids.empty());
    }
}

TEST_CASE("rapidobj::ParseStream(MaterialLibrary)")
{
    // ParseStream, implicit MaterialLibrary::Default
    {
        auto stream = std::ifstream(objpath);

        auto result = ParseStream(stream);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, explicit MaterialLibrary::Default
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::Default();

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, implicit MaterialLibrary::Default, missing mtllib
    {
        auto stream = std::ifstream(objpath_mtllib_missing);

        auto result = ParseStream(stream);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, explicit MaterialLibrary::Default, loading optional
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::Default(Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::InvalidArgumentsError == result.error.code);
    }

    // ParseStream, explicit MaterialLibrary::Default, loading mandatory
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::Default(Load::Mandatory);

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::InvalidArgumentsError == result.error.code);
    }

    // ParseStream, MaterialLibrary::SearchPath, loading mandatory
    {
        auto stream = std::ifstream(objpath);

        auto path   = fs::path(objpath).parent_path() / "red";
        auto mtllib = MaterialLibrary::SearchPath(path);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kRed == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPath, loading optional
    {
        auto stream = std::ifstream(objpath);

        auto path   = fs::path(objpath).parent_path() / "red";
        auto mtllib = MaterialLibrary::SearchPath(path, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kRed == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPath, loading mandatory, missing file
    {
        auto stream = std::ifstream(objpath);

        auto path   = fs::path(objpath).parent_path() / "bad/path";
        auto mtllib = MaterialLibrary::SearchPath(path);

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
        CHECK(result.shapes.empty());
    }

    // ParseStream, MaterialLibrary::SearchPath, loading optional, missing file
    {
        auto stream = std::ifstream(objpath);

        auto path   = fs::path(objpath).parent_path() / "bad/path";
        auto mtllib = MaterialLibrary::SearchPath(path, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPath, loading mandatory, relative file path
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::SearchPath("red");

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::MaterialRelativePathError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseStream, MaterialLibrary::SearchPath, loading mandatory, material file override
    {
        auto stream = std::ifstream(objpath);

        auto path   = fs::path(objpath).parent_path() / "yellow.mtl";
        auto mtllib = MaterialLibrary::SearchPath(path);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPath, loading optional, material file override
    {
        auto stream = std::ifstream(objpath);

        auto path   = fs::path(objpath).parent_path() / "yellow.mtl";
        auto mtllib = MaterialLibrary::SearchPath(path, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading mandatory
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "blue";
        auto path_2 = fs::path(objpath).parent_path() / "green";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2 });

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kBlue == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading optional
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "blue";
        auto path_2 = fs::path(objpath).parent_path() / "green";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2 }, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kBlue == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading mandatory, mix of good and bad paths
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "bad/path";
        auto path_2 = fs::path(objpath).parent_path() / "cube.mtl";
        auto path_3 = fs::path(objpath).parent_path() / "red";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2, path_3 });

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kWhite == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading optional, mix of good and bad paths
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "bad/path";
        auto path_2 = fs::path(objpath).parent_path() / "green";
        auto path_3 = fs::path(objpath).parent_path();
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2, path_3 }, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kGreen == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading optional, mix of good and bad paths, material file override
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "bad.mtl";
        auto path_2 = fs::path(objpath).parent_path() / "yellow.mtl";
        auto path_3 = fs::path(objpath).parent_path() / "red";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2, path_3 });

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kYellow == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading mandatory, bad paths
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "bad.mtl";
        auto path_2 = fs::path(objpath).parent_path() / "bad/path";
        auto path_3 = fs::path(objpath).parent_path() / "another/bad/path";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2, path_3 });

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::MaterialFileError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading optional, bad paths
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "bad.mtl";
        auto path_2 = fs::path(objpath).parent_path() / "bad/path";
        auto path_3 = fs::path(objpath).parent_path() / "another/bad/path";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2, path_3 }, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading manadatory, relative path
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "blue";
        auto path_2 = "relative/path";
        auto path_3 = fs::path(objpath).parent_path() / "green";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2, path_3 });

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::MaterialRelativePathError == result.error.code);
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading optional, relative path
    {
        auto stream = std::ifstream(objpath);

        auto path_1 = fs::path(objpath).parent_path() / "blue";
        auto path_2 = "relative/path";
        auto path_3 = fs::path(objpath).parent_path() / "green";
        auto mtllib = MaterialLibrary::SearchPaths({ path_1, path_2, path_3 }, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::MaterialRelativePathError == result.error.code);
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading manadatory, no paths
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::SearchPaths({});

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::InvalidArgumentsError == result.error.code);
    }

    // ParseStream, MaterialLibrary::SearchPaths, loading optional, no paths
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::SearchPaths({}, Load::Optional);

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::InvalidArgumentsError == result.error.code);
    }

    // ParseStream, MaterialLibrary::String
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::String(purple_materials);

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(3 == result.materials.size());

        CHECK("foo" == result.materials.front().name);
        CHECK(kPurple == result.materials.front().diffuse);

        CHECK(IDsOkay(result.shapes.front().mesh.material_ids));
    }

    // ParseStream, MaterialLibrary::String, empty string
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::String("");

        auto result = ParseStream(stream, mtllib);

        CHECK(rapidobj_errc::MaterialNotFoundError == result.error.code);

        CHECK(result.materials.empty());
    }

    // ParseStream, MaterialLibrary::Ignore
    {
        auto stream = std::ifstream(objpath);

        auto mtllib = MaterialLibrary::Ignore();

        auto result = ParseStream(stream, mtllib);

        CHECK(kSuccess == result.error.code);

        CHECK(result.materials.empty());
        CHECK(result.shapes.front().mesh.material_ids.empty());
    }
}
