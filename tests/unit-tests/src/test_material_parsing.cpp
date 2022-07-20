#include <doctest/doctest.h>
#include <rapidobj/rapidobj.hpp>

using namespace rapidobj;
using namespace rapidobj::detail;

static constexpr auto test_material = R"(

newmtl foo 

Ka  0  1  2 
Kd  3  4  5 
Ks  6  7  8 
Kt  9 10 11 
Ke 12 13 14 

Ns 2.0 
Ni 3.0 
d  4.0 

illum 42 

map_Ka    -texres 1 ambient.jpg 
map_Kd    -texres 2 diffuse.jpg 
map_Ks    -texres 3 specular.jpg 
map_Ns    -texres 4 highlight.jpg 
map_bump  -texres 5 bump.bmp 
disp      -texres 6 displacement.png 
map_d     -texres 7 alpha.png 
refl      -texres 8 refl.jpg 

)";

static constexpr auto pbr_material = R"(

newmtl bar 

Pr  1.0
Pm  2.0
Ps  3.0
Pc  4.0
Pcr 5.0

aniso  6.0
anisor 7.0

map_Pr roughness.jpg 
map_Pm metallic.jpg 
map_Ps sheen.jpg 
map_Ke emissive.hdr 
norm   normal.png 

)";

static constexpr auto bad_material_1 = R"(

newmtl bad 

Pr  1.0
$%!*
Ps  3.0

)";

static constexpr auto bad_material_2 = R"(

newmtl bad 

Pr  1.0
Ps  3.0
illum 3 foo

)";

TEST_CASE("rapidobj::detail::ParseTextureOption")
{
    // option -blendu
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendu on", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.blendu == true);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendu off", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.blendu == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendu", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendu foo", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendu offten", &texture_option);

        CHECK(success == false);
    }

    // option -blendv
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendv on", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.blendv == true);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendv off", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.blendv == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendv", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendv onn", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendv foo", &texture_option);

        CHECK(success == false);
    }

    // option -boost
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-boost 1.75", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.sharpness == 1.75f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-boost", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-boost on", &texture_option);

        CHECK(success == false);
    }

    // option -mm
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-mm 2.5 4.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.brightness == 2.5f);
        CHECK(texture_option.contrast == 4.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-mm", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-mm 2.5", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-mm 2.5 4.0?", &texture_option);

        CHECK(success == false);
    }

    // option -o
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-o 5.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.origin_offset[0] == 5.0f);
        CHECK(texture_option.origin_offset[1] == 0.0f);
        CHECK(texture_option.origin_offset[2] == 0.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-o 5.0 6.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.origin_offset[0] == 5.0f);
        CHECK(texture_option.origin_offset[1] == 6.0f);
        CHECK(texture_option.origin_offset[2] == 0.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-o 5.0 6.0 7.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.origin_offset[0] == 5.0f);
        CHECK(texture_option.origin_offset[1] == 6.0f);
        CHECK(texture_option.origin_offset[2] == 7.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-o", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-o ?", &texture_option);

        CHECK(success == false);
    }

    // option -s
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-s 5.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.scale[0] == 5.0f);
        CHECK(texture_option.scale[1] == 1.0f);
        CHECK(texture_option.scale[2] == 1.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-s 5.0 6.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.scale[0] == 5.0f);
        CHECK(texture_option.scale[1] == 6.0f);
        CHECK(texture_option.scale[2] == 1.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-s 5.0 6.0 7.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.scale[0] == 5.0f);
        CHECK(texture_option.scale[1] == 6.0f);
        CHECK(texture_option.scale[2] == 7.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-s", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-s 2.g?", &texture_option);

        CHECK(success == false);
    }

    // option -t
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-t 5.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.turbulence[0] == 5.0f);
        CHECK(texture_option.turbulence[1] == 0.0f);
        CHECK(texture_option.turbulence[2] == 0.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-t 5.0 6.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.turbulence[0] == 5.0f);
        CHECK(texture_option.turbulence[1] == 6.0f);
        CHECK(texture_option.turbulence[2] == 0.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-t 5.0 6.0 7.0", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.turbulence[0] == 5.0f);
        CHECK(texture_option.turbulence[1] == 6.0f);
        CHECK(texture_option.turbulence[2] == 7.0f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-t", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-t 12345-", &texture_option);

        CHECK(success == false);
    }

    // option -texres
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-texres 256", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.texture_resolution == 256);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-texres", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-texres true", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-texres 1.0", &texture_option);

        CHECK(success == false);
    }

    // option -clamp
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-clamp off", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.clamp == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-clamp", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-clamp foo", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-clamp offten", &texture_option);

        CHECK(success == false);
    }

    // option -bm
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-bm 0.5", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.bump_multiplier == 0.5f);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-bm", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-bm on", &texture_option);

        CHECK(success == false);
    }

    // option -imfchan
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan r", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.imfchan == 'r');
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan g", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.imfchan == 'g');
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan b", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.imfchan == 'b');
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan m", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.imfchan == 'm');
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan l", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.imfchan == 'l');
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan z", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.imfchan == 'z');
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan a", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan 1", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-imfchan -", &texture_option);

        CHECK(success == false);
    }

    // option -type
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type sphere", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.type == TextureType::Sphere);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type cube_top", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.type == TextureType::CubeTop);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type cube_bottom", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.type == TextureType::CubeBottom);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type cube_front", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.type == TextureType::CubeFront);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type cube_back", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.type == TextureType::CubeBack);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type cube_left", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.type == TextureType::CubeLeft);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type cube_right", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.type == TextureType::CubeRight);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-type cube", &texture_option);

        CHECK(success == false);
    }

    // two options
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-blendu off -blendv off", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.blendu == false);
        CHECK(texture_option.blendv == false);
    }

    // three options
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-boost 2.0 -clamp off -imfchan z", &texture_option);

        CHECK(success == true);
        CHECK(texture_option.sharpness == 2.0f);
        CHECK(texture_option.clamp == false);
        CHECK(texture_option.imfchan == 'z');
    }

    // no options
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("", &texture_option);

        CHECK(success == true);
        CHECK(line == "");
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("foo.jpg", &texture_option);

        CHECK(success == true);
        CHECK(line == "foo.jpg");
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("\t  foo.jpg \t", &texture_option);

        CHECK(success == true);
        CHECK(line == "foo.jpg");
    }

    // bad options
    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-", &texture_option);

        CHECK(success == false);
    }

    {
        auto texture_option  = TextureOption{};
        auto [line, success] = ParseTextureOption("-foo on", &texture_option);

        CHECK(success == false);
    }
}

