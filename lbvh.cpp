#include "lbvh.hpp"
#include <math.h>

lbvh::lbvh(void)
{
}

lbvh::~lbvh(void)
{
}

inline unsigned int lbvh::expand_bits(unsigned int v)
{
	v = ( v * 0x00010001u ) & 0xFF0000FFu;
	v = ( v * 0x00000101u ) & 0x0F00F00Fu;
    v = ( v * 0x00000011u ) & 0xC30C30C3u;
    v = ( v * 0x00000005u ) & 0x49249249u;
    return v;
}

inline unsigned int lbvh::morton_3d(float x, float y ,float z)
{
	x = std::min( std::max(x * 1024.f, 0.f), 1023.f);
	y = std::min( std::max(y * 1024.f, 0.f), 1023.f);
	z = std::min( std::max(z * 1024.f, 0.f), 1023.f);
	unsigned int ex_x = expand_bits( static_cast<unsigned int>(x) );
	unsigned int ex_y = expand_bits( static_cast<unsigned int>(y) );
	unsigned int ex_z = expand_bits( static_cast<unsigned int>(z) );
	return ex_x * 4 + ex_y * 2 + ex_z;
}