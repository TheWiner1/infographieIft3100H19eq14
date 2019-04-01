#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxOpenCv.h"

enum class VectorPrimitiveType { none, pixel, point, line, rectangle, ellipse, triangle };
enum class ModelToDraw { modelOne, modelTwo, modelThree };
enum class Filtrage { none, grayscale, seuil, lissage, seuilLissage};

struct VectorPrimitive
{
	VectorPrimitiveType type;
	float                  position1[2];
	float                  position2[2];
	float                  stroke_width;
	ofColor							   stroke_color;
	ofColor							   fill_color;
};

class Renderer
{
public:
	VectorPrimitiveType draw_mode;

	VectorPrimitive* shapes;

	ofColor background_color;

	int shapeCount;
	int size;
	int stride;
	int index;
	int head;

	float radius;
	int mouse_press_x;
	int mouse_press_y;
	int mouse_current_x;
	int mouse_current_y;

	bool is_mouse_button_pressed;

	bool selection_multiple;

	ofColor stroke_fill;
	ofColor stroke_color;
	float epaisseur;
	bool check;

	ofImage import;

	ofxAssimpModelLoader* models[3];
	ofxAssimpModelLoader model_one;
	ofxAssimpModelLoader model_two;
	ofxAssimpModelLoader model_three;

	ModelToDraw model_draw_mode;

	ofLight light;

	int model_count;

	ofVec3f vector_position;
	ofVec3f vector_rotation;
	ofVec3f vector_proportion;

	bool model_box;
	ofVec3f size_box;

	int w_texture, h_texture;
	ofTexture	texGray;
	ofTexture texColor;
	ofTexture	texColorAlpha;
	ofPixels colorPixels;
	ofPixels grayPixels;
	ofPixels colorAlphaPixels;
	bool affiche_tex;

	ofMesh mesh;
	vector<ofPoint> vertices;
	vector<ofColor> colors;
	int nTri; //le nombre de triangle a creer
	int nVert; //sommet de tous les triangles = nTri * 3
	bool affiche_pyramide;
	bool affiche_sphere;

	ofxCvColorImage image; //image Original
	ofxCvGrayscaleImage grayImage; // image originale en niveaux de gris
	ofxCvGrayscaleImage filtered; // Image utilisée pour le filtrage
	Filtrage filtrage;

	stack<VectorPrimitive> primitiveStack;

	void setup();
	void update();
	void draw();
	void reset();

	void undo();
	void redo();

	void add_vector_shape(VectorPrimitiveType type);
	void remplissage();

	void draw_pixel(float x, float y) const;
	void draw_point(float x, float y, float radius) const;
	void draw_line(float x1, float y1, float x2, float y2) const;
	void draw_rectangle(float x1, float y1, float x2, float y2) const;
	void draw_ellipse(float x1, float y1, float x2, float y2) const;
	void draw_triangle(float x1, float y1, float x2, float y2) const;

	void draw_zone(float x1, float y1, float x2, float y2) const;
	void draw_cursor(float x, float y) const;

	void change_texture(int x, int y);

	void toneMap();

	void add_3d_model(ModelToDraw model);
	void model_reset();
	void get_bounding_box(int model);

	~Renderer();
};
