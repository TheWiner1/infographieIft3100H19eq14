#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscParameterSync.h"
#include "renderer.h"
#include "draggableVertex.h"
#include "ofxDelaunay.h"

class ofApp : public ofBaseApp {

public:
		Renderer renderer;
		DraggableVertex draggableVertex;

		ofxDelaunay triangulation;//triangularisation

		void setup();
		void update();
		void draw();

	//variables camera;
	ofCamera* camera;

	ofCamera camera_front;

	bool is_camera_perspective;

	ofQuaternion camera_orientation;

	ofVec3f camera_position;
	ofVec3f camera_target;

	string camera_name;
	string camera_projection;

	float camera_near;
	float camera_far;

	float camera_fov;
	float camera_fov_delta;

	bool is_camera_fov_narrow;
	bool is_camera_fov_wide;

	float time_current;
	float time_last;
	float time_elapsed;
	// fin variables  camera

	void projection();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxOscParameterSync sync;
		ofxGuiGroup geometry_group;
		ofParameter<bool> model_one;
		ofParameter<bool> model_two;
		ofParameter<bool> model_three;
		ofxButton model_reset;
		ofParameter<bool> model_box;
		ofxGuiGroup trans_interactive;
		ofParameter<bool> draggable_show,delaunay_show;
		ofxGuiGroup tex_procedural;
		ofParameter<bool> affiche_tex;
		ofxGuiGroup geometrie,topologie;
		ofParameter<bool> affiche_pyramide;
		ofParameter<bool> affiche_sphere;
		ofEventListener pyramide_listener;
		ofEventListener sphere_listener;

		ofParameter<float> size;
		ofParameter<int> number;
		ofParameter<bool> check, check2;
		ofParameterGroup parameters;
		ofParameter<ofColor> color;
		ofParameter<ofColor> colorhsb;
		ofxPanel gui;
		ofColor mycolorhsb;
		ofEventListener model_one_listener;
		ofEventListener model_two_listener;
		ofEventListener model_three_listener;

		ofxGuiGroup group_draw, groupe2;
		ofParameter<ofColor> color_picker_background;
		ofParameter<ofColor> color_picker_stroke;
		ofParameter<ofColor> color_picker_fill;
		ofParameter<ofColor> color_picker_background_hsb;
		ofParameter<ofColor> color_picker_stroke_hsb;
		ofParameter<ofColor> color_picker_fill_hsb;
		ofParameter<float> slider_stroke_weight;
		ofParameter<string> textbox;
		ofParameter<bool> checkbox1, rect, point, pixel, ligne, elipse, triangle;
		ofxButton reset_button, undo, redo;
		ofEventListener pixel_listener;
		ofEventListener point_listener;
		ofEventListener ligne_listener;
		ofEventListener rect_listener;
		ofEventListener elipse_listener;
		ofEventListener triangle_listener;

		ofxGuiGroup filter_group;
		ofParameter<bool> aucun_filtre, grayscale, seuil, lissage, seuilLissage;
		ofEventListener aucun_filtre_listener, grayscale_listener, seuil_listener, lissage_listener, seuilLissage_listener;

		ofParameter<string> textboxX;
		ofParameter<string> textboxY;

		int nFrames;
		int recFrames;
		int exportCount;
		ofImage exportImg;

		ofxButton tonemapping;

		void ofxSetColorHSB(float h, float s, float b, float a);
		int getBrightness(float r, float g, float b);
		int getLightness(float r, float g, float b);
		int getHue(float r, float g, float b);
		int getSaturation(float r, float g, float b);

		void captureFrame();
		void reset_button_pressed();
		void tonemapping_pressed();
		void onChangePrimitiveVectorielle(string name, bool value);
		void onChangeGeometryGroup(string name, bool value);
		void model_reset_pressed();
		void onChangeFiltrage(string name, bool value);
		void undo_pressed();
		void redo_pressed();
};