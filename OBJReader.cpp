#include "OBJReader.hpp"
#include "Token.hpp"

OBJReader::OBJReader(void)
{
}

OBJReader::OBJReader(const char* file_name)
: Reader(file_name)
{
}


OBJReader::~OBJReader(void)
{
}

kReturnCode OBJReader::read(Geometry& geom, const int id_offset)
{
	kReturnCode return_code = RETURN_CODE_SUCCESS;

	std::fstream fs;
	fs.open(m_file_name.c_str(), std::ios_base::in);
	if( !fs.is_open() ) {
		return RETURN_CODE_FAILURE;
	}

	// buffer for reading file
	char buffer[MAX_LINE];
	while(!fs.eof()) {
		fs.getline(buffer, MAX_LINE);
		
		// skip comment
		if(buffer[0] == '#') continue;

		Token _t(buffer);
		std::vector<Token> _sub_t;

		if( 0 == strncmp("vt", buffer, 2) ) { // vertex - texture
			// vt u v
			_t.split_by(" ", _sub_t);
			if( 3 != _sub_t.size() ) {
				return_code = RETURN_CODE_FAILURE;
				break;
			}
			geom.insert_tcoord(_sub_t[1].get_float(), _sub_t[2].get_float());
		} else if( 0 == strncmp("vn", buffer, 2) ) { // vertex - normal
			// vn n0 n1 n2
			_t.split_by(" ", _sub_t);
			
			if( 4 != _sub_t.size() ) {
				return_code = RETURN_CODE_FAILURE;
				break;
			}			
			geom.insert_normal(_sub_t[1].get_float(), _sub_t[2].get_float(), _sub_t[3].get_float());
		} else if( 0 == strncmp("v", buffer, 1) ) { // vertex
			// v x y z
			_t.split_by(" ", _sub_t);
			
			if( 4 != _sub_t.size() ) {
				return_code = RETURN_CODE_FAILURE;
				break;
			}		
			geom.insert_vertex(_sub_t[1].get_float(), _sub_t[2].get_float(), _sub_t[3].get_float());
		} else if( 0 == strncmp("f", buffer, 1) ) { // face: vertex/<optional>texture/<optional>normal
			_t.split_by(" ", _sub_t);
			if( 4 > _sub_t.size() ) { // each face requires at least 3 indice + 1 keyword
				return_code = RETURN_CODE_FAILURE;
				break;
			}

			size_t n_elem = 0;
			std::vector<int> f; // point id
			std::vector<int> t; // texture id
			std::vector<int> n; // normal id
			for(std::vector<Token>::const_iterator it = _sub_t.begin() + 1; it != _sub_t.end(); ++it) {
				std::vector<Token> _sub_sub_t;
				it->split_by("/", _sub_sub_t);
				if(3 < _sub_sub_t.size()) {
					return_code = RETURN_CODE_FAILURE;
					break;
				}
				if( 1 <= _sub_sub_t.size() ) f.push_back(_sub_sub_t[0].get_int() - id_offset);
				if( 2 <= _sub_sub_t.size() ) t.push_back(_sub_sub_t[1].get_int() - id_offset);
				if( 3 == _sub_sub_t.size() ) n.push_back(_sub_sub_t[2].get_int() - id_offset);
			}
			if(!f.empty()) geom.insert_face(f, t ,n);
		}	
	}

	fs.close();
	if( !geom.validate() ) return_code = RETURN_CODE_FAILURE;

	return return_code;
}