//#include "ofMain.h"
#include "ofApp.h"
#include "application_Dynamic_Light.h"
#include "ofAppGlutWindow.h"
//========================================================================
//<<<<<<< erwin

bool use_gl_fixed_function_pipeline = false;

int main()
{
	ofAppGlutWindow window;
  ofGLWindowSettings windowSettings;
  windowSettings.setSize(1024, 512);

  if (use_gl_fixed_function_pipeline)
    windowSettings.setGLVersion(2, 1);
  else
    windowSettings.setGLVersion(3, 3);

  ofCreateWindow(windowSettings);
  ofRunApp( new ofApp());/*
  //ofRunApp(new ApplicationDynamicLight());

int main( ){
	//ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	
	//ofRunApp( new ofApp());
	
	ofGLWindowSettings windowSettings;
	windowSettings.setSize(1024, 768);
	windowSettings.setGLVersion(3, 3);
	ofCreateWindow(windowSettings);
	ofRunApp(new ofApp());
*/
}

