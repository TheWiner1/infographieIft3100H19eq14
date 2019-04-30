#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxOpenCv.h"
#include "rendererPlus.h"
#include "renderer_Dynamic_Light.h"
//#include "ofPoint.h"
//#include "Vector.h"

enum class VectorPrimitiveType { none, pixel, point, line, rectangle, ellipse, triangle };
enum class ModelToDraw { modelOne, modelTwo, modelThree, modelFour, modelFive, modelSix };
enum class Filtrage { none, grayscale, seuil, lissage, seuilLissage};
enum class ShaderType { color_fill, lambert, gouraud, phong, blinn_phong };

struct VectorPrimitive
{
	VectorPrimitiveType type;
	float                  position1[2];
	float                  position2[2];
	float                  stroke_width;
	ofColor							   stroke_color;
	ofColor							   fill_color;

	//float                  stroke_width;    // 1 * 4 = 4  octets
//  unsigned char          stroke_color[4]; // 4 * 1 = 4  octets
 // unsigned char          fill_color[4];   // 4 * 1 = 4  octets
  bool isSelected = false;
  float width = abs(position1[0]-position2[0]);
  float height = abs(position1[1]-position2[1]);
};

class Renderer
{
public:
	VectorPrimitiveType draw_mode;

	VectorPrimitive* shapes;

	ofColor background_color;

	int count;
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

	
	ofMaterial material_cube;
	ofMaterial material_sphere;
	ofMaterial material_teapot;


  	float scale_cube;
  	float scale_sphere;
  	float scale_teapot;
  
  	ofxAssimpModelLoader teapot;

  	ofVec3f position_cube;
  	ofVec3f position_sphere;
  	ofVec3f position_teapot;


	ofxAssimpModelLoader* models[6];
	ofxAssimpModelLoader model_one;
	ofxAssimpModelLoader model_two;
	ofxAssimpModelLoader model_three;
/*/*/
	ofxAssimpModelLoader model_four;
	ofxAssimpModelLoader model_five;
	ofxAssimpModelLoader model_six;
/*/*/

	bool addedModels[3] = { false };
	int selectedModel = 0;
	ofxAssimpModelLoader* modelsMirror[3];
	float mirror_z;
	ofxAssimpModelLoader model_one_mirror;
	ofxAssimpModelLoader model_two_mirror;
	ofxAssimpModelLoader model_three_mirror;

	ModelToDraw model_draw_mode;

	ofLight light;

	int model_count;

	ofVec3f vector_position;
	ofVec3f vector_rotation;
	ofVec3f vector_proportion;

	bool model_box;
	ofVec3f size_box;

	bool model_mirror;

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
	ofxCvGrayscaleImage filtered; // Image utilis�e pour le filtrage
	Filtrage filtrage;

	stack<VectorPrimitive> primitiveStack;

	/*Variables de l'illumination*/
	ShaderType shader_active;
	ofShader shader_color_fill;
	ofShader shader_lambert;
	ofShader shader_gouraud;
	ofShader shader_phong;
	ofShader shader_blinn_phong;

	ofShader* shader;
	string shader_name;

	float oscillation_frequency;
	float oscillation_amplitude;
	
	float oscillate(float time, float frequency, float amplitude);

	void updateIllumination();
	//*fin variables illumination

	//variables camera;

	/*Alex begin*/

	ofPlanePrimitive v_plane;
	ofMaterial material_v_plane;

	float speed_delta;
	float speed_translation;
	float speed_rotation;

	bool is_camera_move_left;
	bool is_camera_move_right;
	bool is_camera_move_up;
	bool is_camera_move_down;
	bool is_camera_move_forward;
	bool is_camera_move_backward;
	bool is_camera_roll_left;
	bool is_camera_roll_right;
	bool is_camera_pan_left;
	bool is_camera_pan_right;
	bool is_camera_look_at;


	ofMaterial model_material[3];
	ofMaterial material_0;
	ofMaterial material_1;
	ofMaterial material_2;
	ofMaterial material_3;
	ofMaterial material_4;
	ofMaterial material_current;

	ofVec3f  camera_dir;
	float camera_to_mirror_angle;
	/*Alex end*/
	/*Camera*/
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
	//fin variables  camera

	void setup();
	//void setupCamera();
	void setupIllumination();
	void updateCamera();
	//void projection();
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
	float geox, geoy, geoz;
	void model_reset();
	void get_bounding_box(int model);

	~Renderer();

	void select_shapes(int x, int y);
  	void move_shapes(int x, int y);
  	void delete_selected_shapes();
  	void selectAll();
  	void deselectAll();
};
