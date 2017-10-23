#ifndef __RENDER_WINDOW_H
#define __RENDER_WINDOW_H

#include "func_defs.h"
#include "errors.h"
#include "Thread.hpp"

class RenderWindow
{
public:
	RenderWindow(void);
	virtual ~RenderWindow(void);
	virtual void refresh(void);
	virtual void render(void);
	
public:
	SET_GET_MACRO(size_t, width, width);
	SET_GET_MACRO(size_t, height, height);
	SET_GET_MACRO(size_t, position_x, position_x);
	SET_GET_MACRO(size_t, position_y, position_y);

protected:
	size_t m_width;
	size_t m_height;
	size_t m_position_x;
	size_t m_position_y;
};

#endif // __RENDER_WINDOW_H