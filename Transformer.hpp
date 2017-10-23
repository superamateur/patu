#ifndef __TRANSFORMER_H
#define __TRANSFORMER_H
#include "func_defs.h"
#include "Matrix.hpp"
#include "Vector3D.hpp"

class Transformer
{
public:
	Transformer(void);
	Transformer(const Matrix& mat);
	virtual ~Transformer(void);

	void concat_transform(const Transformer& other) { m_matrix.post_multiply(other.get_matrix()); }
	const Matrix& get_matrix(void) const { return m_matrix; }
private:
	Matrix m_matrix;
};
#endif //__TRANSFORMER_H