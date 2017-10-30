#include "Matrix.hpp"

#define ELEM_GET(row, col) m_elems[row * 4 + col]
#define ELEM_SET(row, col, val) m_elems[row * 4 + col] = val;

Matrix::Matrix()
{
	// perform initialization
	identity();
}

Matrix& Matrix::operator  =(const Matrix& other)
{
	set_elements(other.get_elements());
	return *this;
}

Matrix::~Matrix()
{}

void Matrix::set_element(const int i, const int j, const float e)
{
	m_elems[i * 4 + j] = e;
}

void Matrix::set_elements(const float* e)
{
	std::copy(e, e + 16, m_elems);
}

float Matrix::get_element(int i, int j) const
{
  return m_elems[i * 4 + j];
}

const float* Matrix::get_elements(void) const
{
	return m_elems;
}

float* Matrix::get_elements(void)
{
	return m_elems;
}

void Matrix::identity(void)
{
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			m_elems[i * 4 + j] = (i==j)? 1.f: 0.f;
		}
	}
}

void Matrix::zero(void)
{
	for(int i=0; i<16; i++) m_elems[i] = 0;
}

Matrix Matrix::post_multiply(const Matrix& opr) const
{
	Matrix ret;
	ret.zero();
	float* ret_elem = ret.get_elements();
	const float* opr_elem = opr.get_elements();

	for(size_t row = 0; row < 4; ++row) {
		for(size_t col = 0; col < 4; ++col) {
			for(size_t runner = 0; runner < 4; ++runner) {
				*ret_elem += m_elems[row * 4 + runner] * opr_elem[runner * 4 + col];
			}
			++ret_elem;
		}
	}
	return ret;
}

void Matrix::post_multiply(const Matrix& opr)
{
	float* ret_elem = new float[16];
	const float* opr_elem = opr.get_elements();

	for(size_t row = 0; row < 4; ++row) {
		for(size_t col = 0; col < 4; ++col) {
			*ret_elem = 0;
			for(size_t runner = 0; runner < 4; ++runner) {
				*ret_elem += m_elems[row * 4 + runner] * opr_elem[runner * 4 + col];
			}
			++ret_elem;
		}
	}
	set_elements(ret_elem);
	delete[] ret_elem;
}

Vector3D  Matrix::v_post_multiply(const Vector3D& opr) const
{
	Vector3D product;
	for(size_t row = 0; row < 3; ++row) {
		for(size_t col = 0; col < 3; ++col) {
			product[row] += ELEM_GET(row, col) * opr[row];
		}
		product[row] += ELEM_GET(row, 3);
	}
	return product;
}

Matrix Matrix::pre_multiply(const Matrix& opr) const
{
	Matrix ret;
	ret.zero();
	float* ret_elem = ret.get_elements();
	const float* opr_elem = opr.get_elements();

	for(size_t row = 0; row < 4; ++row) {
		for(size_t col = 0; col < 4; ++col) {
			for(size_t runner = 0; runner < 4; ++runner) {
				*ret_elem += opr_elem[row * 4 + runner] * m_elems[runner * 4 + col];
			}
			++ret_elem;
		}
	}
	return ret;
}

void Matrix::pre_multiply(const Matrix& opr)
{
	float* ret_elem = new float[16];
	const float* opr_elem = opr.get_elements();

	for(size_t row = 0; row < 4; ++row) {
		for(size_t col = 0; col < 4; ++col) {
			*ret_elem = 0;
			for(size_t runner = 0; runner < 4; ++runner) {
				*ret_elem += opr_elem[row * 4 + runner] * m_elems[runner * 4 + col];
			}
			++ret_elem;
		}
	}
	set_elements(ret_elem);
	delete[] ret_elem;
}

float Matrix::get_det() const
{
	return (ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 0) - ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 0) + ELEM_GET(0, 1) * ELEM_GET(1, 3)  * ELEM_GET(2, 2) * ELEM_GET(3, 0) + 
			ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 1) + ELEM_GET(0, 2) * ELEM_GET(1, 3)  * ELEM_GET(2, 0) * ELEM_GET(3, 1) + 
			ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 1) - ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 1) - ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 1) + ELEM_GET(0, 0) * ELEM_GET(1, 2)  * ELEM_GET(2, 3) * ELEM_GET(3, 1) + 
			ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 2) - ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 2) - ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 2) + ELEM_GET(0, 0) * ELEM_GET(1, 3)  * ELEM_GET(2, 1) * ELEM_GET(3, 2) + 
			ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 2) - ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 2) - ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 3) + ELEM_GET(0, 1) * ELEM_GET(1, 2)  * ELEM_GET(2, 0) * ELEM_GET(3, 3) + 
			ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 3) - ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 3) - ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 3) + ELEM_GET(0, 0) * ELEM_GET(1, 1)  * ELEM_GET(2, 2) * ELEM_GET(3, 3));
}