TEST_CASE("rapidobj::detail::ParseMaterials")
{
    {
        auto [map, materials, error] = ParseMaterials(test_material);

        CHECK(error.code == std::error_code());

        auto& material = materials.front();

        CHECK(material.name == "foo");

        CHECK(material.ambient[0] == 0.0f);
        CHECK(material.ambient[1] == 1.0f);
        CHECK(material.ambient[2] == 2.0f);

        CHECK(material.diffuse[0] == 3.0f);
        CHECK(material.diffuse[1] == 4.0f);
        CHECK(material.diffuse[2] == 5.0f);

        CHECK(material.specular[0] == 6.0f);
        CHECK(material.specular[1] == 7.0f);
        CHECK(material.specular[2] == 8.0f);

        CHECK(material.transmittance[0] == 9.0f);
        CHECK(material.transmittance[1] == 10.0f);
        CHECK(material.transmittance[2] == 11.0f);

        CHECK(material.emission[0] == 12.0f);
        CHECK(material.emission[1] == 13.0f);
        CHECK(material.emission[2] == 14.0f);

        CHECK(material.shininess == 2.0f);
        CHECK(material.ior == 3.0f);
        CHECK(material.dissolve == 4.0f);
        CHECK(material.illum == 42);

        CHECK(material.ambient_texname == "ambient.jpg");
        CHECK(material.ambient_texopt.texture_resolution == 1);
        CHECK(material.ambient_texopt.imfchan == 'm');

        CHECK(material.diffuse_texname == "diffuse.jpg");
        CHECK(material.diffuse_texopt.texture_resolution == 2);
        CHECK(material.diffuse_texopt.imfchan == 'm');

        CHECK(material.specular_texname == "specular.jpg");
        CHECK(material.specular_texopt.texture_resolution == 3);
        CHECK(material.specular_texopt.imfchan == 'm');

        CHECK(material.specular_highlight_texname == "highlight.jpg");
        CHECK(material.specular_highlight_texopt.texture_resolution == 4);
        CHECK(material.specular_highlight_texopt.imfchan == 'm');

        CHECK(material.bump_texname == "bump.bmp");
        CHECK(material.bump_texopt.texture_resolution == 5);
        CHECK(material.bump_texopt.imfchan == 'l');

        CHECK(material.displacement_texname == "displacement.png");
        CHECK(material.displacement_texopt.texture_resolution == 6);
        CHECK(material.displacement_texopt.imfchan == 'm');

        CHECK(material.alpha_texname == "alpha.png");
        CHECK(material.alpha_texopt.texture_resolution == 7);
        CHECK(material.alpha_texopt.imfchan == 'm');

        CHECK(material.reflection_texname == "refl.jpg");
        CHECK(material.reflection_texopt.texture_resolution == 8);
        CHECK(material.reflection_texopt.imfchan == 'm');
    }

    {
        auto [map, materials, error] = ParseMaterials(pbr_material);

        CHECK(error.code == std::error_code());

        auto& material = materials.front();

        CHECK(material.name == "bar");

        CHECK(material.roughness == 1.0f);
        CHECK(material.metallic == 2.0f);
        CHECK(material.sheen == 3.0f);
        CHECK(material.clearcoat_thickness == 4.0f);
        CHECK(material.clearcoat_roughness == 5.0f);
        CHECK(material.anisotropy == 6.0f);
        CHECK(material.anisotropy_rotation == 7.0f);

        CHECK(material.roughness_texname == "roughness.jpg");
        CHECK(material.roughness_texopt.imfchan == 'm');

        CHECK(material.metallic_texname == "metallic.jpg");
        CHECK(material.metallic_texopt.imfchan == 'm');

        CHECK(material.sheen_texname == "sheen.jpg");
        CHECK(material.sheen_texopt.imfchan == 'm');

        CHECK(material.emissive_texname == "emissive.hdr");
        CHECK(material.emissive_texopt.imfchan == 'm');

        CHECK(material.normal_texname == "normal.png");
        CHECK(material.normal_texopt.imfchan == 'm');
    }

    {
        auto [map, materials, error] = ParseMaterials(bad_material_1);

        CHECK(error.code == rapidobj_errc::MaterialParseError);
        CHECK(error.line == "$%!*");
        CHECK(error.line_num == 6);
    }

    {
        auto [map, materials, error] = ParseMaterials(bad_material_2);

        CHECK(error.code == rapidobj_errc::MaterialParseError);
        CHECK(error.line == "illum 3 foo");
        CHECK(error.line_num == 7);
    }
}
