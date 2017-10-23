#ifndef __VECTOR_3D_H
#define __VECTOR_3D_H
#include "func_defs.h"

class Vector3D
{
public:
	explicit Vector3D(void);
	explicit Vector3D(const float x, const float y, const float z);
	explicit Vector3D(const float* v);
	virtual ~Vector3D(void);

	// member access
		  std::vector<float>& get_vector(void)		 { return m_vector; }
	const std::vector<float>& get_vector(void) const { return m_vector; }

	// operator oveload
	Vector3D& operator  =(const Vector3D& other);
	Vector3D& operator +=(const Vector3D& other);
	Vector3D& operator -=(const Vector3D& other);
	Vector3D& operator *=(const Vector3D& other);
	Vector3D& operator /=(const Vector3D& other);
	Vector3D  operator  +(const Vector3D& other) const;
	Vector3D  operator  -(const Vector3D& other) const;
	Vector3D  operator  *(const Vector3D& other) const;
	Vector3D  operator  /(const Vector3D& other) const;
	
	Vector3D cross(const Vector3D& other) const;
	float dot(const Vector3D& other) const;

	float& operator [](const size_t u);
	float  operator [](const size_t u) const { return m_vector[u]; }

	// other functionality
	void normalize(void);
	float get_length(void);
	const float* vector(void) const { return &m_vector[0]; }
	void update();

private:
	size_t m_dimension;
	float m_length;
	std::vector<float> m_vector;
	bool m_dirty_flag;
};

typedef struct Point_
{
	float p[3];
	Point_(void)
	{
		p[0] = FLT_0;
		p[1] = FLT_0;
		p[2] = FLT_0;
	}
} Point_t;

#endif //__VECTOR_3D_H