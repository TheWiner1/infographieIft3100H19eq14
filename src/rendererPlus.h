// IFT3100H19_ImageExport/rendererPlus.h
// Classe responsable du rendu de l'application.

#pragma once
//#include <iostream>
#include <stdlib.h>
#include "ofMain.h"

enum class VectorPrimitive3dType {none,plane, box, sphere, icoSphere, cylinder, cone};

struct VectorPrimitive3d
{
  VectorPrimitive3dType type;            // 1 * 4 = 4  octets
  float                  position1[3];    // 3 * 4 = 12  octets
  float                  position2[3];    // 3 * 4 = 12  octets
  float                  stroke_width;    // 1 * 4 = 4  octets
  unsigned char          stroke_color[4]; // 4 * 1 = 4  octets
  unsigned char          fill_color[4];   // 4 * 1 = 4  octets
  bool isSelected = false;
  float width = abs(position1[0]-position2[0]);
  float height = abs(position1[1]-position2[1]);
  float depth = abs(position1[2]-position2[2]);
};

class RendererPlus
{
public:



  //--------------
  int mouse_press_x;
  int mouse_press_y;

  int nBpress;
  int mouse_press_x2;
  int mouse_press_y2;


  int mouse_current_x;
  int mouse_current_y;

  int last_mouse_released_x;
  int last_mouse_released_y;

  int radius;

  bool is_mouse_button_pressed;

  void setup();
  void update();
  void draw();

  void reset();

  void draw_zone(float x1, float y1, float x2, float y2) const;
  void draw_cursor(float x, float y) const;



  bool bFill;
  bool bWireframe;
  bool bDrawNormals;
  bool bDrawAxes;
  bool bDrawLights;
  bool bHelpText;
  bool bMousePressed;
  bool bSplitFaces;
  float stroke_width3d_default;

  VectorPrimitive3dType draw_mode3d;
  VectorPrimitive3d* shapes3d;

  ofImage texture;
  ofVideoGrabber vidGrabber;

  unsigned char stroke_color_r;
  unsigned char stroke_color_g;
  unsigned char stroke_color_b;
  unsigned char stroke_color_a;
  unsigned char fill_color_r;
  unsigned char fill_color_g;
  unsigned char fill_color_b;
  unsigned char fill_color_a;

  int mode, modeP;

  int count3d;
  int size3d;
  int stride3d;

  int index3d;
  int head3d;

  ofImage image;

  float screenWidth = ofGetWidth();
  float screenHeight = ofGetHeight();

  ofShader shader;
  ofShader shader_passthrough;
  ofShader shader_invert;

  ofSpherePrimitive sphere;
  ofIcoSpherePrimitive icoSphere;
  ofPlanePrimitive plane;
  ofCylinderPrimitive cylinder;
  ofConePrimitive cone;
  ofBoxPrimitive box;

  ofLight pointLight;
  ofLight pointLight2;
  ofLight pointLight3;
  ofMaterial material;

  // place to store the sides of the box //
  ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
  ofVboMesh deformPlane;
  ofVboMesh topCap, bottomCap, body;
  vector<ofMeshFace> triangles;

  ofCamera cam;
//  void set_primitive_3d();//(int modeP){
  void set_primitive_3d_plane(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2);//int modeP);
  void set_primitive_3d_box(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2);//int modeP);
  void set_primitive_3d_sphere(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2);//int modeP);
  void set_primitive_3d_icosphere(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2);//int modeP);
  void set_primitive_3d_cylinder(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2);//int modeP);
  void set_primitive_3d_cone(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2);//int modeP);
  void add_vector_shape3d(VectorPrimitive3dType type);
  void rotate_primitive_3D();
  void select_shapes3d(int x, int y, int z);
  void move_shapes3d(int x, int y, int z);
  void delete_selected_shapes3d();
  void selectAll3d();
  void deselectAll3d();
  //---------------

  ofTrueTypeFont font;

  ofColor background_color;

  ofColor stroke_color;

  ofRectangle bounding_box;

  string text;

  float stroke_weight;

  float line_offset;

  int font_size;

  bool filter;


  void image_export(const string name, const string extension) const;
  ~RendererPlus();
};
