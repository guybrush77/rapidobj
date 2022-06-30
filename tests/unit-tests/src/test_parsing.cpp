#include <array>
#include <doctest/doctest.h>
#include <rapidobj/rapidobj.hpp>

bool operator==(const rapidobj::Index& lhs, const rapidobj::Index& rhs) noexcept
{
    return lhs.position_index == rhs.position_index && lhs.texcoord_index == rhs.texcoord_index &&
           lhs.normal_index == rhs.normal_index;
}

bool operator==(rapidobj::detail::OffsetFlags lhs, const rapidobj::detail::ApplyOffset& rhs) noexcept
{
    return lhs == static_cast<rapidobj::detail::OffsetFlags>(rhs);
}

using namespace rapidobj;
using namespace rapidobj::detail;

static const auto kAllowPosition           = static_cast<OffsetFlags>(ApplyOffset::Position);
static const auto kAllowPositionAndTexture = static_cast<OffsetFlags>(ApplyOffset::Position | ApplyOffset::Texcoord);
static const auto kAllowAll                = static_cast<OffsetFlags>(ApplyOffset::All);

TEST_CASE("rapidobj::detail::ParseReals")
{
    auto floats = std::array<float, 3>{};

    {
        auto count = ParseReals("1.0", 3, floats.data());

        CHECK(count == 1);
        CHECK(floats[0] == 1.0f);
    }

    {
        auto count = ParseReals(" 1.0", 3, floats.data());

        CHECK(count == 1);
        CHECK(floats[0] == 1.0f);
    }

    {
        auto count = ParseReals("1.0 ", 3, floats.data());

        CHECK(count == 1);
        CHECK(floats[0] == 1.0f);
    }

    {
        auto count = ParseReals(" 1.0 ", 3, floats.data());

        CHECK(count == 1);
        CHECK(floats[0] == 1.0f);
    }

    {
        auto count = ParseReals("1.0 2.0", 3, floats.data());

        CHECK(count == 2);
        CHECK(floats[0] == 1.0f);
        CHECK(floats[1] == 2.0f);
    }

    {
        auto count = ParseReals("1.0 2.0 3.0", 3, floats.data());

        CHECK(count == 3);
        CHECK(floats[0] == 1.0f);
        CHECK(floats[1] == 2.0f);
        CHECK(floats[2] == 3.0f);
    }

    {
        auto count = ParseReals("1.0 2.0 3.0 4.0", 3, floats.data());

        CHECK(count == 0);
    }

    {
        auto count = ParseReals("", 3, floats.data());

        CHECK(count == 0);
    }

    {
        auto count = ParseReals("  ", 3, floats.data());

        CHECK(count == 0);
    }

    {
        auto count = ParseReals("?", 3, floats.data());

        CHECK(count == 0);
    }

    {
        auto count = ParseReals("?5.0", 3, floats.data());

        CHECK(count == 0);
    }

    {
        auto count = ParseReals("5.0?", 3, floats.data());

        CHECK(count == 0);
    }

    {
        auto count = ParseReals("5.?0", 3, floats.data());

        CHECK(count == 0);
    }

    {
        auto count = ParseReals("5.0 ?", 3, floats.data());

        CHECK(count == 0);
    }
}

