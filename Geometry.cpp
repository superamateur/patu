#include "Geometry.hpp"
#include <cmath>
#include<Eigen/SparseLU>
#include <Eigen/Dense>
#include<Eigen/SparseCholesky>
#include<Eigen/SparseQR>

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

namespace {
	static int geometry_instance_number = 0;
}

Geometry::Geometry(void)
{
	m_instance_id_number = ++geometry_instance_number;
}

Geometry::~Geometry(void)
{
}

SVertex Geometry::insert_vertex(const float *xyz, const int mtl_id)
{
	m_mtl_ids.push_back(mtl_id);
	m_vert_pos.push_back(Point3D_t(xyz));
	m_vertices.push_back(SVertex((int)(m_vert_pos.size()) - 1));
	m_bounding_box.update(Point3D_t(xyz));
	return m_vertices.back();
}

SVertex Geometry::insert_vertex(const float x, const float y, const float z, const int mtl_id)
{
	m_mtl_ids.push_back(mtl_id);
	m_vert_pos.push_back(Point3D_t(x,y,z));
	m_vertices.push_back(SVertex((int)(m_vert_pos.size()) - 1));
	m_bounding_box.update(Point3D_t(x,y,z));
	return m_vertices.back();
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

SFace Geometry::insert_face(const std::vector<int>& vert_id, const std::vector<int>& texture_id, const std::vector<int>& norm_id)
{
	SFace nface;
	nface.m_index = (int)m_faces.size();
	nface.m_vertex_id = vert_id;
	nface.m_texure_id = texture_id;
	nface.m_normal_id = norm_id;
	m_faces.push_back(nface);
	for(int i = 0; i < (int)nface.m_vertex_id.size();++i)
	{
		m_vertices.at(nface.m_vertex_id[i]).m_adjacent_face_id.push_back(nface.m_index);
	}
	return nface;
}

bool Geometry::validate(void) {
	// vertex and face should not be empty
	if(m_vertices.empty() || m_faces.empty()) return false;

	// check consistency between number of vertex, normal, texture coords, face index
	const size_t nvertex = m_vertices.size();
	const size_t nface = m_faces.size();

	if( has_normal() ) {
		if( nvertex != m_normals.size() ) return false;
		
	}

	if( has_tcoord() ) {
		if( nvertex != m_tcoords.size() ) return false;
	}
	
	for(int i = 0; i < (int)m_vertices.size(); ++i)
	{
		if(m_vertices.at(i).m_index != i) return false;
	}

	for(int i = 0; i < (int)m_faces.size(); ++i)
	{
		if(m_faces.at(i).m_index != i) return false;
	}

	return true;
}

void Geometry::set_transform(const Transformer& transformer)
{
	const Matrix& cMatrix = transformer.get_matrix();
	
	// transform vertices
	for(std::vector<Point3D_t>::iterator it = m_vert_pos.begin(); it != m_vert_pos.end(); ++it) {
		Vector3D tmp = cMatrix.v_post_multiply(*it);
		*it = Point3D_t(tmp.vector());
	}

	// transform normal
	for(std::vector<Point3D_t>::iterator it = m_normals.begin(); it != m_normals.end(); ++it) {
		Vector3D tmp = cMatrix.v_post_multiply(*it);
		*it = Point3D_t(tmp.vector());
	}
}

void Geometry::show_off(void)
{
	std::cout << "=== SHOW OFF START ===" << std::endl;
	std::cout << "Class name: Geometry" << std::endl;
	std::cout << "Num Vertex: " << m_vertices.size() << std::endl; 
	std::cout << "Num face: " << m_faces.size() << std::endl;
	std::cout << "Has TCoord: " << (has_tcoord() ? "TRUE" : "FALSE") << std::endl;
	std::cout << "Has Normal: " << (has_normal() ? "TRUE" : "FALSE") << std::endl;
	std::cout << "Bounding Box: [" << m_bounding_box.from[0] << ", " << m_bounding_box.to[0] << "] ["
		<< m_bounding_box.from[1] << ", " << m_bounding_box.to[1] << "] ["
		<< m_bounding_box.from[2] << ", " << m_bounding_box.to[2] << "]" << std::endl;
	std::cout << "=== SHOW OFF END ===" << std::endl;
}

bool Geometry::planar_cut(Geometry& g1, Geometry& g2, const Point3D_t& origin, const Vector3D_t& normal)
{
	const int vertex_num = (int)m_vertices.size();
	std::vector<int> front_index_map(vertex_num, -1);
	std::vector<int> back_index_map(vertex_num, -1);
	std::vector<bool> is_front_half(vertex_num);
	// assign each of the vertex to either of the split geometry
	for(int i = 0; i < vertex_num; ++i) {
		is_front_half[i] = ((m_vert_pos[i] - origin).dot(normal) >= 0.f);
		Geometry& target_geom = (is_front_half[i]) ? g1 : g2;
		std::vector<int>& target_index_map = (is_front_half[i]) ? front_index_map : back_index_map;
		SVertex new_vert = target_geom.insert_vertex(m_vert_pos[i].vector());
		target_index_map.at(i) = new_vert.m_index;
	}
	
	typedef struct SEdgeCut_{
		int v1;
		int v2;
		Vector3D cut_point;
		int registered_id;
		SEdgeCut_(): v1(-1), v2(-1) {}
		SEdgeCut_(const int a, const int b): v1(std::min<int>(a, b)), v2(std::max<int>(a, b)) {}
	} SEdgeCut;
	struct less_cmp {
		bool operator() (const SEdgeCut& lhs, const SEdgeCut& rhs) const {
			return (lhs.v1 < rhs.v1) || (lhs.v1 == rhs.v1 && lhs.v2 < rhs.v2);
		}
	};
	std::set<SEdgeCut, less_cmp> front_edge_cuts;
	std::set<SEdgeCut, less_cmp> back_edge_cuts;

	// assign each face to either of the split geometry
	for(int face_index = 0; face_index < (int)m_faces.size(); ++face_index)
	{
		const SFace& face = m_faces[face_index];
		std::vector<int> front_verts;
		std::vector<int> back_verts;
		for(ConstIntIterType it = face.m_vertex_id.begin(); it != face.m_vertex_id.end(); ++it)
		{
			if(is_front_half.at(*it)) {
				front_verts.push_back(*it);
			} else {
				back_verts.push_back(*it);
			}
		}
		if(front_verts.empty() || back_verts.empty())
		{
			// add face to either of the two
			Geometry& target_geom = (back_verts.empty()) ? g1 : g2;
			std::vector<int>& target_index_map = (back_verts.empty()) ? front_index_map : back_index_map;

			std::vector<int> f; // point id
			std::vector<int> t; // texture id
			std::vector<int> n; // normal id

			for(ConstIntIterType it = face.m_vertex_id.begin(); it != face.m_vertex_id.end(); ++it)
			{
				const int v_index_on_target = target_index_map.at(*it);
				f.push_back(v_index_on_target);
				t.push_back(v_index_on_target);
				n.push_back(v_index_on_target);
			}
			SFace nface = target_geom.insert_face(f, t, n);
		} else {
			// cut the face into halves
			std::vector<SEdgeCut> cut_points;
			std::set<int> duplicated_vert_id;
			for(int j = 0; j < 3; ++j) {
				const int j_next = (j + 1) % 3;
				if(is_front_half.at(face.m_vertex_id[j]) != is_front_half.at(face.m_vertex_id[j_next])) { // should be an intersect
					const Vector3D A = m_vert_pos.at(face.m_vertex_id[j]);
					const Vector3D B = m_vert_pos.at(face.m_vertex_id[j_next]);
					const float t = (origin - A).dot(normal) / (B - A).dot(normal);
					if(!(EQUAL_FLT(t, 0) || EQUAL_FLT(t, 1))) {
						SEdgeCut cut(face.m_vertex_id[j], face.m_vertex_id[j_next]);
						cut.cut_point = A + Vector3D(t, t, t) * (B - A);
						cut_points.push_back(cut);
					} else {
						if(EQUAL_FLT(t, 0)) {
							duplicated_vert_id.insert(face.m_vertex_id[j]);
						} else {
							duplicated_vert_id.insert(face.m_vertex_id[j_next]);
						}
					}
				}
			}
			if(2 < cut_points.size()) {
				ERR_MSG("Cannot have more then 2 intersection points");
			}

			// add any duplicated vertex to the other side
			for(std::set<int>::iterator it = duplicated_vert_id.begin(); it != duplicated_vert_id.end(); ++it)
			{
				Geometry& target_geom = (is_front_half.at(*it)) ? g2 : g1;
				std::vector<int>& target_index_map = (is_front_half.at(*it)) ? back_index_map : front_index_map;
				std::vector<int>& new_vert_id = (is_front_half.at(*it)) ? back_verts : front_verts;
				SVertex new_vert = target_geom.insert_vertex(m_vert_pos[*it].vector());
				target_index_map.at(*it) = new_vert.m_index;
				new_vert_id.push_back(*it);
			}

			// add cut points to both side
			for(int sw = 0; sw < 2; ++sw)
			{
				Geometry& target_geom = (0 == sw) ? g1 : g2;
				std::vector<int>& target_index_map = (0 == sw) ? front_index_map : back_index_map;
				std::vector<int>& new_vert_id = (0 == sw) ? front_verts : back_verts;
				std::set<SEdgeCut, less_cmp>& edge_cuts = (0 == sw) ? front_edge_cuts : back_edge_cuts;
				for(std::vector<SEdgeCut>::iterator cit = cut_points.begin(); cit != cut_points.end(); ++cit)
				{
					if(edge_cuts.find(*cit) == edge_cuts.end())
					{
						SVertex new_vert = target_geom.insert_vertex(cit->cut_point.vector());
						new_vert_id.push_back((int)target_index_map.size());
						target_index_map.push_back(new_vert.m_index);
						cit->registered_id = new_vert_id.back();
						edge_cuts.insert(*cit);
					} else {
						std::set<SEdgeCut, less_cmp>::iterator found_it = edge_cuts.find(*cit);
						new_vert_id.push_back(found_it->registered_id);
					}
				}
				
				if(3 > new_vert_id.size() || 4 < new_vert_id.size()) {
					// if a edge of the original triangle conside with the cut plane, set the edge as boundary
					if(2 == new_vert_id.size()) {
						target_geom.mark_vert_as_boundary(
							target_index_map.at(new_vert_id[0]),
							target_index_map.at(new_vert_id[1])
							);
					}
					continue;
				}
				for(int offset = 0; offset <2; ++offset) {
					if((int)new_vert_id.size() < offset + 3) break;
					std::vector<int> f; // point id
					std::vector<int> t; // texture id
					std::vector<int> n; // normal id
					for(int k = offset; k < offset + 3; ++k) {
						const int v_index_on_target = target_index_map.at(new_vert_id[k]);
						f.push_back(v_index_on_target);
						t.push_back(v_index_on_target);
						n.push_back(v_index_on_target);
					}
					SFace nface = target_geom.insert_face(f, t, n);
					if(offset + 3 == (int)new_vert_id.size()) 
					{
						target_geom.mark_vert_as_boundary(nface.m_vertex_id[1],nface.m_vertex_id[2]);
					}
				}
			}
		}
	}
	
	return (g1.validate() && g2.validate());
}

bool Geometry::export_to_obj(const std::string& file_name, const int id_offset)
{
	std::fstream fs;
	fs.open(file_name.c_str(), std::ios_base::out);
	if( !fs.is_open() ) {
		false;
	}
	for(std::vector<Point3D_t>::iterator it = m_vert_pos.begin(); it != m_vert_pos.end(); ++it)
	{
		fs << "v " << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << std::endl;
	}
	for(std::vector<Vector3D_t>::iterator it = m_normals.begin(); it != m_normals.end(); ++it)
	{
		fs << "v " << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << std::endl;
	}
	for(std::vector<TCoord_t>::iterator it = m_tcoords.begin(); it != m_tcoords.end(); ++it)
	{
		fs << "v " << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << std::endl;
	}

	const bool b_has_tcoord = has_tcoord();
	const bool b_has_normal = has_normal();
	for(std::vector<SFace>::iterator it = m_faces.begin(); it != m_faces.end(); ++it)
	{
		fs << "f";
		for(int i = 0; i < (int)it->m_vertex_id.size(); ++i) {
			fs << " " << it->m_vertex_id[i] + id_offset;
			if(b_has_tcoord) {
				fs << "/" <<it->m_texure_id[i] + id_offset;
			}
			if(b_has_normal) {
				fs << "/" <<it->m_normal_id[i] + id_offset;
			}
		}
		fs << std::endl;
	}
	fs.close();
	return true;
}

Point3D_t Geometry::get_geometry_center(void)
{
	Point3D_t sum(0, 0, 0);
	for(int i = 0; i < (int)m_vert_pos.size(); ++i)
	{
		sum = sum + m_vert_pos[i];
	}
	if(0 < m_vert_pos.size()) {
		for(int i = 0; i < 3; ++i) sum[i] /= m_vert_pos.size();
	}
	return sum;
}

std::vector<int> Geometry::get_longest_boundary_chain()
{
	int boundary_vert_num = 0;
	const int N = (int)m_vertices.size();
	for(int i = 0; i < N; ++i) {
		boundary_vert_num += m_vertices[i].m_is_boundary;
	}

	std::vector<int> visited_verts(N, false);
	std::vector<int> ret;
	size_t longest_length = ret.size();
	
	while(1) {
		std::vector<int> boundary_chain;
		// look for a starting point for a new chain
		for(int i = 0; i < N; ++i) {
			if(m_vertices[i].m_is_boundary && !visited_verts[i]) {
				boundary_chain.push_back(i);
				visited_verts[i] = true;
				break;
			}
		}

		if(boundary_chain.empty()) {
			// we are done
			break;
		}

		// grow the chain
		while(1)
		{
			SVertex v = m_vertices.at(boundary_chain.back());
			
			bool found_next = false;
			for(IntIterType it = v.m_boundary_partner.begin(); it != v.m_boundary_partner.end(); ++it)
			{
				if(!visited_verts.at(*it)) {
					found_next = true;
					boundary_chain.push_back(*it);
					visited_verts[*it] = true;
					break;
				}
			}
			if(!found_next) break; // the chain cannot grow
		}

		if(longest_length < boundary_chain.size()) {
			longest_length = boundary_chain.size();
			ret = boundary_chain;
			INFO_MSG("New Chain : " << boundary_chain.size());
		}
	}

	INFO_MSG("boundary num = " << ret.size() << "/ " << boundary_vert_num);
	INFO_MSG("First = " << ret.front());
	INFO_MSG("Last = " << ret.back());
	return ret; 
}

Geometry Geometry::make_flatten(void)
{
	std::vector<int> boundary_vert = get_longest_boundary_chain();
	
	const int K = (int)boundary_vert.size();
	
	if(K < 3) ERR_MSG("Should not happen");

	std::vector<float> alphas(K, 0.f); // sum of triangle angles
	float sum_of_sum = 0.f;
	for(int i = 0; i < K; ++i) {
		SVertex v = m_vertices.at(boundary_vert[i]);
		Vector3D curr_pos = m_vert_pos.at(boundary_vert[i]); 
		for(ConstIntIterType it = v.m_adjacent_face_id.begin(); it != v.m_adjacent_face_id.end(); ++it)
		{
			SFace face = m_faces.at(*it);
			Vector3D pos[2];
			int ci = 0;
			for(ConstIntIterType it2 = face.m_vertex_id.begin(); it2 != face.m_vertex_id.end(); ++it2)
			{
				if(*it2 != boundary_vert[i]) {
					pos[ci++] = m_vert_pos.at(*it2); 
				}
				if (2 == ci) break;
			}
			Point3D_t vec_1 = pos[0] - curr_pos; 
			Point3D_t vec_2 = pos[1] - curr_pos; 

			vec_1.normalize();
			vec_2.normalize();
			alphas[i] += std::acos(vec_1.dot(vec_2));
			//INFO_MSG("alpha = " << std::acos(vec_1.dot(vec_2)) * 180 / M_PI);
		}
		//INFO_MSG("sum = " << alphas[i] * 180 / M_PI);
		sum_of_sum += alphas[i];		
	}

	std::vector<float> betas(K, 0.f); // normalized angles
	for(int i = 0; i < (int)K; ++i) {
		betas[i] = ((int)K - 2) * M_PI * alphas[i] / sum_of_sum; 
		//INFO_MSG("beta = " << betas[i] * 180 / M_PI << " alpha = " << alphas[i] * 180 / M_PI << " sum_of_sum = " << sum_of_sum * 180 / M_PI);
	}

	make_convex_hull_angles(betas);
	float sum_beta = 0;
	for(int i = 0; i < (int)K; ++i) {
		//INFO_MSG("beta = " << betas[i] * 180 / M_PI << " alpha = " << alphas[i] * 180 / M_PI << " sum_of_sum = " << sum_of_sum * 180 / M_PI);
		sum_beta += betas[i];
	}
	INFO_MSG("sum beta = " << sum_beta * 180 / M_PI << ", sum alpha = " << sum_of_sum * 180 / M_PI);

	std::vector<Point3D_t> U(K);
	for(int i = 0; i < (int)K; ++i) {
		const float ulength = (m_vert_pos[boundary_vert[i]] - m_vert_pos[boundary_vert[(i + 1) % K]]).get_length();
		if(0 == i) {
			U[i] = Point3D_t(ulength, 0, 0);
		} else {
			// rotate by beta[i]
			const float cosine = std::cos(M_PI - betas[i]);
			const float sine = std::sin(betas[i]);
			U[i][0] = U[i-1][0] * cosine - U[i-1][1] * sine;
			U[i][1] = U[i-1][0] * sine   + U[i-1][1] * cosine;
			U[i][2] = 0;
			U[i].normalize();
			U[i] *= Point3D_t(ulength, ulength, ulength);
		}
	}

	float sum_u_length = 0;
	Point3D_t sum_u(0, 0, 0);
	for(int i = 0; i < (int)K; ++i) {
		sum_u_length += U[i].get_length();
		sum_u += U[i];
	}

	std::vector<Point3D_t> S(K);
	Point3D_t sum_s(0, 0, 0);
	for(int i = 0; i < (int)K; ++i) {
		const float r = U[i].get_length() / sum_u_length;
		S[i] = U[i] - sum_u * Point3D_t(r, r, r);
		sum_s += S[i];
	}
	sum_s.dump();

	std::vector<Vector3D> new_boundary_pos(K);
	Vector3D old_center(0, 0, 0);
	Vector3D new_center(0, 0, 0);
	for(int i = 0; i < (int)K; ++i) {
		new_boundary_pos.at(i) = (i == 0) ?  m_vert_pos.at(boundary_vert[i]) : new_boundary_pos.at(i - 1) + S[i - 1];
		old_center += m_vert_pos.at(boundary_vert[i]);
		new_center += new_boundary_pos.at(i);
	}
	
	// transform the new verts so that the center does not change
	Vector3D displ = (old_center - new_center) * Vector3D(1.f/K, 1.f/K ,1.f/K);

	std::map<int, Vector3D> flatten_map;
	for(int i = 0; i < K; ++i)
	{
		flatten_map.insert(std::make_pair(boundary_vert[i], new_boundary_pos[i] + displ));
		flatten_map[boundary_vert[i]][2] = 0.f;
	}
	solve_for_internal_vertices(flatten_map);

	Geometry ret_geom;
	for(std::map<int, Vector3D>::iterator it = flatten_map.begin(); it != flatten_map.end(); ++it)
	{
		ret_geom.insert_vertex(it->second.vector());
	}
	for(FaceArray::iterator it = m_faces.begin(); it != m_faces.end(); ++it)
	{
		ret_geom.insert_face(it->m_vertex_id, it->m_texure_id, it->m_normal_id);
	}
	return ret_geom;
}

bool Geometry::make_convex_hull_angles(std::vector<float>& b)
{
	const int N = (int)b.size();
	float sum_before = 0.f;
	for(int i = 0; i < N; ++i)
	{
		sum_before += b[i];
	}
	INFO_MSG("Before sum = " << sum_before * 180 / M_PI);

	while(true)
	{
		int over_saturated_num = 0;
		float over_saturated_sum = 0;
		float non_saturated_sum = 0;
		for(int i = 0; i < N; ++i)
		{
			if(M_PI < b[i])
			{
				++over_saturated_num;
				over_saturated_sum += b[i] - M_PI;
				b[i] = M_PI;
			} else if(M_PI > b[i]) {
				non_saturated_sum += b[i];
			}
		}
		
		if(over_saturated_num == 0) {
			break;
		}
		
		float after_sum = 0;
		float debug = 0.f;
		for(int i = 0; i < N; ++i)
		{
			if(M_PI > b[i])
			{
				b[i] += over_saturated_sum * b[i] / non_saturated_sum;
				debug += b[i] / non_saturated_sum;
			}
			after_sum += b[i];
		}
		INFO_MSG("After sum = " << after_sum / (N - 2) * 180 / M_PI << ", debug = " << debug << "over saturated num = " << over_saturated_num);
	} 
	return true;
}

std::vector<int> Geometry::list_adjacent_verts(const int curr_vert_id)
{
	SVertex curr_vert = m_vertices.at(curr_vert_id);
	std::vector<int> adj_faces = curr_vert.m_adjacent_face_id;
	std::set<int> unique_adj_vert;
	for(IntIterType it = adj_faces.begin(); it != adj_faces.end(); ++it)
	{
		std::vector<int> adj_verts = m_faces.at(*it).m_vertex_id;
		for(IntIterType vit = adj_verts.begin(); vit != adj_verts.end(); ++vit)
		{
			if(*vit != curr_vert_id) {
				unique_adj_vert.insert(*vit);
			}
		}
	}

	return std::vector<int>(unique_adj_vert.begin(), unique_adj_vert.end());
}

std::pair<float, float> Geometry::calc_edge_weight(int i, int j)
{
	bool debug = false;
	if((1 == i && 2668 == j) || (5 == i && 4 == j))
	{
		debug = true;
	}
	Vector3D vi_pos = m_vert_pos.at(i);
	Vector3D vj_pos = m_vert_pos.at(j);
	Vector3D eij = (vj_pos - vi_pos); 
	const double edge_len = eij.get_length();
	eij.normalize();
	Vector3D eji(-eij[0], -eij[1], -eij[2]);

	SVertex vi = m_vertices.at(i);
	SVertex vj = m_vertices.at(j);
	FaceArray common_faces;
	for(IntIterType it = vi.m_adjacent_face_id.begin(); it != vi.m_adjacent_face_id.end(); ++it)
	{
		SFace face = m_faces.at(*it);
		if(std::find(face.m_vertex_id.begin(), face.m_vertex_id.end(), j) != face.m_vertex_id.end())
		{
			common_faces.push_back(face);
			if(debug) {
				face.show_off();
			}
		}
	}
	if(common_faces.size() < 1 || common_faces.size() > 2) {
		//ERR_MSG("Bad common face number: " << common_faces.size() << " I = " << i << " j = " << j);
	}
	float tan_ij = 0.f, tan_ji = 0.f;
	for(FaceArray::iterator it = common_faces.begin(); it != common_faces.end(); ++it)
	{
		int other_index = -1;
		for(int k = 0; k < 3; ++k) {
			if(i != it->m_vertex_id[k] && j != it->m_vertex_id[k]) {
				other_index = it->m_vertex_id[k];
				break;
			}
		}
		if(-1 == other_index)
		{
			ERR_MSG("Hmm");
		}
		for(int sw = 0; sw < 2; ++sw)
		{
			Vector3D ex = m_vert_pos.at(other_index) - ((sw == 0)  ? vi_pos : vj_pos);
			ex.normalize();
			const float cosine = ex.dot(((sw == 0)  ? eij : eji));
			
			const float tan_val = std::tan(0.5 * std::acos(cosine));
			if(debug) {
				ex.dump();
				INFO_MSG( " consine = " << cosine);
				INFO_MSG( " tan_val = " << tan_val);
			}
			if(0 == sw) tan_ij += tan_val;
			else tan_ji += tan_val;
		}
	}
	if(debug)
	{
		INFO_MSG("tan ij = " << tan_ij << ", ji = " << tan_ji << ", edge = " << edge_len);
	}
	return std::make_pair(tan_ij / edge_len, tan_ji / edge_len);
}

bool Geometry::solve_for_internal_vertices(std::map<int, Vector3D>& flatten_map)
{
	typedef Eigen::SparseMatrix<float> SpMat; // declares a column-major sparse matrix type of float
	typedef Eigen::Triplet<float> T;
	typedef Eigen::VectorXf SpVec;

	const int num_vert = (int)m_vertices.size(); 
	SpMat weight_matrix(num_vert, num_vert);
	// build weight matrix
	{
		weight_matrix.setZero();
		std::vector<T> tripletList;
		tripletList.reserve(num_vert * 4);
		for(int i = 0; i < num_vert; ++i)
		{
			std::vector<int> adj_verts = list_adjacent_verts(i);
			for(IntIterType it = adj_verts.begin(); it != adj_verts.end(); ++it)
			{
				if(0 == weight_matrix.coeff(i, *it))
				{
					std::pair<float, float> ww = calc_edge_weight(i, *it);
					weight_matrix.coeffRef(i, *it) = ww.first;
					weight_matrix.coeffRef(*it, i) = ww.second;
				}
			}
		}
	}
	
	// build the system
	{
		Eigen::MatrixXf D(num_vert, num_vert);

		SpMat A(num_vert, num_vert);
		SpVec Bx(num_vert), By(num_vert);
		std::vector<T> tripletList;
		tripletList.reserve(num_vert * 4);
		for(int i = 0; i < num_vert; ++i)
		{
			tripletList.push_back(T(i, i, 1));
			D.coeffRef(i, i) = 1;
			if(m_vertices.at(i).m_is_boundary) {
				if(flatten_map.find(i) == flatten_map.end()) {
					ERR_MSG(i << " Should be ready in the flatten map");
				}
				Bx(i) = flatten_map[i][0];
				By(i) = flatten_map[i][1];
			} else {
				std::vector<int> adj_verts = list_adjacent_verts(i);
				float weight_sum = 0.f;
				for(IntIterType it = adj_verts.begin(); it != adj_verts.end(); ++it)
				{
					weight_sum += weight_matrix.coeff(i, *it);
				}
				const float W = 1.f / weight_sum;
				for(IntIterType it = adj_verts.begin(); it != adj_verts.end(); ++it)
				{
					const float coeff = /*-1.f / adj_verts.size();*/ -weight_matrix.coeff(i, *it) * W;
					tripletList.push_back(T(i, *it, coeff));
					Bx(i) = 0;
					By(i) = 0;

					D.coeffRef(i, *it) = coeff;
				}
			}
		}
		A.setFromTriplets(tripletList.begin(), tripletList.end());
		
		std::fstream fs;
		fs.open("sys_matrix.txt", std::ios_base::out);
		if( !fs.is_open() ) {
			false;
		}
		fs << weight_matrix << std::endl << "=====" << std::endl;
		fs << A << std::endl << "=====" << std::endl;
		fs << D << std::endl << "=====" << std::endl;
		fs << Bx << std::endl << "=====" << std::endl;
		fs << By << std::endl;
		fs.close();

		INFO_MSG("Solving ...");
		Eigen::SimplicialLDLT<SpMat> solver;
		Eigen::ColPivHouseholderQR<Eigen::MatrixXf> dense_solver; dense_solver.compute(D);

		solver.compute(A);
		if(solver.info()!= Eigen::Success) {
			ERR_MSG("Failed Solving the system");
		}

		SpVec X = solver.solve(Bx);
		if(solver.info()!= Eigen::Success) {
			ERR_MSG("Failed Solving the system");
		}

		SpVec Y = solver.solve(By);
		if(solver.info()!= Eigen::Success) {
			ERR_MSG("Failed Solving the system");
		}

		Eigen::VectorXf DX = dense_solver.solve(Bx);
		Eigen::VectorXf DY = dense_solver.solve(By);

		for(int i = 0; i < num_vert; ++i)
		{
			//flatten_map.insert(std::make_pair(i, Vector3D(X(i), Y(i), 0.f)));
			flatten_map.insert(std::make_pair(i, Vector3D(DX(i), DY(i), 0.f)));
		}

		// test 
		for(int i = 0; i < num_vert; ++i)
		{
			float result = 0;
			for(int j = 0; j < num_vert; ++j)
			{
				result += A.coeff(i, j) * X(j);
			}
			//INFO_MSG("Result at " << i << " = " << result << ", expected " << Bx(i) << ". X = " << X(i) << ", Y = " << Y(i));
		}
	}
	return true;
}