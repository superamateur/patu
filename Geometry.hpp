#ifndef __GEOMETRY_H
#define __GEOMETRY_H
#include "func_defs.h"
#include "Transformer.hpp"

typedef struct FaceIndex__
{
	std::vector<int> vertices;
	int& operator[](const size_t i) {
		return vertices[i];
	}
	void push_back(const int id) {
		vertices.push_back(id);
	}
	FaceIndex__() {}
	FaceIndex__(const int* indices, const int num) 
	{
		vertices.resize(num);
		for(int i = 0; i < num; ++i) {
			vertices[i] = indices[i];
		}
	}
} FaceIndex_t;

typedef struct Point3D__
{
	float p[3];
	float& operator[](const size_t id) {
		return p[id];
	}
	const float operator[](const size_t id) const {
		return p[id];
	}
	Point3D__() {}
	Point3D__(const float* xyz)
	{
		p[0] = xyz[0];
		p[1] = xyz[1];
		p[2] = xyz[2];
	}
	Point3D__(const float x, const float y, const float z)
	{
		p[0] = x;
		p[1] = y;
		p[2] = z;
	}
	Point3D__ operator - (const Point3D__& other) {
		return Point3D__(p[0] - other[0], p[1] - other[1], p[2] - other[2]);
	}
	Point3D__ operator + (const Point3D__& other) {
		return Point3D__(p[0] + other[0], p[1] + other[1], p[2] + other[2]);
	}
	float operator * (const Point3D__& other) {
		return (p[0] * other[0] + p[1] * other[1] + p[2] * other[2]);
	}
} Point3D_t;

typedef struct TCoord__
{
	float uv[2];
	float& operator[](const size_t id) {
		return uv[id];
	}
	const float& operator[](const size_t id) const {
		return uv[id];
	}
	TCoord__() {}
	TCoord__(const float* coord)
	{
		uv[0] = coord[0];
		uv[1] = coord[1];
	}
	TCoord__(const float u, const float v)
	{
		uv[0] = u;
		uv[1] = v;
	}
} TCoord_t;

typedef Point3D_t Vector3D_t;

class Geometry
{
public:
	Geometry(void);
	virtual ~Geometry(void);

	SET_GET_MACRO(int, vertex_num, vertex_num);
	SET_GET_MACRO(int, face_num, face_num);
	
	void insert_vertex(const float* xyz, const int mtl_id = 0);
	void insert_vertex(const float x, const float y, const float z, const int mtl_id = 0);

	void insert_tcoord(const float* uv);
	void insert_tcoord(const float u, const float v);

	void insert_normal(const float* xyz);
	void insert_normal(const float x, const float y, const float z);

	void insert_face(const int* indices, const int num);
	void insert_face_tcoord(const int* indices, const int num);
	void insert_face_normal(const int* indices, const int num);

	bool has_tcoord(void) { return !m_tcoords.empty(); }
	bool has_normal(void) { return !m_normals.empty(); }
	bool validate(void);

	void set_transform(const Transformer& transformer);

	void show_off(void);

	const std::vector<Point3D_t>& vertices(void) const { return m_vertices; }
	const std::vector<FaceIndex_t>& faces(void)  const { return m_face_indices; }
	const std::vector<TCoord_t>& tcoords(void)  const { return m_tcoords; }

	void set_vertex_offset(const int offset);
	bool planar_cut(Geometry& g1, Geometry& g2, const Point3D_t& origin, const Vector3D_t& normal);

	bool export_to_obj(const std::string& file_name);
private:
	int m_vertex_num;
	int m_face_num;
	std::vector<Point3D_t> m_vertices;
	std::vector<Vector3D_t> m_normals;
	std::vector<TCoord_t> m_tcoords;
	std::vector<int> m_mtl_ids;
	std::vector<FaceIndex_t> m_face_indices;
	std::vector<FaceIndex_t> m_face_tcoord_indices;
	std::vector<FaceIndex_t> m_face_normal_indices;
};

#endif
