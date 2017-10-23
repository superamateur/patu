#ifndef __OBJ_READER_H
#define __OBJ_READER_H

#include "Reader.hpp"

#define MAX_LINE 1024

class OBJReader : public Reader
{
public:
	OBJReader(void);
	OBJReader(const char* file_name);
	virtual ~OBJReader(void);
	virtual kReturnCode read(Geometry& geom);
};
#endif