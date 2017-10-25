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

bool Geometry::planar_cut(Geometry& g1, Geometry& g2, const Point3D_t& origin, const Vector3D_t& normal)
{
	std::vector<bool> front_face(m_vertex_num);
	std::vector<int> vert_index_map(m_vertex_num);
	int num_front_vert = 0, num_back_vert = 0;
	for(int i = 0; i < m_vertex_num; ++i) {
		Geometry& target_geom = (0.f > (m_vertices[i] - origin) * normal) ? g2 : g1;
		if(0.f > (m_vertices[i] - origin) * normal) {
			front_face[i] = false;
			vert_index_map[i] = num_back_vert;
			++num_back_vert;
		} else {
			front_face[i] = true;
			vert_index_map[i] = num_front_vert;
			++num_front_vert;
		}
		target_geom.insert_vertex(m_vertices[i].p); 
	}

	for(int i = 0; i < (int)m_face_indices.size(); ++i)
	{
		const FaceIndex_t& face = m_face_indices[i];
		int num_front = 0;
		int num_back = 0;
		for(int j = 0; j < (int)face.vertices.size(); ++j)
		{
			if(front_face[face.vertices[j]]) {
				++num_front;
			} else {
				++num_back;
			}
		}
		if(0 == num_front || 0 == num_back) { // the face totally belong to one of the two geoms
			Geometry& target_geom = (0 == num_back) ? g1 : g2;
			std::vector<int> f; // point id
			std::vector<int> t; // texture id
			std::vector<int> n; // normal id

			for(int k = 0; k < (int)face.vertices.size(); ++k) {
				f.push_back(vert_index_map[face.vertices[k]]);
			}
			target_geom.insert_face(&f[0], face.vertices.size());
		}
	}
	return (g1.validate() && g2.validate());
}

bool Geometry::export_to_obj(const std::string& file_name)
{
	std::fstream fs;
	fs.open(file_name.c_str(), std::ios_base::out);
	if( !fs.is_open() ) {
		false;
	}
	for(std::vector<Point3D_t>::iterator it = m_vertices.begin(); it != m_vertices.end(); ++it)
	{
		fs << "v " << (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << std::endl;
	}
	for(std::vector<FaceIndex_t>::iterator it = m_face_indices.begin(); it != m_face_indices.end(); ++it)
	{
		fs << "f " << it->vertices[0] << ", " << it->vertices[1] << ", " << it->vertices[2] << std::endl;
	}
	fs.close();
	return true;
}