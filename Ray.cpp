#include "Ray.hpp"

Ray::Ray(void)
: m_bounds(0)
, m_traveled(FLT_0)
{
}

Ray::~Ray(void)
{
}

Point_t& Ray::start(void)
{
	return m_origin;
}

Vector3D& Ray::dir(void)
{
	return m_direction;
}