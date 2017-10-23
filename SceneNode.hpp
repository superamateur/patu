#pragma once

#include "func_defs.h"

class SceneNodeList;
class SceneNode
{
public:
	SceneNode(void);
	virtual ~SceneNode(void);
	
	SceneNodeList* get_child(void);
	void add_child(SceneNode* node);

protected:
	SceneNodeList* m_child_node;
};

class SceneNodeList
{
public:
	SceneNodeList(void);
	virtual ~SceneNodeList(void);
	
	std::vector<SceneNode*>& get_nodes(void);
	SceneNode* get_node(const size_t n);
	void init_traversal(void);
	SceneNode* get_next_node(void);

protected:
	std::vector<SceneNode*> m_node;
	std::vector<SceneNode*>::iterator m_node_traversal;
};