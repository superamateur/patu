#ifndef __READER_H
#define __READER_H

#include "func_defs.h"
#include "errors.h"
#include "Geometry.hpp"

class Reader
{
public:
	Reader(void);
	Reader(const char* file_name);
	virtual ~Reader(void);

	void set_file_name(const char* file_name);

	virtual kReturnCode read(Geometry& geom) = 0;
protected:
	std::string m_file_name;
};
#endif
