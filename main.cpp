#include "func_defs.h"
#include "ParamParse.hpp"
#include "OpenGLRenderWindow.hpp"
#include "OBJReader.hpp"

#include <windows.h>

int main(int argc, char* argv[]) {
	ParamParse::set_args(argc, argv);
	OpenGLRenderWindow renwin;
	GLubyte* render_buffer = reinterpret_cast<GLubyte*>(renwin.get_render_buffer());
	int w = renwin.get_width();
	int h = renwin.get_height();
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			render_buffer[(i * w + j) * 3 + 0] = (i + j) % 255;
			render_buffer[(i * w + j) * 3 + 1] = (i + j) % 128;
			render_buffer[(i * w + j) * 3 + 2] = (i + j) % 64;
		}
	}
	renwin.render();

	glutMainLoop();
	return 0;
}