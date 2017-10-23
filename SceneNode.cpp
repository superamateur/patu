#include "SceneNode.hpp"

// SceneNode class
SceneNode::SceneNode(void)
: m_child_node(NULL)
{
}

SceneNode::~SceneNode(void)
{
	if(NULL != m_child_node) {
		delete m_child_node;
		m_child_node = NULL;
	}
}

SceneNodeList* SceneNode::get_child(void)
{
	return m_child_node;
}

void SceneNode::add_child(SceneNode* node)
{
	if(NULL == m_child_node) {
		m_child_node = new SceneNodeList;
	}
	m_child_node->get_nodes().push_back(node);
}

// SceneNodeList class
SceneNodeList::SceneNodeList(void)
: m_node(0)
{
}

SceneNodeList::~SceneNodeList(void)
{
	for(std::vector<SceneNode*>::iterator it = m_node.begin(); it != m_node.end(); ++it) {
		if(NULL != *it) {
			delete *it;
		}
	}
}

std::vector<SceneNode*>& SceneNodeList::get_nodes(void) {
	return m_node;
}

SceneNode* SceneNodeList::get_node(const size_t n) {
	if(n >= m_node.size()) {
		ERR_MSG("Index exceeding vector size");
	}
	return m_node[n];
}

void SceneNodeList::init_traversal(void)
{
	m_node_traversal = m_node.begin();
}

SceneNode* SceneNodeList::get_next_node(void) {
	if(m_node_traversal == m_node.end()) {
		return NULL;
	}

	SceneNode* ret = *m_node_traversal;
	++m_node_traversal;
	return ret;
}