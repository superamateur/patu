#ifndef __GEOMETRY_H
#define __GEOMETRY_H
#include "func_defs.h"
#include "Transformer.hpp"
#include "Vector3D.hpp"

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

//typedef struct Point3D__
//{
//	float p[3];
//	float& operator[](const size_t id) {
//		return p[id];
//	}
//	const float operator[](const size_t id) const {
//		return p[id];
//	}
//	Point3D__() {}
//	Point3D__(const float* xyz)
//	{
//		p[0] = xyz[0];
//		p[1] = xyz[1];
//		p[2] = xyz[2];
//	}
//	Point3D__(const float x, const float y, const float z)
//	{
//		p[0] = x;
//		p[1] = y;
//		p[2] = z;
//	}
//	Point3D__ operator - (const Point3D__& other) {
//		return Point3D__(p[0] - other[0], p[1] - other[1], p[2] - other[2]);
//	}
//	Point3D__ operator + (const Point3D__& other) {
//		return Point3D__(p[0] + other[0], p[1] + other[1], p[2] + other[2]);
//	}
//	float operator * (const Point3D__& other) {
//		return (p[0] * other[0] + p[1] * other[1] + p[2] * other[2]);
//	}
//} Point3D_t;

typedef Vector3D Point3D_t;

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

typedef std::vector<TCoord_t> TCoordArray;
typedef Point3D_t Vector3D_t;

struct SVertex {
	int m_index;
	std::vector<int> m_adjacent_face_id;
	bool m_is_boundary;
	std::vector<int> m_boundary_partner;
	SVertex(const int idx) : m_index(idx), m_is_boundary(false) {}
	SVertex() : m_index(0), m_is_boundary(false) {}
	void show_off() {
		std::cout << "index = " << m_index << ", Adj faces = ";
		for(IntIterType it = m_adjacent_face_id.begin(); it != m_adjacent_face_id.end(); ++it)
		{
			std::cout << *it << " ";
		}
		std::cout << ", is boundary: " << m_is_boundary << " , partner = ";
		for(IntIterType it = m_boundary_partner.begin(); it != m_boundary_partner.end(); ++it)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
};
typedef std::vector<SVertex> VertexArray;

struct SFace {
	int m_index;
	std::vector<int> m_vertex_id;
	std::vector<int> m_normal_id;
	std::vector<int> m_texure_id;
	void show_off() const {
		std::cout << "index = " << m_index << ", vert = ";
		for(ConstIntIterType it = m_vertex_id.begin(); it != m_vertex_id.end(); ++it)
		{
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
};
typedef std::vector<SFace> FaceArray;

struct SBoundingBox {
	Vector3D from;
	Vector3D to;
	SBoundingBox() {
		from = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
		to   = Vector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	}
	void update(const Vector3D& point) {
		for(int i = 0; i < 3; ++i) {
			from[i] = std::min<float>(from[i], point[i]);
			to[i] = std::max<float>(to[i], point[i]);
		}
		
	}
};

class Geometry
{
public:
	Geometry(void);
	virtual ~Geometry(void);
	
	SVertex insert_vertex(const float* xyz, const int mtl_id = 0);
	SVertex insert_vertex(const float x, const float y, const float z, const int mtl_id = 0);

	void insert_tcoord(const float* uv);
	void insert_tcoord(const float u, const float v);

	void insert_normal(const float* xyz);
	void insert_normal(const float x, const float y, const float z);

	SFace insert_face(const std::vector<int>& vert_id, const std::vector<int>& texture_id, const std::vector<int>& norm_id);

	bool has_tcoord(void) { return !m_tcoords.empty(); }
	bool has_normal(void) { return !m_normals.empty(); }
	bool validate(void);

	void set_transform(const Transformer& transformer);

	void show_off(void);

	const VertexArray& vertices(void) const { return m_vertices; }
	const FaceArray& faces(void)  const { return m_faces; }
	const TCoordArray& tcoords(void)  const { return m_tcoords; }
	bool planar_cut(Geometry& g1, Geometry& g2, const Point3D_t& origin, const Vector3D_t& normal);

	bool export_to_obj(const std::string& file_name, const int id_offset = 1);

	Point3D_t get_geometry_center(void);

	void mark_vert_as_boundary(const int id, const int partner) 
	{ 
		m_vertices.at(id).m_is_boundary = true;
		m_vertices.at(id).m_boundary_partner.push_back(partner);

		m_vertices.at(partner).m_is_boundary = true;
		m_vertices.at(partner).m_boundary_partner.push_back(id);
	}
	
	Geometry make_flatten(void);
	SBoundingBox get_bounding_box() { return m_bounding_box; }
	Point3D_t get_vertex_pos(const int id) {return m_vert_pos.at(id);}

	int get_instance_id() const { return m_instance_id_number; }
	std::vector<int> list_adjacent_verts(const int curr_vert_id);
private:
	std::vector<int> get_longest_boundary_chain();

private:
	std::vector<Point3D_t> m_vert_pos;
	std::vector<Vector3D_t> m_normals;
	std::vector<TCoord_t> m_tcoords;
	std::vector<int> m_mtl_ids;

	VertexArray m_vertices;
	FaceArray m_faces;
	SBoundingBox m_bounding_box;
	int m_instance_id_number;
};

#endif
