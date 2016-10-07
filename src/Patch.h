#ifndef PATCH_H_
#define PATCH_H_

#if defined(WIN32)
	#define NOMINMAX
	#include <windows.h>
	#include <GL/gl.h>
#elif defined(__APPLE__)
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/glu.h>
#else
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <vector>

class Frustum;

class Patch
{
public:
	Patch(int x, int y, int nbTilesX, int nbTilesY);
	~Patch();

	void initialise();
	void display ();
	void update();

	bool testVisibility(Frustum * frustum);
	bool getVisibilty();
	
private:

	int m_x;
	int m_y;

	std::vector <float> pointcoordsx;
	std::vector <float> pointcoordsy;
	std::vector <float> pointcoordsz;
	bool visibility;
	bool requiresUpdate;
	GLuint texture;
	GLuint drawList;

	void createGeometry(int nb_TileX, int nb_TileY);
};

#endif
