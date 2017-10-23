#ifndef __ETU_MATRIX_4X4_H
#define __ETU_MATRIX_4X4_H
#include "func_defs.h"
#include "Vector3D.hpp"

class Matrix {
public:
	Matrix(void);
	Matrix& operator  =(const Matrix& other);
	virtual ~Matrix();
	
	void identity(void);
	void zero(void);
	float get_det(void) const;

	// member access
	void set_element(const int i, const int j, const float e);
	void set_elements(const float* e);
	
	float		 get_element(int i, int j) const;
	const float* get_elements(void) const;
	float*		 get_elements(void);

	// matrix multiplication
	Matrix post_multiply(const Matrix& opr) const;
	void	  post_multiply(const Matrix& opr);
	Matrix pre_multiply(const Matrix& opr) const;
	void	  pre_multiply(const Matrix& opr);

	// vector multiplication
	Vector3D v_post_multiply(const Vector3D& opr) const;
	Vector3D get_column(const size_t c) const;

	// matrix inversion
	Matrix get_inverse(void) const;
	void	  inverse(void);

	// transpose operation
	Matrix get_transpose(void) const;
	void	  transpose(void);

private:
  float m_elems[16];

};

#endif //__ETU_MATRIX_4X4_H
