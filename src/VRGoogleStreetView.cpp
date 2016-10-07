#include <string>
#include <iostream>
#include <math.h>

#include <display/VRConsoleNode.h>
#include <main/VRMain.h>
#include <main/VREventHandler.h>
#include <main/VRRenderHandler.h>
#include <math/VRMath.h>

#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <gl/GLU.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "Frustum.h"
#include "Patch.h"

using namespace MinVR;

class MyVRApp : public VREventHandler, public VRRenderHandler {
public:
	MyVRApp(int argc, char** argv) : _vrMain(NULL){
		_vrMain = new VRMain();
		_vrMain->initialize(argc, argv);
		_vrMain->addEventHandler(this);
		_vrMain->addRenderHandler(this);

		frustum = new Frustum();
	}

	virtual ~MyVRApp() {
		_vrMain->shutdown();
		delete _vrMain;
		delete frustum;
		for (int i = 0; i < patches.size(); i++)
		{
			delete patches[i];
		}
	}

	// Callback for event handling, inherited from VREventHandler
	virtual void onVREvent(const std::string &eventName, VRDataIndex *eventData) {
		if (eventName == "/KbdEsc_Down") {
			
		}
	}


	virtual void onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode) {
		//perform update here
		if ((int)renderState->getValue("InitRender", "/") == 1) {
			
			int tiles_x = 26;
			int tiles_y = 13;

			for (int i = 0; i < tiles_x; i++)
			{
				for (int j = 0; j < tiles_y; j++)
				{
					Patch * p = new Patch(i, j, tiles_x, tiles_y);
					p->initialise();
					patches.push_back(p);
				}
			}
		}
	}


	// Callback for rendering, inherited from VRRenderHandler
	virtual void onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode) {
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);

		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0f);
		glClearColor(0.0, 0.0, 0.0, 1.f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		VRMatrix4 P = renderState->getValue("ProjectionMatrix", "/");
		glLoadMatrixd(P.m);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		VRMatrix4 V = renderState->getValue("ViewMatrix", "/");
		glLoadMatrixd(V.m);	

		//set Frustum
		frustum->update(P.m, V.m);

		//check Visisbility for patches
		for (int i = 0; i < patches.size(); i++)
		{
			patches[i]->testVisibility(frustum);
		}

		//update patches if necessary
		for (int i = 0; i < patches.size(); i++)
		{
			patches[i]->update();
		}

		//render patches
		for (int i = 0; i < patches.size(); i++)
		{
			patches[i]->display();
		}
		std::cerr << "Draw" << std::endl;
	}

	void run() {
		while (1) {
			_vrMain->mainloop();
		}
	}

protected:
	VRMain *_vrMain;
	std::vector <Patch *> patches;
	Frustum * frustum;
};



int main(int argc, char **argv) {

	MyVRApp app(argc, argv);
	app.run();

	exit(0);
}
