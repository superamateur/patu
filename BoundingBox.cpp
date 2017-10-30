#include "BoundingBox.hpp"

BoundingBox::BoundingBox(void)
: m_min(Vector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX))
, m_max(Vector3D(FLT_MAX, FLT_MAX, FLT_MAX))
{
}

BoundingBox::~BoundingBox(void)
{
}

bool BoundingBox::intersect(const Ray* iray, float& min, float& max)
{
	return false;
}