#include "Vector3D.hpp"
#include <math.h>

#define set_dirty_flag(shame) m_dirty_flag = shame

Vector3D::Vector3D(void)
: m_dimension(3)
, m_length(0.f)
, m_dirty_flag(false)
{
	m_vector.resize(m_dimension);
	m_vector[0] = 0.f;
	m_vector[1] = 0.f;
	m_vector[2] = 0.f;
}

Vector3D::Vector3D(const float x, const float y, const float z)
: m_dimension(3)
, m_length(0.f)
, m_dirty_flag(true)
{
	m_vector.resize(m_dimension);
	m_vector[0] = x;
	m_vector[1] = y;
	m_vector[2] = z;
}

Vector3D::Vector3D(const float* v)
: m_dimension(3)
, m_length(0.f)
, m_dirty_flag(true)
{
	m_vector.resize(m_dimension);
	m_vector[0] = v[0];
	m_vector[1] = v[1];
	m_vector[2] = v[2];
}

Vector3D::~Vector3D(void)
{
}

Vector3D& Vector3D::operator  =(const Vector3D& other)
{
	m_vector = other.get_vector();
	set_dirty_flag(true);
	return *this;
}

Vector3D& Vector3D::operator +=(const Vector3D& other)
{
	for(size_t i = 0; i < m_dimension; ++i) {
		m_vector[i] += other[i];
	}
	set_dirty_flag(true);
	return *this;
}

Vector3D& Vector3D::operator -=(const Vector3D& other)
{
	for(size_t i = 0; i < m_dimension; ++i) {
		m_vector[i] -= other[i];
	}
	set_dirty_flag(true);
	return *this;
}

Vector3D& Vector3D::operator *=(const Vector3D& other)
{
	for(size_t i = 0; i < m_dimension; ++i) {
		m_vector[i] *= other[i];
	}
	set_dirty_flag(true);
	return *this;
}

Vector3D& Vector3D::operator /=(const Vector3D& other)
{
	for(size_t i = 0; i < m_dimension; ++i) {
		m_vector[i] /= other[i];
	}
	set_dirty_flag(true);
	return *this;
}

Vector3D  Vector3D::operator +(const Vector3D& other) const
{
	Vector3D product;
	for(size_t i = 0; i < m_dimension; ++i) {
		product[i] = m_vector[i] + other[i];
	}
	return product;
}

Vector3D  Vector3D::operator -(const Vector3D& other) const
{
	Vector3D product;
	for(size_t i = 0; i < m_dimension; ++i) {
		product[i] = m_vector[i] - other[i];
	}
	return product;
}

Vector3D  Vector3D::operator *(const Vector3D& other) const
{
	Vector3D product;
	for(size_t i = 0; i < m_dimension; ++i) {
		product[i] = m_vector[i] * other[i];
	}
	return product;
}

Vector3D  Vector3D::operator /(const Vector3D& other) const
{
	Vector3D product;
	for(size_t i = 0; i < m_dimension; ++i) {
		product[i] = m_vector[i] / other[i];
	}
	return product;
}

float& Vector3D::operator [](const size_t u)
{
	return m_vector[u];
	set_dirty_flag(true);
}

Vector3D Vector3D::cross(const Vector3D& other) const
{
	Vector3D product;
	product[0] = m_vector[1] * other[2] - m_vector[2] * other[1];
	product[1] = m_vector[2] * other[0] - m_vector[0] * other[2];
	product[2] = m_vector[0] * other[1] - m_vector[1] * other[0];
}

float Vector3D::dot(const Vector3D& other) const
{
	return m_vector[0] * other[0] + m_vector[1] * other[1] + m_vector[2] * other[2];
}

void Vector3D::normalize(void)
{
	update();
	for(size_t i = 0; i < m_dimension; ++i) {
		m_vector[i] /= m_length;
	}
}

float Vector3D::get_length(void)
{
	update();
	return m_length;
}


void Vector3D::update(void)
{
	if( m_dirty_flag) {
		m_length = 0;
		for(size_t i = 0; i < m_dimension; ++i) {
			m_length += m_vector[i] * m_vector[i];
		}
		m_length = sqrtf(m_length);
		set_dirty_flag(false);
	}
}