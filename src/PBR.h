// IFT3100H19_PBR/renderer.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
enum class ModelToDrawPBR { modelOne, modelTwo, modelThree };
class PBR
{
public:

  ofShader shader_pbr;

  ofShader* shader;
  ofVec3f size_box;
  void get_bounding_box(int model);
  ofColor material_color_ambient;
  ofColor material_color_diffuse;
  ofColor material_color_specular;
  int mouse_press_x;
  int mouse_press_y;

  float material_metallic;
  float material_roughness;
  float material_occlusion;
  float material_brightness;

  glm::vec3 material_fresnel_ior;
  void model_reset();
	  void add_3d_model(ModelToDrawPBR model);
	  ModelToDrawPBR model_draw_mode;
	  int mouse_current_x;
	  int selectedModel = 0;
	  int mouse_current_y;
	  ofxAssimpModelLoader* models[3];
	  ofxAssimpModelLoader model_one;
	  ofxAssimpModelLoader model_two;
	  ofxAssimpModelLoader model_three;
	  ofVec3f vector_position;
	  bool addedModels[3] = { false };
	  int model_count;
	  ofVec3f vector_proportion;
  ofImage texture_diffuse;
  ofImage texture_metallic;
  ofImage texture_roughness;
  ofImage texture_occlusion;
  bool model_box;
  ofLight light;
  ofColor light_color;
  float light_intensity;
  bool light_motion;

  float tone_mapping_exposure;
  float tone_mapping_gamma;
  bool tone_mapping_toggle;

  ofxAssimpModelLoader teapot;

  ofVec3f position_cube;
  ofVec3f position_sphere;
  ofVec3f position_teapot;

  float scale_cube;
  float scale_sphere;
  float scale_teapot;

  float speed_motion;
  float speed_rotation;

  float offset_x;
  float offset_z;

  float delta_x;
  float delta_y;
  float delta_z;

  float rotation_y;

  float initial_x;
  float initial_y;

  float center_x;
  float center_y;

  void setup();
  void update();
  void draw();
  void reset();
};