TEST_CASE("rapidobj::detail::ParseFace")
{
    auto indices = Buffer<Index>();
    auto flags   = Buffer<OffsetFlags>();

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1 2 3", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, -1, -1 });
        CHECK(indices.data()[1] == Index{ 1, -1, -1 });
        CHECK(indices.data()[2] == Index{ 2, -1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1 2 3 4", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 4);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, -1, -1 });
        CHECK(indices.data()[1] == Index{ 1, -1, -1 });
        CHECK(indices.data()[2] == Index{ 2, -1, -1 });
        CHECK(indices.data()[3] == Index{ 3, -1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
        CHECK(flags.data()[3] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace(
            "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 "
            "39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 "
            "74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 "
            "107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 "
            "133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 "
            "159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 "
            "185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 "
            "211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 "
            "237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255",
            0,
            0,
            0,
            3,
            255,
            kAllowAll,
            &indices,
            &flags);

        CHECK(count == 255);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, -1, -1 });
        CHECK(indices.data()[254] == Index{ 254, -1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[254] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-42 -43 -44", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -42, -1, -1 });
        CHECK(indices.data()[1] == Index{ -43, -1, -1 });
        CHECK(indices.data()[2] == Index{ -44, -1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::Position);
        CHECK(flags.data()[1] == ApplyOffset::Position);
        CHECK(flags.data()[2] == ApplyOffset::Position);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-42 -43 -44", 42, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, -1, -1 });
        CHECK(indices.data()[1] == Index{ -1, -1, -1 });
        CHECK(indices.data()[2] == Index{ -2, -1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::Position);
        CHECK(flags.data()[1] == ApplyOffset::Position);
        CHECK(flags.data()[2] == ApplyOffset::Position);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace(" 42   43\t44  ", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 41, -1, -1 });
        CHECK(indices.data()[1] == Index{ 42, -1, -1 });
        CHECK(indices.data()[2] == Index{ 43, -1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1\t1\t1", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, -1, -1 });
        CHECK(indices.data()[1] == Index{ 0, -1, -1 });
        CHECK(indices.data()[2] == Index{ 0, -1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("42/43 44/45 46/47", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 41, 42, -1 });
        CHECK(indices.data()[1] == Index{ 43, 44, -1 });
        CHECK(indices.data()[2] == Index{ 45, 46, -1 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("42/43 44/45 46/47 48/49", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 4);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 41, 42, -1 });
        CHECK(indices.data()[1] == Index{ 43, 44, -1 });
        CHECK(indices.data()[2] == Index{ 45, 46, -1 });
        CHECK(indices.data()[3] == Index{ 47, 48, -1 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
        CHECK(flags.data()[3] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-42/42 1/1 2/2", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -42, 41, -1 });
        CHECK(indices.data()[1] == Index{ 0, 0, -1 });
        CHECK(indices.data()[2] == Index{ 1, 1, -1 });

        CHECK(flags.data()[0] == ApplyOffset::Position);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("42/-42 1/-1 2/-2", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 41, -42, -1 });
        CHECK(indices.data()[1] == Index{ 0, -1, -1 });
        CHECK(indices.data()[2] == Index{ 1, -2, -1 });

        CHECK(flags.data()[0] == ApplyOffset::Texcoord);
        CHECK(flags.data()[1] == ApplyOffset::Texcoord);
        CHECK(flags.data()[2] == ApplyOffset::Texcoord);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-42/-42 -43/-43 -44/-44", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -42, -42, -1 });
        CHECK(indices.data()[1] == Index{ -43, -43, -1 });
        CHECK(indices.data()[2] == Index{ -44, -44, -1 });

        CHECK(flags.data()[0] == (ApplyOffset::Position | ApplyOffset::Texcoord));
        CHECK(flags.data()[1] == (ApplyOffset::Position | ApplyOffset::Texcoord));
        CHECK(flags.data()[2] == (ApplyOffset::Position | ApplyOffset::Texcoord));
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/2/3 4/5/6 7/8/9", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, 1, 2 });
        CHECK(indices.data()[1] == Index{ 3, 4, 5 });
        CHECK(indices.data()[2] == Index{ 6, 7, 8 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/2/3 4/5/6 7/8/9 10/11/12", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 4);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, 1, 2 });
        CHECK(indices.data()[1] == Index{ 3, 4, 5 });
        CHECK(indices.data()[2] == Index{ 6, 7, 8 });
        CHECK(indices.data()[3] == Index{ 9, 10, 11 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
        CHECK(flags.data()[3] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/2/-1 3/4/-2 5/6/-3", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, 1, -1 });
        CHECK(indices.data()[1] == Index{ 2, 3, -2 });
        CHECK(indices.data()[2] == Index{ 4, 5, -3 });

        CHECK(flags.data()[0] == ApplyOffset::Normal);
        CHECK(flags.data()[1] == ApplyOffset::Normal);
        CHECK(flags.data()[2] == ApplyOffset::Normal);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/-2/3 4/-5/6 7/-8/9", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 0, -2, 2 });
        CHECK(indices.data()[1] == Index{ 3, -5, 5 });
        CHECK(indices.data()[2] == Index{ 6, -8, 8 });

        CHECK(flags.data()[0] == ApplyOffset::Texcoord);
        CHECK(flags.data()[1] == ApplyOffset::Texcoord);
        CHECK(flags.data()[2] == ApplyOffset::Texcoord);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("2/-3/-4 5/-6/-7 8/-9/-10", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 1, -3, -4 });
        CHECK(indices.data()[1] == Index{ 4, -6, -7 });
        CHECK(indices.data()[2] == Index{ 7, -9, -10 });

        CHECK(flags.data()[0] == (ApplyOffset::Texcoord | ApplyOffset::Normal));
        CHECK(flags.data()[1] == (ApplyOffset::Texcoord | ApplyOffset::Normal));
        CHECK(flags.data()[2] == (ApplyOffset::Texcoord | ApplyOffset::Normal));
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("\t-2/3/4\t-5/6/7\t-8/9/9\t", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -2, 2, 3 });
        CHECK(indices.data()[1] == Index{ -5, 5, 6 });
        CHECK(indices.data()[2] == Index{ -8, 8, 8 });

        CHECK(flags.data()[0] == ApplyOffset::Position);
        CHECK(flags.data()[1] == ApplyOffset::Position);
        CHECK(flags.data()[2] == ApplyOffset::Position);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-2/3/-4   -5/6/-7   -8/9/-9", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -2, 2, -4 });
        CHECK(indices.data()[1] == Index{ -5, 5, -7 });
        CHECK(indices.data()[2] == Index{ -8, 8, -9 });

        CHECK(flags.data()[0] == (ApplyOffset::Position | ApplyOffset::Normal));
        CHECK(flags.data()[1] == (ApplyOffset::Position | ApplyOffset::Normal));
        CHECK(flags.data()[2] == (ApplyOffset::Position | ApplyOffset::Normal));
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-2/-3/4 -5/-6/7 -8/-9/10", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -2, -3, 3 });
        CHECK(indices.data()[1] == Index{ -5, -6, 6 });
        CHECK(indices.data()[2] == Index{ -8, -9, 9 });

        CHECK(flags.data()[0] == (ApplyOffset::Position | ApplyOffset::Texcoord));
        CHECK(flags.data()[1] == (ApplyOffset::Position | ApplyOffset::Texcoord));
        CHECK(flags.data()[2] == (ApplyOffset::Position | ApplyOffset::Texcoord));
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-2/-3/-4 -5/-6/-7 -8/-9/-1", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -2, -3, -4 });
        CHECK(indices.data()[1] == Index{ -5, -6, -7 });
        CHECK(indices.data()[2] == Index{ -8, -9, -1 });

        CHECK(flags.data()[0] == (ApplyOffset::Position | ApplyOffset::Texcoord | ApplyOffset::Normal));
        CHECK(flags.data()[1] == (ApplyOffset::Position | ApplyOffset::Texcoord | ApplyOffset::Normal));
        CHECK(flags.data()[2] == (ApplyOffset::Position | ApplyOffset::Texcoord | ApplyOffset::Normal));
    }

    SUBCASE("")
    {
        auto [count, ec] =
            ParseFace("-12/-13/-14 -15/-16/-17 -18/-19/-11", 10, 10, 10, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -2, -3, -4 });
        CHECK(indices.data()[1] == Index{ -5, -6, -7 });
        CHECK(indices.data()[2] == Index{ -8, -9, -1 });

        CHECK(flags.data()[0] == (ApplyOffset::Position | ApplyOffset::Texcoord | ApplyOffset::Normal));
        CHECK(flags.data()[1] == (ApplyOffset::Position | ApplyOffset::Texcoord | ApplyOffset::Normal));
        CHECK(flags.data()[2] == (ApplyOffset::Position | ApplyOffset::Texcoord | ApplyOffset::Normal));
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("2//3 4//5 6//7", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 1, -1, 2 });
        CHECK(indices.data()[1] == Index{ 3, -1, 4 });
        CHECK(indices.data()[2] == Index{ 5, -1, 6 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("2//3 4//5 6//7 8//9", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 4);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 1, -1, 2 });
        CHECK(indices.data()[1] == Index{ 3, -1, 4 });
        CHECK(indices.data()[2] == Index{ 5, -1, 6 });
        CHECK(indices.data()[3] == Index{ 7, -1, 8 });

        CHECK(flags.data()[0] == ApplyOffset::None);
        CHECK(flags.data()[1] == ApplyOffset::None);
        CHECK(flags.data()[2] == ApplyOffset::None);
        CHECK(flags.data()[3] == ApplyOffset::None);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("2//-3 4//-5 6//-7", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ 1, -1, -3 });
        CHECK(indices.data()[1] == Index{ 3, -1, -5 });
        CHECK(indices.data()[2] == Index{ 5, -1, -7 });

        CHECK(flags.data()[0] == ApplyOffset::Normal);
        CHECK(flags.data()[1] == ApplyOffset::Normal);
        CHECK(flags.data()[2] == ApplyOffset::Normal);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-2//3 -4//5 -6//7", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -2, -1, 2 });
        CHECK(indices.data()[1] == Index{ -4, -1, 4 });
        CHECK(indices.data()[2] == Index{ -6, -1, 6 });

        CHECK(flags.data()[0] == ApplyOffset::Position);
        CHECK(flags.data()[1] == ApplyOffset::Position);
        CHECK(flags.data()[2] == ApplyOffset::Position);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("-2//-3 -4//-5 -6//-7", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 3);
        CHECK(ec == rapidobj_errc());

        CHECK(indices.data()[0] == Index{ -2, -1, -3 });
        CHECK(indices.data()[1] == Index{ -4, -1, -5 });
        CHECK(indices.data()[2] == Index{ -6, -1, -7 });

        CHECK(flags.data()[0] == (ApplyOffset::Position | ApplyOffset::Normal));
        CHECK(flags.data()[1] == (ApplyOffset::Position | ApplyOffset::Normal));
        CHECK(flags.data()[2] == (ApplyOffset::Position | ApplyOffset::Normal));
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::TooFewIndicesError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace(" ", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::TooFewIndicesError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::TooFewIndicesError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1 2", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::TooFewIndicesError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1 2 3 0", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::IndexOutOfBoundsError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace(
            "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 "
            "39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 "
            "74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 101 102 103 104 105 106 "
            "107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125 126 127 128 129 130 131 132 "
            "133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 149 150 151 152 153 154 155 156 157 158 "
            "159 160 161 162 163 164 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 183 184 "
            "185 186 187 188 189 190 191 192 193 194 195 196 197 198 199 200 201 202 203 204 205 206 207 208 209 210 "
            "211 212 213 214 215 216 217 218 219 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 "
            "237 238 239 240 241 242 243 244 245 246 247 248 249 250 251 252 253 254 255 256",
            0,
            0,
            0,
            3,
            255,
            kAllowAll,
            &indices,
            &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::TooManyIndicesError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("/", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace(" / ", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("//", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("/42", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("42/", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("42//", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("42/?/52", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("42///52", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("?", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("? 41", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("41?", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("4?1", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("41 ?", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1 2 3?", 0, 0, 0, 3, 255, kAllowAll, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1 2", 0, 0, 0, 2, 255, kAllowPosition, &indices, &flags);

        CHECK(count == 2);
        CHECK(ec == rapidobj_errc());
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/1 2/2", 0, 0, 0, 2, 255, kAllowPosition, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/1/1 2/2/2", 0, 0, 0, 2, 255, kAllowPosition, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1//1 2//2", 0, 0, 0, 2, 255, kAllowPosition, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1 2", 0, 0, 0, 2, 255, kAllowPositionAndTexture, &indices, &flags);

        CHECK(count == 2);
        CHECK(ec == rapidobj_errc());
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/1 2/2", 0, 0, 0, 2, 255, kAllowPositionAndTexture, &indices, &flags);

        CHECK(count == 2);
        CHECK(ec == rapidobj_errc());
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1/1/1 2/2/2", 0, 0, 0, 2, 255, kAllowPositionAndTexture, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }

    SUBCASE("")
    {
        auto [count, ec] = ParseFace("1//1 2//2", 0, 0, 0, 2, 255, kAllowPositionAndTexture, &indices, &flags);

        CHECK(count == 0);
        CHECK(ec == rapidobj_errc::ParseError);
    }
}
