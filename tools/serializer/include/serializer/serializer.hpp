#include "rapidobj/rapidobj.hpp"

namespace rapidobj {

//
// Helper functions used to compare rapidobj::Result for equality
//
inline bool operator==(const Index& lhs, const Index& rhs)
{
    return lhs.position_index == rhs.position_index && lhs.texcoord_index == rhs.texcoord_index &&
           lhs.normal_index == rhs.normal_index;
}

inline bool operator!=(const Index& lhs, const Index& rhs)
{
    return !operator==(lhs, rhs);
}

inline bool operator==(const Mesh& lhs, const Mesh& rhs)
{
    return lhs.indices == rhs.indices && lhs.num_face_vertices == rhs.num_face_vertices &&
           lhs.material_ids == rhs.material_ids && lhs.smoothing_group_ids == rhs.smoothing_group_ids;
}

inline bool operator!=(const Mesh& lhs, const Mesh& rhs)
{
    return !operator==(lhs, rhs);
}

inline bool operator==(const Shape& lhs, const Shape& rhs)
{
    return lhs.name == rhs.name && lhs.mesh == rhs.mesh;
}

inline bool operator!=(const Shape& lhs, const Shape& rhs)
{
    return !operator==(lhs, rhs);
}

inline bool operator==(const Attributes& lhs, const Attributes& rhs)
{
    return lhs.positions == rhs.positions && lhs.texcoords == rhs.texcoords && lhs.normals == rhs.normals;
}

inline bool operator==(const Result& lhs, const Result& rhs)
{
    return lhs.attributes == rhs.attributes && lhs.shapes == rhs.shapes;
}

inline bool operator!=(const Result& lhs, const Result& rhs)
{
    return !operator==(lhs, rhs);
}

namespace serializer {

void Serialize(const rapidobj::Result& result, const std::filesystem::path& filepath);

Result Deserialize(const std::filesystem::path& filepath);

std::string ComputeFileHash(std::filesystem::path filepath);

} // namespace serializer

} // namespace rapidobj
