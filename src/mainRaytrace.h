#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofxGui.h"

class ofAppRT : public ofBaseApp {
	public:
		ofxPanel gui;
		ofParameter<ofVec3f> color1;
		//ofParameter<ofVec3f> color2;
		//ofParameter<ofVec3f> color3;
		ofParameter<float>     radius1;
		//ofParameter<float>     radius2;
		//ofParameter<float>     radius3;
		ofParameter<float>     intLight;
		ofParameter<ofVec2f>   speLight;
		ofParameter<ofVec3f>   colLight;
		ofParameter<float>     ambient;
		ofParameter<ofVec3f>     position_lumiere;
		ofParameter<ofVec3f>   posSphere1;
		
		ofParameter<ofVec3f>   posCubemax;
		ofParameter<ofVec3f>   posCubemin;
		ofParameter<bool> s1,s2,s3,s4;

		ofFbo fbo;
		int w,h;
		ofShader lanceRayon;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);

};
/*
int main( ){
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800,600, OF_WINDOW);
	ofRunApp(new ofAppRT());

}*/