Vector3D Matrix::get_column(const size_t c) const
{
	return Vector3D(ELEM_GET(0, c), ELEM_GET(1, c), ELEM_GET(2, c));
}

Matrix Matrix::get_inverse(void) const
{
	Matrix ret;
	const float det = get_det();

	ret.set_element(0, 0, (ELEM_GET(1, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 1) - ELEM_GET(1, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 1) + ELEM_GET(1, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 2) - 
			   ELEM_GET(1, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 2) - ELEM_GET(1, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 3) + ELEM_GET(1, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det);
	ret.set_element(0,1,(ELEM_GET(0, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 1) - ELEM_GET(0, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 1) - ELEM_GET(0, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 2) + 
			   ELEM_GET(0, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 2) + ELEM_GET(0, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 3) - ELEM_GET(0, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det);
	ret.set_element(0,2,(ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(3, 1) - ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(3, 1) + ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(3, 2) - 
			   ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(3, 2) - ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(3, 3) + ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(3, 3))/det);
	ret.set_element(0,3,(ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(2, 1) - ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(2, 1) - ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(2, 2) + 
			   ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(2, 2) + ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(2, 3) - ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(2, 3))/det);
	ret.set_element(1,0,(ELEM_GET(1, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 0) - ELEM_GET(1, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(1, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 2) + 
			   ELEM_GET(1, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 2) + ELEM_GET(1, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 3) - ELEM_GET(1, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det);
	ret.set_element(1,1,(ELEM_GET(0, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 0) + ELEM_GET(0, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 2) - 
			   ELEM_GET(0, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 2) - ELEM_GET(0, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 3) + ELEM_GET(0, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det);
	ret.set_element(1,2,(ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(3, 0) - ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(3, 2) + 
			   ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(3, 2) + ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(3, 3) - ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(3, 3))/det);
	ret.set_element(1,3,(ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(2, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(2, 0) + ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(2, 2) - 
			   ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(2, 2) - ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(2, 3) + ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(2, 3))/det);
	ret.set_element(2,0,(ELEM_GET(1, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(1, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 0) + ELEM_GET(1, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 1) - 
			   ELEM_GET(1, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 1) - ELEM_GET(1, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 3) + ELEM_GET(1, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 3))/det);
	ret.set_element(2,1,(ELEM_GET(0, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 0) - ELEM_GET(0, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 1) + 
			   ELEM_GET(0, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 1) + ELEM_GET(0, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 3) - ELEM_GET(0, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 3))/det);
	ret.set_element(2,2,(ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(3, 0) + ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(3, 1) - 
			   ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(3, 1) - ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(3, 3) + ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(3, 3))/det);
	ret.set_element(2,3,(ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(2, 0) - ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(2, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(2, 1) + 
			   ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(2, 1) + ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(2, 3) - ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(2, 3))/det);
	ret.set_element(3,0,(ELEM_GET(1, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 0) - ELEM_GET(1, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 0) - ELEM_GET(1, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 1) + 
			   ELEM_GET(1, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 1) + ELEM_GET(1, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 2) - ELEM_GET(1, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 2))/det);
	ret.set_element(3,1,(ELEM_GET(0, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 0) - ELEM_GET(0, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 0) + ELEM_GET(0, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 1) - 
			   ELEM_GET(0, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 1) - ELEM_GET(0, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 2) + ELEM_GET(0, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 2))/det);
	ret.set_element(3,2,(ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(3, 0) - ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(3, 0) - ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(3, 1) + 
			   ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(3, 1) + ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(3, 2) - ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(3, 2))/det);
	ret.set_element(3,3,(ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(2, 0) - ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(2, 0) + ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(2, 1) - 
			   ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(2, 1) - ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(2, 2) + ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(2, 2))/det);
	return ret;
}

void Matrix::inverse(void)
{
  float det=get_det();
  float tmp[16];

  tmp[0] = (ELEM_GET(1, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 1) - ELEM_GET(1, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 1) + ELEM_GET(1, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 2) - 
	    ELEM_GET(1, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 2) - ELEM_GET(1, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 3) + ELEM_GET(1, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det;
  tmp[1] = (ELEM_GET(0, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 1) - ELEM_GET(0, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 1) - ELEM_GET(0, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 2) + 
	    ELEM_GET(0, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 2) + ELEM_GET(0, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 3) - ELEM_GET(0, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det;
  tmp[2] = (ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(3, 1) - ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(3, 1) + ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(3, 2) - 
	    ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(3, 2) - ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(3, 3) + ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(3, 3))/det;
  tmp[3] = (ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(2, 1) - ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(2, 1) - ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(2, 2) + 
	    ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(2, 2) + ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(2, 3) - ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(2, 3))/det;
  tmp[4] = (ELEM_GET(1, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 0) - ELEM_GET(1, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(1, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 2) + 
	    ELEM_GET(1, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 2) + ELEM_GET(1, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 3) - ELEM_GET(1, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det;
  tmp[5] = (ELEM_GET(0, 2) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(2, 2) * ELEM_GET(3, 0) + ELEM_GET(0, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 2) - 
	    ELEM_GET(0, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 2) - ELEM_GET(0, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 3) + ELEM_GET(0, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 3))/det;
  tmp[6] = (ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(3, 0) - ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(3, 2) + 
	    ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(3, 2) + ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(3, 3) - ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(3, 3))/det;
  tmp[7] = (ELEM_GET(0, 2) * ELEM_GET(1, 3) * ELEM_GET(2, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 2) * ELEM_GET(2, 0) + ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(2, 2) - 
	    ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(2, 2) - ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(2, 3) + ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(2, 3))/det;
  tmp[8] = (ELEM_GET(1, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(1, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 0) + ELEM_GET(1, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 1) - 
	    ELEM_GET(1, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 1) - ELEM_GET(1, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 3) + ELEM_GET(1, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 3))/det;
  tmp[9] = (ELEM_GET(0, 3) * ELEM_GET(2, 1) * ELEM_GET(3, 0) - ELEM_GET(0, 1) * ELEM_GET(2, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(2, 0) * ELEM_GET(3, 1) + 
	    ELEM_GET(0, 0) * ELEM_GET(2, 3) * ELEM_GET(3, 1) + ELEM_GET(0, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 3) - ELEM_GET(0, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 3))/det;
  tmp[10] = (ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(3, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(3, 0) + ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(3, 1) - 
	     ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(3, 1) - ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(3, 3) + ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(3, 3))/det;
  tmp[11] = (ELEM_GET(0, 3) * ELEM_GET(1, 1) * ELEM_GET(2, 0) - ELEM_GET(0, 1) * ELEM_GET(1, 3) * ELEM_GET(2, 0) - ELEM_GET(0, 3) * ELEM_GET(1, 0) * ELEM_GET(2, 1) + 
	     ELEM_GET(0, 0) * ELEM_GET(1, 3) * ELEM_GET(2, 1) + ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(2, 3) - ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(2, 3))/det;
  tmp[12] = (ELEM_GET(1, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 0) - ELEM_GET(1, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 0) - ELEM_GET(1, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 1) + 
	     ELEM_GET(1, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 1) + ELEM_GET(1, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 2) - ELEM_GET(1, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 2))/det;
  tmp[13] = (ELEM_GET(0, 1) * ELEM_GET(2, 2) * ELEM_GET(3, 0) - ELEM_GET(0, 2) * ELEM_GET(2, 1) * ELEM_GET(3, 0) + ELEM_GET(0, 2) * ELEM_GET(2, 0) * ELEM_GET(3, 1) - 
	     ELEM_GET(0, 0) * ELEM_GET(2, 2) * ELEM_GET(3, 1) - ELEM_GET(0, 1) * ELEM_GET(2, 0) * ELEM_GET(3, 2) + ELEM_GET(0, 0) * ELEM_GET(2, 1) * ELEM_GET(3, 2))/det;
  tmp[14] = (ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(3, 0) - ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(3, 0) - ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(3, 1) + 
	     ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(3, 1) + ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(3, 2) - ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(3, 2))/det;
  tmp[15] = (ELEM_GET(0, 1) * ELEM_GET(1, 2) * ELEM_GET(2, 0) - ELEM_GET(0, 2) * ELEM_GET(1, 1) * ELEM_GET(2, 0) + ELEM_GET(0, 2) * ELEM_GET(1, 0) * ELEM_GET(2, 1) - 
	     ELEM_GET(0, 0) * ELEM_GET(1, 2) * ELEM_GET(2, 1) - ELEM_GET(0, 1) * ELEM_GET(1, 0) * ELEM_GET(2, 2) + ELEM_GET(0, 0) * ELEM_GET(1, 1) * ELEM_GET(2, 2))/det;

  set_elements(tmp);
}


Matrix Matrix::get_transpose(void) const
{
	Matrix ret;
	for(size_t row = 0; row < 4; ++row) {
		for(size_t col = 0; col < 4; ++col) {
			ret.set_element(col, row, m_elems[row * 4 + col]);
		}
	}
	return *this;
}

void Matrix::transpose(void)
{
	float tmp[16];
	for(size_t row = 0; row < 4; ++row) {
		for(size_t col = 0; col < 4; ++col) {
			tmp[col * 4 + row] = m_elems[row * 4 + col];
		}
	}
	set_elements(tmp);
}
