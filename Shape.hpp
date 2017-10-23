#pragma once
#include "SceneNode.hpp"
#include "Transformer.hpp"
#include "Geometry.hpp"
#include "shader.hpp"

class Shape : public SceneNode
{
public:
	Shape(void);
	virtual ~Shape(void);
	
	// get-set routine
	const Geometry* geometry(void) const;
	Geometry* geometry(void);

    const Transformer* transformer(void) const;
	Transformer* transformer(void);

	const Shader* shader(void) const;
	Shader* shader(void);

private:
	Geometry* m_geometry;
	Transformer* m_transformer;
	Shader* m_shader;
};
