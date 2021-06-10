# Introduction

Rapidobj is a single header C++17 library that loads and parses [Wavefront .obj files](https://en.wikipedia.org/wiki/Wavefront_.obj_file).

The .obj file format was first used by Wavefront Technologies around 1990. However, this 3D geometry file format did not age well. An .obj file is a text file and, consequently, large models take a lot of of disk space and are slow to load and parse. Moreover, after loading and parsing .obj files, additional processing steps are required to transform the data into a format suitable for hardware rendering.

Rapidobj library, as implied by its name, was designed to quickly load and parse large .obj files. The API was heavily influenced by another single header C++ library, [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader). In terms of interface the two libraries are fairly similar, but rapidobj was optimised to take advantage of modern computer hardware. It is well suited to parse large .obj files on computers with multi-core CPUs and fast solid-state disk drives.

# Using Rapidobj

The API of the rapidobj library is rather simple. It consists of just two free-standing functions: ```ParseFile()``` and ```Triangulate()```. For example, suppose we want to print the total number of triangles in an .obj file. We can use the ```ParseFile()``` function to load and parse the file; parsed binary data is returned in ```struct Result```. Next, we triangulate all the meshes in ```Result``` by invoking the ```Triangulate()``` function. If the meshes are already triangulated, the function call will have no effect. Finally, we loop through all the shapes, and add the number of triangles in mesh to a running sum. The code for this is shown below:

```cpp
#include "rapidobj/rapidobj.hpp"

#include <iostream>

int main()
{
    auto result = rapidobj::ParseFile("/home/user/3d/model.obj");

    if (result.error) {
        std::cout << result.error.code.message() << '\n';
        return EXIT_FAILURE;
    }

    rapidobj::Triangulate(result);

    if (result.error) {
        std::cout << result.error.code.message() << '\n';
        return EXIT_FAILURE;
    }

    auto num_triangles = size_t();

    for (const auto& shape : result.shapes) {
        num_triangles += shape.mesh.num_face_vertices.size();
    }

    std::cout << "Shapes:    " << result.shapes.size() << '\n';
    std::cout << "Triangles: " << num_triangles << '\n';

    return EXIT_SUCCESS;
}
```
