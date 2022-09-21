# ![rapidobj](data/images/docs/logo-light.svg#gh-light-mode-only)![rapidobj](data/images/docs/logo-dark.svg#gh-dark-mode-only)

[![Standard](https://img.shields.io/badge/standard-C%2B%2B17-blue)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
![Platform](https://img.shields.io/badge/platform-windows%20%7C%20macos%20%7C%20linux-blue)
![Build Status](https://github.com/guybrush77/rapidobj/actions/workflows/build.yml/badge.svg?event=push)

- [About](#about)
- [Integration](#integration)
  - [Prerequisites](#prerequisites)
  - [Manual Integration](#manual-integration)
  - [CMake Integration](#cmake-integration)
- [API](#api)
  - [ParseFile](#parsefile)
  - [MaterialLibrary](#materiallibrary)
  - [Load Policy](#load-policy)
  - [Triangulate](#triangulate)
- [Data Layout](#data-layout)
  - [Result](#result)
  - [Attributes](#attributes)
  - [Shape](#shape)
  - [Mesh](#mesh)
  - [Lines](#lines)
  - [Points](#points)
  - [Materials](#materials)
- [Example](#example)
- [Next Steps](#next-steps)
- [OS Support](#os-support)
- [Third Party Tools and Resources](#third-party-tools-and-resources)
- [License](#license)

## About

rapidobj is an easy-to-use, single-header C++17 library that loads and parses [Wavefront .obj files](https://en.wikipedia.org/wiki/Wavefront_.obj_file).

The .obj file format was first used by Wavefront Technologies around 1990. However, this 3D geometry file format did not age well. An .obj file is a text file and, consequently, large models take a lot of of disk space and are slow to load and parse. Moreover, after loading and parsing, additional processing steps are required to transform the data into a format suitable for hardware (i.e. GPU) rendering. Nevertheless, .obj files are common enough that it's useful to have an efficient way to parse them.

rapidobj's API was influenced by another single header C++ library, [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader). From users' point of view, the two libraries look fairly similar. That said, tinyobjloader has been around for some time; it is a mature and well tested library. So, why use rapidobj library? It is fast, and especially so when parsing large files. It was designed to take full advantage of modern computer hardware. See [Benchmarks](docs/BENCHMARKS.md) page.

## Integration

### Prerequisites

You will need a C++ compiler that fully supports C++17 standard. In practice, this means:

- GCC 8 or higher
- MSVC 19.14 or higher
- Clang 7 or higher

If you intend to use CMake as your build system, you will need to install CMake version 3.20 or higher.

### Manual Integration

The simplest way to integrate the library in your project is to copy the header file, ```rapidobj.hpp```, to a location that is in your compiler's include path. To use the library from your application, include the header file:

```cpp
#include "rapidobj.hpp"
```

To compile your project, make sure to use the C++17 switch (```-std=c++17``` for g++ and clang, ```/std:c++17``` for MSVC).

There are some extra considerations when building a Linux project: you need to link your application against _libpthread_ library. For example, assuming g++ compiler:

```bash
g++ -std=c++17 my_src.cpp -pthread -o my_app
```

> :page_facing_up: If you are using gcc version 8, you also have to link against the _stdc++fs_ library (_std::filesystem_ used by rapidobj is not part of _libstdc++_ until gcc version 9).

### CMake Integration

#### External

This section explains how to use rapidobj external to your project. If using the command line, perform cmake configuration and generation steps from inside the rapidobj folder:

```bash
cmake -B build .
```

The next step is to actually install the rapidobj package:

```bash
cd build
sudo make install
```

The install command will copy the files to well defined system directories. Note that this command will likely require administrative access.

The only remaining step is to find the rapidobj package from inside your own CMakeLists.txt file and link against it. For example:

```cmake
add_executable(my_app my_src.cpp)

find_package(RapidObj REQUIRED)

target_link_libraries(my_app PRIVATE rapidobj::rapidobj)
```

rapidobj cmake script places the header file in a ```rapidobj``` subfolder of the include directory. Consequently, the include directive in your code should look like this:

```cpp
#include "rapidobj/rapidobj.hpp"
```

What if you don't want to install rapidobj in a system directory? rapidobj allows you to specify custom install folders. CMake cache variable RAPIDOBJ_INCLUDE_DIR is used to set header file install location; RAPIDOBJ_CMAKE_DIR is used to set cmake files install location. For example, to install rapidobj in a folder ```local``` inside your home directory, the cmake configuration and generation steps are as follows:

```bash
cmake -B build -DRAPIDOBJ_INCLUDE_DIR=${HOME}/local/include -DRAPIDOBJ_CMAKE_DIR=${HOME}/local/cmake .
```

The install step is almost the same as before:

```bash
cd build
make install
```

The only difference is that administrative access (i.e. sudo) is no longer required since the destination is users' home folder, as opposed to system folders.

Because the files have been installed to a custom location that CMake does not know about, CMake cannot find the rapidobj package automatically. We can fix this by providing a hint about the cmake directory whereabouts:

```cmake
add_executable(my_app my_src.cpp)

find_package(RapidObj REQUIRED HINTS $ENV{HOME}/local/cmake)

target_link_libraries(my_app PRIVATE rapidobj::rapidobj)
```

Once the package has been successfully installed, rapidobj directory can be deleted.

#### Embedded

Another way to use rapidobj is to embed it inside your project. In your project's root, create a folder named thirdparty and then copy rapidobj to this folder. Installation is not required; it is sufficient to add rapidobj's subfolder to your project:

```cmake
add_executable(my_app my_src.cpp)

add_subdirectory(thirdparty/rapidobj)

target_link_libraries(my_app PRIVATE rapidobj::rapidobj)
```

If you do not wish to manually download and place rapidobj files, you can automate these steps by using CMake's FetchContent module:

```cmake
add_executable(my_app my_src.cpp)

include(FetchContent)

FetchContent_Declare(rapidobj
    GIT_REPOSITORY  https://github.com/guybrush77/rapidobj.git
    GIT_TAG         origin/master)

FetchContent_MakeAvailable(rapidobj)

target_link_libraries(my_app PRIVATE rapidobj::rapidobj)
```

## API

### ParseFile

Loads an .obj file, parses it and returns a result object.

**Signature:**

```c++
Result ParseFile(
    const std::filesystem::path& obj_filepath,
    const MaterialLibrary&       mtl_library = MaterialLibrary::Default())
```

**Parameters:**

- `obj_filepath` - Path to .obj file to be parsed.
- `mtl_library` - MaterialLibrary object specifies .mtl file search path(s) and loading policy.

**Result:**

- `Result` - The .obj file data in a binary format.

<details>
<summary><i>Show examples</i></summary>
  
```c++
Result result = ParseFile("/home/user/teapot/teapot.obj");
```

</details>

### MaterialLibrary

An object of type MaterialLibrary is used as an argument for the ParseFile function. It tells the ParseFile function how materials are to be handled.

**Signature:**

```c++
struct MaterialLibrary
{
    static MaterialLibrary Default(Load policy = Load::Mandatory);
    static MaterialLibrary SearchPath(std::filesystem::path path, Load policy = Load::Mandatory);
    static MaterialLibrary SearchPaths(std::vector<std::filesystem::path> paths, Load policy = Load::Mandatory);
    static MaterialLibrary String(std::string_view text);
    static MaterialLibrary Ignore();
};
```

**`Default`**

A convenience constructor identical to `MaterialLibrary::SearchPath(".")`.

<details>
<summary><i>Show examples</i></summary>

```c++
// Default search assumes .obj and .mtl file are in the same folder.
//  
// home
// └── user
//     └── teapot
//         ├── teapot.mtl
//         └── teapot.obj
Result result = ParseFile("/home/user/teapot/teapot.obj", MaterialLibrary::Default());
```

```c++
// MaterialLibrary::Default can be omitted since it is the default argument of ParseFile.
//
// home
// └── user
//     └── teapot
//         ├── teapot.mtl
//         └── teapot.obj
Result result = ParseFile("/home/user/teapot/teapot.obj");
```

</details>

**`SearchPath`**

Constructor used to specify .mtl file's relative or absolute search path and file loading policy (search path is relative to .obj file's parent folder).

<details>
<summary><i>Show examples</i></summary>

```c++
// Look for .mtl file in subfolder 'materials'.
//
// home
// └── user
//     └── teapot
//         ├── materials
//         │   └── teapot.mtl
//         └── teapot.obj
Result result = ParseFile("/home/user/teapot/teapot.obj", MaterialLibrary::SearchPath("materials"));
```

```c++
// Look for .mtl file in an arbitrary file folder.
//
// home
// └── user
//     ├── materials
//     │   └── teapot.mtl
//     └── teapot
//         └── teapot.obj
Result result = ParseFile("/home/user/teapot/teapot.obj", MaterialLibrary::SearchPath("/home/user/materials"));
```

</details>

**`SearchPaths`**

Constructor used to specify .mtl file's relative or absolute search paths and file loading policy (search paths are relative to .obj file's parent folder). The paths are examined in order; the first .mtl file found will be the one to be loaded and parsed.

<details>
<summary><i>Show examples</i></summary>

```c++
// Look for .mtl file in folder /home/user/teapot/materials, then /home/user/materials, then /home/user/teapot.
//
// home
// └── user
//     ├── materials
//     │   └── teapot.mtl
//     └── teapot
//         ├── materials
//         │   └── teapot.mtl
//         ├── teapot.mtl
//         └── teapot.obj
MaterialLibrary mtllib = MaterialLibrary::SearchPaths({ "materials", "../materials", "." });
Result          result = ParseFile("/home/user/teapot/teapot.obj", mtllib);
```

</details>

**`String`**

Constructor used to provide .mtl material description as a string.

<details>
<summary><i>Show examples</i></summary>

```c++
// Define a material library as a string, then pass it to ParseFile function.
//
// home
// └── user
//     └── teapot
//         └── teapot.obj
static constexpr auto materials = R"(
    newmtl red
    Kd 1 0 0
)";
Result result = ParseFile("/home/user/teapot/teapot.obj", MaterialLibrary::String(materials));
```

</details>

**`Ignore`**

Constructor used to instruct ParseFile to ignore material library, regardless of whether it is present or not. Materials array will be empty. Mesh material_ids arrays will be empty.

<details>
<summary><i>Show examples</i></summary>
  
```c++
// Instruct ParseFile to ignore teapot.mtl file.
//
// home
// └── user
//     └── teapot
//         ├── teapot.mtl
//         └── teapot.obj
Result result = ParseFile("/home/user/teapot/teapot.obj", MaterialLibrary::Ignore());
```

</details>

### Load Policy

Load is passed as an argument to MaterialLibrary::SearchPath(s) constructors to specify which actions to take if the material library file cannot be opened.

Mandatory loading instructs the ParseFile function to immediately stop further execution and produce an error if the .mtl file cannot be opened.

Optional loading instructs the ParseFile function to continue .obj loading and parsing, even if the .mtl file cannot be opened. However, Materials array will be empty. Mesh material_ids will contain ids assigned in order of appearance in the .obj file (0, 1, 2 etc.).

**Signature:**

```c++
enum class Load { Mandatory, Optional };
```

<details>
<summary><i>Show examples</i></summary>
  
```c++
// Look for .mtl file in subfolder 'materials'.
// If the file cannot be found, ParseFile function will fail with an error.
//
MaterialLibrary mtllib = MaterialLibrary::SearchPath("materials", Load::Mandatory);
Result          result = ParseFile("/home/user/teapot/teapot.obj", mtllib);
```

```c++
// Look for .mtl file in subfolder 'materials'.
// If the file cannot be found, ParseFile function will continue .obj loading and parsing.
//
MaterialLibrary mtllib = MaterialLibrary::SearchPath("materials", Load::Optional);
Result          result = ParseFile("/home/user/teapot/teapot.obj", mtllib);
```

```c++
// Look for .mtl file in default location.
// If the file cannot be found, ParseFile function will continue .obj loading and parsing.
//
MaterialLibrary mtllib = MaterialLibrary::Default(Load::Optional);
Result          result = ParseFile("/home/user/teapot/teapot.obj", mtllib);
```

</details>

### Triangulate

Triangulate all meshes in the Result object.

**Signature:**

```c++
bool Triangulate(Result& result)
```

**Parameters:**

- `result` - Result object obtained from calling ParseFile function.

**Result:**

- `bool` - True if triangulation was successful; false otherwise.

<details>
<summary><i>Show examples</i></summary>
  
```c++
Result result  = ParseFile("/home/user/teapot/teapot.obj");
bool   success = Triangulate(result);
```

</details>

## Data Layout

### Result

Result object is the return value of the ParseFile function. It contains the .obj and .mtl file data in binary format.

![rapidobj::Result](data/images/docs/result-light.png#gh-light-mode-only)
![rapidobj::Result](data/images/docs/result-dark.png#gh-dark-mode-only)

### Attributes

Attributes class contains four linear arrays which store vertex positions, texture coordinates, normals and colors data. The element value type is 32-bit float. Only vertex positions are mandatory. Texture coordinates, normals and color attribute arrays can be empty. Array elements are interleaved as { x, y, z } for positions and normals, { u, v } for texture coordinates and { r, g, b } for colors.

**`Attributes::positions`**

<table>
    <tr>
        <th colspan=3>p<sub>0</sub></th>
        <th colspan=3>p<sub>1</sub></th>
        <th colspan=3>p<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th colspan=3>p<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>x</td><td>y</td><td>z</td>
        <td>x</td><td>y</td><td>z</td>
        <td>x</td><td>y</td><td>z</td>
        <td>x</td><td>y</td><td>z</td>
    </tr>
</table>

**`Attributes::texcoords`**

<table>
    <tr>
        <th colspan=2>t<sub>0</sub></th>
        <th colspan=2>t<sub>1</sub></th>
        <th colspan=2>t<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th colspan=3>t<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>u</td><td>v</td>
        <td>u</td><td>v</td>
        <td>u</td><td>v</td>
        <td>u</td><td>v</td>
    </tr>
</table>

**`Attributes::normals`**

<table>
    <tr>
        <th colspan=3>n<sub>0</sub></th>
        <th colspan=3>n<sub>1</sub></th>
        <th colspan=3>n<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th colspan=3>n<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>x</td><td>y</td><td>z</td>
        <td>x</td><td>y</td><td>z</td>
        <td>x</td><td>y</td><td>z</td>
        <td>x</td><td>y</td><td>z</td>
    </tr>
</table>

**`Attributes::colors`**

<table>
    <tr>
        <th colspan=3>c<sub>0</sub></th>
        <th colspan=3>c<sub>1</sub></th>
        <th colspan=3>c<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th colspan=3>c<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>r</td><td>g</td><td>b</td>
        <td>r</td><td>g</td><td>b</td>
        <td>r</td><td>g</td><td>b</td>
        <td>r</td><td>g</td><td>b</td>
    </tr>
</table>

### Shape

Shape is a polyhedral mesh (`Mesh`), a set of polylines (`Lines`) or a set of points (`Points`).

### Mesh

Mesh class defines the shape of a polyhedral object. The geometry data is stored in two arrays: indices and num_face_vertices. Per face material information is stored in the material_ids array. Smoothing groups, used for normal interpolation, are stored in the smoothing_group_ids array.

**`Mesh::indices`**

The indices array is a collection of faces formed by indexing into vertex attribute arrays. It is a linear array of Index objects. Index class has three fields: position_index, texcoord_index, and normal_index. Only the position_index is mandatory; a vertex normal and UV coordinates are optional. For optional attributes, invalid index (-1) is stored in the normal_index and texcoord_index fields.

<table>
    <tr>
        <th colspan=3>f<sub>0</sub></th>
        <th colspan=5>f<sub>1</sub></th>
        <th colspan=4>f<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th colspan=3>f<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>i<sub>0</sub></td><td>i<sub>1</sub></td><td>i<sub>2</sub></td>
        <td>i<sub>3</sub></td><td>i<sub>4</sub></td><td>i<sub>5</sub></td><td>i<sub>6</sub></td><td>i<sub>7</sub></td>
        <td>i<sub>8</sub></td><td>i<sub>9</sub></td><td>i<sub>10</sub></td><td>i<sub>11</sub></td>
        <td>i<sub>N-3</sub></td><td>i<sub>N-2</sub></td><td>i<sub>N-1</sub></td>
    </tr>
</table>

**`Mesh::num_face_vertices`**

A mesh face can have three (triangle), four (quad) or more vertices. Because the indices array is flat, extra information is required to identify which indices are associated with a particular face. The number of vertices for each face [3..255] is stored in the num_face_vertices array. The size of the num_face_vertices array is equal to the number of faces in the mesh.

<table>
    <tr>
        <th>f<sub>0</sub></th>
        <th>f<sub>1</sub></th>
        <th>f<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th>f<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>3</td><td>5</td><td>4</td><td>3</td>
    </tr>
</table>

**`Mesh::material_ids`**

Material IDs index into the the Materials array.

<table>
    <tr>
        <th>f<sub>0</sub></th>
        <th>f<sub>1</sub></th>
        <th>f<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th>f<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>m<sub>0</sub></td><td>m<sub>1</sub></td><td>m<sub>2</sub></td><td>m<sub>N-1</sub></td>
    </tr>
</table>

**`Mesh::smoothing_group_ids`**

 Smoothing group IDs can be used to calculate vertex normals.

<table>
    <tr>
        <th>f<sub>0</sub></th>
        <th>f<sub>1</sub></th>
        <th>f<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th>f<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>s<sub>0</sub></td><td>s<sub>1</sub></td><td>s<sub>2</sub></td><td>s<sub>N-1</sub></td>
    </tr>
</table>

### Lines

Lines class contains a set of polylines. The geometry data is stored in two arrays: indices and num_line_vertices.

**`Lines::indices`**

The indices array defines polylines by indexing into vertex attribute arrays. It is a linear array of Index objects. Index class has three fields: position_index, texcoord_index, and normal_index. The position_index is mandatory. UV coordinates are optional. If UV coordinates are not present, invalid index (-1) is stored in the texcoord_index fields. The normal_index is always set to invalid index.

<table>
    <tr>
        <th colspan=6>l<sub>0</sub></th>
        <th colspan=2>l<sub>1</sub></th>
        <th colspan=4>l<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th colspan=5>l<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>i<sub>0</sub></td><td>i<sub>1</sub></td><td>i<sub>2</sub></td><td>i<sub>3</sub></td><td>i<sub>4</sub></td><td>i<sub>5</sub></td>
        <td>i<sub>6</sub></td><td>i<sub>7</sub></td>
        <td>i<sub>8</sub></td><td>i<sub>9</sub></td><td>i<sub>10</sub></td><td>i<sub>11</sub></td>
        <td>i<sub>N-5</sub></td><td>i<sub>N-4</sub></td><td>i<sub>N-3</sub></td><td>i<sub>N-2</sub></td><td>i<sub>N-1</sub></td>
    </tr>
</table>

**`Lines::num_line_vertices`**

A polyline can have two or more vertices. Because the indices array is flat, extra information is required to identify which indices are associated with a particular polyline. The number of vertices for each polyline [2..2<sup>31</sup>) is stored in the num_line_vertices array. The size of the num_line_vertices array is equal to the number of polylines.

<table>
    <tr>
        <th>l<sub>0</sub></th>
        <th>l<sub>1</sub></th>
        <th>l<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th>l<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>6</td><td>2</td><td>4</td><td>5</td>
    </tr>
</table>

### Points

Points class contains a set of points. The geometry data is stored in the indices array.

**`Points::indices`**

The indices array defines points by indexing into vertex attribute arrays. It is a linear array of Index objects. Index class has three fields: position_index, texcoord_index, and normal_index. The position_index is mandatory. UV coordinates are optional. If UV coordinates are not present, invalid index (-1) is stored in the texcoord_index fields. The normal_index is always set to invalid index.

<table>
    <tr>
        <th>p<sub>0</sub></th>
        <th>p<sub>1</sub></th>
        <th>p<sub>2</sub></th>
        <th rowspan=2>...</th>
        <th>p<sub>N-1</sub></th>
    </tr>
    <tr>
        <td>i<sub>0</sub></td><td>i<sub>1</sub></td><td>i<sub>2</sub></td><td>i<sub>N-1</sub></td>
    </tr>
</table>

### Materials

After ParseFunction loads and parses the .mtl file, all the material information is stored in the Result Materials array.

#### Material Parameters

| Parameter                   | Keyword  | Type          | Description                          |
|-----------------------------|----------|---------------|--------------------------------------|
| ambient                     | Ka       | color         | Ambient reflectance                  |
| diffuse                     | Kd       | color         | Diffuse reflectance                  |
| specular                    | Ks       | color         | Specular reflectance                 |
| transmittance               | Kt       | color         | Transparency                         |
| emission                    | Ke       | color         | Emissive coeficient                  |
| shininess                   | Ns       | float         | Specular exponent                    |
| ior                         | Ni       | float         | Index of refraction                  |
| dissolve                    | d        | float         | Fake transparency                    |
| illum                       | illum    | int           | Illumination model                   |
| ambient_texname             | map_Ka   | path          | Path to ambient texture              |
| diffuse_texname             | map_Kd   | path          | Path to diffuse texture              |
| specular_texname            | map_Ks   | path          | Path to specular texture             |
| specular_highlight_texname  | map_Ns   | path          | Path to specular highlight texture   |
| bump_texname                | map_bump | path          | Path to bump map texture             |
| displacement_texname        | disp     | path          | Path to displacement map texture     |
| alpha_texname               | map_d    | path          | Path to alpha texture                |
| reflection_texname          | refl     | path          | Path to reflection map texture       |
| ambient_texopt              |          | TextureOption | Ambient texture options              |
| diffuse_texopt              |          | TextureOption | Diffuse texture options              |
| specular_texopt             |          | TextureOption | Specular texture options             |
| specular_highlight_texopt   |          | TextureOption | Specular highlight texture options   |
| bump_texopt                 |          | TextureOption | Bump map texture options             |
| displacement_texopt         |          | TextureOption | Displacement map texture options     |
| alpha_texopt                |          | TextureOption | Alpha texture options                |
| reflection_texopt           |          | TextureOption | Reflection map texture options       |

#### Material Parameters (PBR Extension)

| Parameter                   | Keyword  | Type          | Description                          |
|-----------------------------|----------|---------------|--------------------------------------|
| roughness                   | Pr       | float         | Surface roughness                    |
| metallic                    | Pm       | float         | Surface metalness                    |
| sheen                       | Ps       | float         | Amount of soft reflection near edges |
| clearcoat_thickness         | Pc       | float         | Extra white specular layer on top    |
| clearcoat_roughness         | Pcr      | float         | Roughness of white specular layer    |
| anisotropy                  | aniso    | float         | Anisotropy for specular reflection   |
| anisotropy_rotation         | anisor   | float         | Anisotropy rotation amount           |
| roughness_texname           | map_Pr   | path          | Path to roughness texture            |
| metallic_texname            | map_Pm   | path          | Path to metalness texture            |
| sheen_texname               | map_Ps   | path          | Path to sheen texture                |
| emissive_texname            | map_Ke   | path          | Path to emissive texture             |
| normal_texname              | norm     | path          | Path to normal texture               |
| roughness_texopt            |          | TextureOption | Roughness texture options            |
| metallic_texopt             |          | TextureOption | Metalness texture options            |
| sheen_texopt                |          | TextureOption | Sheen texture options                |
| emissive_texopt             |          | TextureOption | Emissive texture options             |
| normal_texopt               |          | TextureOption | Normal texture options               |

#### TextureOption

| Parameter                   | Keyword  | Type          | Description                          |
|-----------------------------|----------|---------------|--------------------------------------|
| type                        | type     | TextureType   | Mapping type (None, Sphere, Cube)    |
| sharpness                   | boost    | float         | Boosts mip-map sharpness             |
| brightness                  | mm       | float         | Controls texture brightness          |
| contrast                    | mm       | float         | Controls texture contrast            |
| origin_offset               | o        | float3        | Moves texture origin                 |
| scale                       | s        | float3        | Adjusts texture scale                |
| turbulence                  | t        | float3        | Controls texture turbulence          |
| texture_resolution          | texres   | int           | Texture resolution to create         |
| clamp                       | clamp    | bool          | Only render texels in clamped range  |
| imfchan                     | imfchan  | char          | Specifies channels of the file to use|
| blendu                      | blendu   | bool          | Set horizontal texture blending      |
| blendv                      | blendv   | bool          | Set vertical texture blending        |
| bump_multiplier             | bm       | float         | Bump map multiplier                  |

## Example

Suppose we want to find out the total number of triangles in an .obj file. This can be accomplished by passing the .obj file path to```ParseFile()``` and triangulating the result. The next step is looping through all the meshes; in each iteration, the number of triangles in the current mesh is added to the running sum. The code for this logic is shown below:

```cpp
#include "rapidobj/rapidobj.hpp"

#include <iostream>

int main()
{
    rapidobj::Result result = rapidobj::ParseFile("/path/to/my.obj");

    if (result.error) {
        std::cout << result.error.code.message() << '\n';
        return EXIT_FAILURE;
    }

    bool success = rapidobj::Triangulate(result);

    if (!success) {
        std::cout << result.error.code.message() << '\n';
        return EXIT_FAILURE;
    }

    size_t num_triangles{};

    for (const auto& shape : result.shapes) {
        num_triangles += shape.mesh.num_face_vertices.size();
    }

    std::cout << "Shapes:    " << result.shapes.size() << '\n';
    std::cout << "Materials: " << result.materials.size() << '\n';
    std::cout << "Triangles: " << num_triangles << '\n';

    return EXIT_SUCCESS;
}
```

## Next Steps

Typically, parsed .obj data cannot be used "as is". For instance, for hardware rendering, a number of additional processing steps are required so that the data is in a format easily consumed by a GPU. rapidobj provides one convenience function, ```Triangulate()```, to assist with this task. Other tasks must be implemented by the rendering application. These may include:

- Gathering all the attributes so that the vertex data is in a single array of interleaved attributes. This step may optionally include vertex deduplication.
- Generate normals in case they are not provided in the .obj file. This step may use smoothing groups (if any) to create higher quality normals.
- Optionally optimise the meshes for rendering based on some criteria such as: material type, mesh size, number of batches to be submitted, etc.

## OS Support

- Windows
- macOS
- Linux

## Third Party Tools and Resources

This is a list of third party tools and resources used by this project:

- [3D models](https://casual-effects.com/data/) from McGuire Computer Graphics Archive
- [cereal](https://uscilab.github.io/cereal/) for serialization
- [CMake](https://cmake.org/) for build automation
- [cxxopts](https://github.com/jarro2783/cxxopts) for command line option parsing
- [doctest](https://github.com/onqtam/doctest) for testing
- [earcut.hpp](https://github.com/mapbox/earcut.hpp) for polygon triangulation
- [fast_float](https://github.com/fastfloat/fast_float) for string to float parsing
- [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader) for .obj file parsing
- [xxHash](https://github.com/Cyan4973/xxHash) for hashing

## License

The rapidobj single-header library is licensed under the MIT License.

The rapidobj single-header library contains a copy of [fast_float](https://github.com/fastfloat/fast_float) number parsing library from Daniel Lamire which is licensed under the MIT License as well as under the Apache 2.0 License.

The rapidobj single-header library contains a copy of [earcut.hpp](https://github.com/mapbox/earcut.hpp) polygon triangulation library from Mapbox which is licensed under the ISC License.
