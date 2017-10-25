#include "OpenGLRenderWindow.hpp"
#include "ParamParse.hpp"

// static member declaration
std::map<int, GLvoid*> OpenGLRenderBuffer::m_render_buffer_list;

// forward declaration of utility functions
static void keyboard ( unsigned char key, int x ,int y);
static void display(void);

OpenGLRenderWindow::OpenGLRenderWindow(void)
: RenderWindow()
{
	if( ERROR_CODE_NO_ERROR != init_glut_window() ) {
		ERR_MSG("Failed to create render window");
	}
}

OpenGLRenderWindow::~OpenGLRenderWindow(void)
{
}

void OpenGLRenderWindow::refresh(void)
{
}

void OpenGLRenderWindow::render(void)
{
	// make this window the current window
	glutSetWindow(m_id);

	// invoke the display callback
	glutPostRedisplay();

	// main display loop
	glutMainLoop();
}

kErrorCode OpenGLRenderWindow::init_glut_window(void)
{
	// init render window using glut funtions
	int main_argc = ParamParse::argc();
	char** main_argv = ParamParse::argv();
	glutInit(&main_argc, main_argv);

	// set display node: RGB, double buffered
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// set size
	glutInitWindowSize(m_width, m_height);

	// set position
	glutInitWindowPosition(m_position_x, m_position_y);

	// set window name to the application name
	glutCreateWindow(main_argv[0]);

	// set keyboard callback
	glutKeyboardFunc(keyboard);

	// get this window id
	m_id = glutGetWindow();

	// prepare the render buffer
	OpenGLRenderBuffer::render_buffer_list().insert(std::make_pair<int, GLvoid*>(m_id, new GLubyte[m_width * m_height * 3]));
	
	// set display callback
	glutDisplayFunc(display);

	return ERROR_CODE_NO_ERROR;
}

GLvoid* OpenGLRenderWindow::get_render_buffer(void)
{
	return OpenGLRenderBuffer::render_buffer_list()[m_id];
}

static void keyboard ( unsigned char key, int x ,int y)
{
  switch ( key )
  {
    case 27: // esc key
      exit ( 0 );
      break;
    default:
      break;
  }
}

static void display(void)
{
	/* NOTE: this window must be made current first*/

	// get window size
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);

	// get the winddow id
	int wid = glutGetWindow();

	if(0 != wid) {
		GLvoid* render_buffer = OpenGLRenderBuffer::render_buffer_list()[wid];
		if(NULL != render_buffer) {
			glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, render_buffer);
			glutSwapBuffers();
		}
	}
}