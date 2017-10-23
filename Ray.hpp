#pragma once
#include "Vector3D.hpp"

class Ray
{
public:
	Ray(void);
	virtual ~Ray(void);
	
	Point_t& start(void);
	Vector3D& dir(void);

private:
	Point_t m_origin;
	Vector3D m_direction;
	size_t m_bounds;
	float m_traveled;
};
