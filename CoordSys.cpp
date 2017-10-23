#include "CoordSys.hpp"

CoordSys::CoordSys(void)
{
}

CoordSys::CoordSys(const Vector3D& i, const Vector3D& j, const Vector3D& k, const Vector3D& o)
: m_i(i)
, m_j(j)
, m_k(k)
, m_o(o)
{
}

CoordSys::~CoordSys(void)
{
}