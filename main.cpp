#include "func_defs.h"
#include "ParamParse.hpp"
#include "OpenGLRenderWindow.hpp"
#include "OBJReader.hpp"

#include <windows.h>
#include <math.h>
int main(int argc, char* argv[]) {
	OBJReader obj("pumpkin_tall_10k.obj");
	Geometry bunny_geom;
	obj.read(bunny_geom);
	bunny_geom.show_off();
	const std::vector<TCoord_t>& tc = bunny_geom.tcoords();	
	Geometry bunny_half_geom[2];
	/*INFO_MSG("cut point = ");
	bunny_geom.get_geometry_center().dump();*/
	bunny_geom.planar_cut(bunny_half_geom[0], bunny_half_geom[1], bunny_geom.get_geometry_center(), Vector3D_t(0, 0, 1));
	
	bunny_half_geom[0].show_off(); bunny_half_geom[0].export_to_obj("bunny_01.obj");
	bunny_half_geom[1].show_off(); bunny_half_geom[1].export_to_obj("bunny_02.obj");
	Geometry  flatten = bunny_half_geom[0].make_flatten();
	
	flatten.show_off(); flatten.export_to_obj("flatten.obj");

	return 0;
	OpenGLRenderWindow::m_geometry = &bunny_half_geom[0];

	ParamParse::set_args(argc, argv);
	OpenGLRenderWindow renwin;
	GLubyte* render_buffer = reinterpret_cast<GLubyte*>(renwin.get_render_buffer());
	int w = renwin.get_width();
	int h = renwin.get_height();
	INFO_MSG("W = " << w << ",H = " << h);
	for(int i = 0; i < (int)tc.size(); ++i) {
		const int iu = (int)(floor(tc[i][0] * w)) % w;
		const int iv = (int)(floor(tc[i][1] * h)) % h;
		//INFO_MSG("iu = " << iu << ", iv = " << iv << "u = " << tc[i][0] << ", v = " << tc[i][1]);
		render_buffer[(iv * w + iu) * 3 + 1] = 255;
	}
	/*for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			render_buffer[(i * w + j) * 3 + 0] = (i + j) % 255;
			render_buffer[(i * w + j) * 3 + 1] = (i + j) % 128;
			render_buffer[(i * w + j) * 3 + 2] = (i + j) % 64;
		}
	}*/
	renwin.render();
	
	return 0;
}