#pragma once
#include "func_defs.h"

class lbvh
{
public:
	lbvh(void);
	virtual ~lbvh(void);

private:
	// expand a 10 bit integer to a 30 bits integer
	unsigned int expand_bits(unsigned int v);

	// generate Morton code for a 3D point whose coordinates are inside a unit cube
	unsigned int morton_3d(float x, float y ,float z);
};
