#pragma once
#include "SceneNode.hpp"
#include "func_defs.h"

/* 
 * Scene graph is the holder class for any component participating in the scene, 
 * including shape, light, camera, etc.. It takes control of what component is available 
 * for the next rendering pass
 */

class SceneGraph
{
public:
	typedef enum {
		_rgb = 0,
		_a,
		_z,
		_n
	} aov_t;

public:
	SceneGraph(void);
	virtual ~SceneGraph(void);

	void add_node(const SceneNode* node);
	void build_bvh(void);

private:
	SceneNode* m_root_node;
};
