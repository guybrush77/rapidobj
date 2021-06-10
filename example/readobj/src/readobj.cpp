#include "rapidobj/rapidobj.hpp"

#include <iostream>

void ReportError(const rapidobj::Error& error)
{
    std::cout << error.code.message() << "\n";
    if (!error.line.empty()) {
        std::cout << "On line " << error.line_num << ": \"" << error.line << "\"\n";
    }
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << *argv << " input-file\n";
        return EXIT_FAILURE;
    }

    auto result = rapidobj::ParseFile(argv[1]);

    if (result.error) {
        ReportError(result.error);
        return EXIT_FAILURE;
    }

    rapidobj::Triangulate(result);

    if (result.error) {
        ReportError(result.error);
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
