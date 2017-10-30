#include "OpenGLRenderWindow.hpp"
#include "ParamParse.hpp"
#include "Geometry.hpp"

// static member declaration
std::map<int, GLvoid*> OpenGLRenderBuffer::m_render_buffer_list;
Geometry* OpenGLRenderWindow::m_geometry = NULL;

// forward declaration of utility functions
static void keyboard ( unsigned char key, int x ,int y);
static void display(void);
static void reshape(GLsizei width, GLsizei height);
static void draw(Geometry* g);

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

	glutReshapeFunc(reshape); 

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
	case 'w':
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		glutPostRedisplay();
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
		/*if(NULL != render_buffer) {
			glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, render_buffer);
			glutSwapBuffers();
		}*/

		draw(OpenGLRenderWindow::m_geometry);
	}
}

static void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset

   SBoundingBox bbox = OpenGLRenderWindow::m_geometry->get_bounding_box();
   Vector3D center = (bbox.from + bbox.to) * Vector3D(0.5, 0.5, 0.5);
   center.dump();
   gluLookAt(center[0], center[1], 0.f,
			center[0], center[1], center[2],
			0.0f, 1.0f,  0.0f);

   gluPerspective(45.0f, aspect, 0.1f, 1000.0f);

   glutPostRedisplay();
}

static void draw(Geometry* g)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	glLoadIdentity();                 // Reset the model-view matrix
	if(NULL != g) {
		FaceArray faces = g->faces();
		glBegin(GL_TRIANGLES); 
		const float scale = 1;
		for(int i = 0; i < (int)faces.size(); ++i) {
			const std::vector<int>& triangle = faces[i].m_vertex_id;
			glVertex3f(scale * g->get_vertex_pos(triangle[0])[0], scale * g->get_vertex_pos(triangle[0])[1], scale * g->get_vertex_pos(triangle[0])[2]);
			glVertex3f(scale * g->get_vertex_pos(triangle[1])[0], scale * g->get_vertex_pos(triangle[1])[1], scale * g->get_vertex_pos(triangle[1])[2]);
			glVertex3f(scale * g->get_vertex_pos(triangle[2])[0], scale * g->get_vertex_pos(triangle[2])[1], scale * g->get_vertex_pos(triangle[2])[2]);
		}
		glEnd();
	}
	glutSwapBuffers();
}