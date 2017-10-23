#include "Shape.hpp"

Shape::Shape(void)
: m_geometry(NULL)
, m_transformer(NULL)
, m_shader(NULL)
{
}

Shape::~Shape(void)
{
	if(NULL != m_geometry) delete m_geometry;
	if(NULL != m_transformer) delete m_transformer;
	if(NULL != m_shader) delete m_shader;
}

const Geometry* Shape::geometry(void) const
{
	return m_geometry;
}
Geometry* Shape::geometry(void)
{
	return m_geometry;
}

const Transformer* Shape::transformer(void) const
{
	return m_transformer;
}

Transformer* Shape::transformer(void)
{
	return m_transformer;
}

const Shader* Shape::shader(void) const
{
	return m_shader;
}

Shader* Shape::shader(void)
{
	return m_shader;
}
