#ifndef __COORDSYS_H
#define __COORDSYS_H

#include "func_defs.h"
#include "Transformer.hpp"

class CoordSys
{
public:
	CoordSys(void);
	CoordSys(const Vector3D& i, const Vector3D& j, const Vector3D& k, const Vector3D& o);
	virtual ~CoordSys(void);

	const Vector3D& get_i(void) { return m_i; }
	const Vector3D& get_j(void) { return m_j; }
	const Vector3D& get_k(void) { return m_k; }

	Transformer make_transformer_from(const CoordSys& from) const; 
	Transformer make_transformer_to(const CoordSys& to)     const;
	void transform(const Transformer& tfm);

private:
	Vector3D m_i;
	Vector3D m_j;
	Vector3D m_k;
	Vector3D m_o; 
};
#endif //__COORDSYS_H