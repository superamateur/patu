#ifndef __OPEN_GL_RENDER_WINDOW_H
#define __OPEN_GL_RENDER_WINDOW_H

#if defined(__APPLE__)
#  include <GLUT/glut.h>
#else
#  include <GL/glew.h>
#  if defined(_WIN32)
#    include <GL/wglew.h>
#  endif
#  include <GL/glut.h>
#endif

#include "RenderWindow.hpp"

class OpenGLRenderBuffer
{
MAKE_SINGLETON(OpenGLRenderBuffer);
public:
	static std::map<int, GLvoid*>& render_buffer_list(void) { return m_render_buffer_list; }	
private:
	static std::map<int, GLvoid*> m_render_buffer_list;	
};

class OpenGLRenderWindow : public RenderWindow
{
public:
	OpenGLRenderWindow(void);
	virtual ~OpenGLRenderWindow(void);

	virtual void refresh(void);
	virtual void render(void);
	GLvoid* get_render_buffer(void);
	int get_window_id(void) { return m_id; }

	static Geometry* m_geometry;
private:
	kErrorCode init_glut_window(void);
	int m_id; // window id
};
#endif // __OPEN_GL_RENDER_WINDOW_H