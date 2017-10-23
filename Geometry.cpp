#include "Geometry.hpp"

Geometry::Geometry(void)
: m_vertex_num(0)
, m_face_num(0)
, m_vertices(0)
, m_normals(0)
, m_tcoords(0)
, m_mtl_ids(0)
, m_face_indices(0)
, m_face_tcoord_indices(0)
, m_face_normal_indices(0)
{
}

Geometry::~Geometry(void)
{
}

void Geometry::insert_vertex(const float *xyz, const int mtl_id)
{
	m_mtl_ids.push_back(mtl_id);
	m_vertices.push_back(Point3D_t(xyz));
}

void Geometry::insert_vertex(const float x, const float y, const float z, const int mtl_id)
{
	m_mtl_ids.push_back(mtl_id);
	m_vertices.push_back(Point3D_t(x,y,z));
}

void Geometry::insert_tcoord(const float* uv)
{
	m_tcoords.push_back(TCoord_t(uv));
}

void Geometry::insert_tcoord(const float u, const float v)
{
	m_tcoords.push_back(TCoord_t(u, v));
}

void Geometry::insert_normal(const float* xyz)
{
	m_normals.push_back(Vector3D_t(xyz));
}

void Geometry::insert_normal(const float x, const float y, const float z)
{
	m_normals.push_back(Vector3D_t(x, y, z));
}

void Geometry::insert_face(const int* indices, const int num)
{
	m_face_indices.push_back(FaceIndex_t(indices, num));
}

void Geometry::insert_face_tcoord(const int* indices, const int num)
{
	m_face_tcoord_indices.push_back(FaceIndex_t(indices, num));
}

void Geometry::insert_face_normal(const int* indices, const int num)
{
	m_face_normal_indices.push_back(FaceIndex_t(indices, num));
}

bool Geometry::validate(void) {
	// vertex and face should not be empty
	if(m_vertices.empty() || m_face_indices.empty()) return false;

	// check consistency between number of vertex, normal, texture coords, face index
	const size_t nvertex = m_vertices.size();
	const size_t nface = m_face_indices.size();

	if( has_normal() ) {
		if( nvertex != m_normals.size() ) return false;
		if( m_face_normal_indices.empty() ) return false;
		if( nface != m_face_normal_indices.size() ) return false;
	}

	if( has_tcoord() ) {
		if( nvertex != m_tcoords.size() ) return false;
		if( m_face_tcoord_indices.empty() ) return false;
		if( nface != m_face_tcoord_indices.size() ) return false;
	}
	m_vertex_num = nvertex;
	m_face_num = nface;
	for(size_t i = 0; i < nface; ++i) {
		int v0 = m_face_indices[i][0] - 1;
		int v1 = m_face_indices[i][1] - 1;
		int v2 = m_face_indices[i][2] - 1;
		
		Vector3D vec1(m_vertices[v0].p);
		Vector3D vec2(m_vertices[v1].p);
		Vector3D vec3(m_vertices[v2].p);
		
		Vector3D l1, l2;
		l2 = vec3 - vec2;			
		l1 = vec2 - vec1;
		if(l1.get_length() > 1 && l2.get_length() > 1) {
			std::cout << "v " << vec1[0]  << " " << vec1[1] << " " << vec1[2] << std::endl;
			std::cout << "v " << vec2[0]  << " " << vec2[1] << " " << vec2[2] << std::endl;
			std::cout << "v " << vec3[0]  << " " << vec3[1] << " " << vec3[2] << std::endl;
			std::cout << v2  << std::endl;
		}
	}
	return true;
}

void Geometry::set_transform(const Transformer& transformer)
{
	const Matrix& cMatrix = transformer.get_matrix();
	
	// transform vertices
	for(std::vector<Point3D_t>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it) {
		Vector3D tmp = cMatrix.v_post_multiply(Vector3D(it->p));
		*it = Point3D_t(tmp.vector());
	}

	// transform normal
	for(std::vector<Point3D_t>::iterator it = m_normals.begin(); it != m_normals.end(); ++it) {
		Vector3D tmp = cMatrix.v_post_multiply(Vector3D(it->p));
		*it = Point3D_t(tmp.vector());
	}
}

void Geometry::show_off(void)
{
	std::cout << "=== SHOW OFF START ===" << std::endl;
	std::cout << "Class name: Geometry" << std::endl;
	std::cout << "Num Vertex: " << m_vertices.size() << std::endl; 
	std::cout << "Num face: " << m_face_indices.size() << std::endl;
	std::cout << "Has TCoord: " << (has_tcoord() ? "TRUE" : "FALSE") << std::endl;
	std::cout << "Has Normal: " << (has_normal() ? "TRUE" : "FALSE") << std::endl;
	std::cout << "=== SHOW OFF END ===" << std::endl;
}

void Geometry::set_vertex_offset(const int offset)
{
	for(int i = 0; i < m_face_num; ++i) {
		m_face_indices[i].vertices[0] -= offset;
		m_face_indices[i].vertices[1] -= offset;
		m_face_indices[i].vertices[2] -= offset;
	}
}