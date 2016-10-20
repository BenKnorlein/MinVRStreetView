#include <string>
#include <iostream>
#include <math.h>

#include <display/VRConsoleNode.h>
#include <main/VRMain.h>
#include <main/VREventHandler.h>
#include <main/VRRenderHandler.h>
#include <math/VRMath.h>
#include <input/VRInputDevice.h>

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

enum Mode { idle, requestPanoId, waitForPanoID, requestData, waitForData };

using namespace MinVR;

class MyVRApp : public VREventHandler, public VRRenderHandler, public VRInputDevice {
public:
	MyVRApp(int argc, char** argv) : _vrMain(NULL), isMaster(false), mode(idle){
		_vrMain = new VRMain();
		_vrMain->initialize(argc, argv);
		_vrMain->addEventHandler(this);
		_vrMain->addRenderHandler(this);
		_vrMain->addInputDevice(this);

		//get all names from the config-file
		std::list<std::string> names = _vrMain->getConfig()->selectByAttribute("hostType","*");

		//check if it is the first entry and if it is set it as the master
		if(names.size() > 0 && _vrMain->getName() == names.front()){
			isMaster = true;
			std::cerr << "I am the master : " << _vrMain->getName()  << std::endl;
		}
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

	//Function to append events
	virtual void appendNewInputEventsSinceLastCall(VRDataQueue *queue){
		for (int f = 0; f < _events.size(); f++)
		{
			queue->push(_events[f]);
		}
		_events.clear();
	}

	// Callback for event handling, inherited from VREventHandler
	virtual void onVREvent(const std::string &eventName, VRDataIndex *eventData) {
		if (isMaster && mode == idle) {
			std::cerr << eventName << std::endl;
			if(eventName == "/Wand_Left_Btn_Down"){
				mode = requestPanoId;
			}
		}
		
		if(eventName == "/ReloadImages"){
				//Received on all nodes to update textures
				std::cerr << "Reload Images" << std::endl;
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
	}

	void run() {
		while (1) {
			_vrMain->mainloop();
			
			if(isMaster){
					
				//the check here should be implemented in a switch statement. For now just with if statements
				if(mode == requestPanoId)
				{
					//request a new PanoID and go to the next mode to wait for panID
					std::cerr << "Request new PanoID" << std::endl;
					mode = waitForPanoID;
				}
				if(mode == waitForPanoID)
				{
					//check if the panoID was received
					bool panoIDreceived = true; // for now always true
					if(panoIDreceived){
						//ID received change mode
						std::cerr << "PanoID received" << std::endl;
						mode = requestData;
					}
				}
				if(mode == requestData)
				{
					//request all new Images and go to the next mode to wait for completion
					std::cerr << "Request new images" << std::endl;
					mode = waitForData;
				}
				if(mode == waitForData)
				{
					//check if all images were received. 
					bool datareceived = true; // for now always true
					//send an event
					if(datareceived){
						//Data received. Go to idle mode
						std::cerr << "All images received send event to update textures" << std::endl;					
						mode = idle;
						
						//send a signal to update the textures	
						VRDataIndex di;
						di.addData("/ReloadImages","");
						_events.push_back(di.serialize("/ReloadImages"));
					}
				}	
			}
		}
	}

protected:
	VRMain *_vrMain;
	std::vector <Patch *> patches;
	Frustum * frustum;
	
	bool isMaster;
	Mode mode;
	//Queue to send events from the master to the client
	std::vector<std::string> _events;
};



int main(int argc, char **argv) {

	MyVRApp app(argc, argv);
	app.run();

	exit(0);
}
