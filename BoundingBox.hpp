#pragma once
#include "func_defs.h"
#include "Vector3D.hpp"

class Ray;
class BoundingBox
{
public:
	BoundingBox(void);
	virtual ~BoundingBox(void);

	bool intersect(const Ray* iray, float& min, float& max);

private:
	Vector3D m_min;
	Vector3D m_max;
};
