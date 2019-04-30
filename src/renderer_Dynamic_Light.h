// IFT3100H19_Lighting/rendererDynamicLight.h
// Classe responsable du rendu de l'application.

#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class RendererDynamicLight
{
public:

  float am_posX, am_posY, am_posZ,
  dir_posX, dir_posY, dir_posZ,
  po_posX, po_posY, po_posZ,
  spo_posX, spo_posY, spo_posZ,
  cub_posX, cub_posY, cub_posZ,
  sphe_posX, sphe_posY, sphe_posZ,
  tea_posX, tea_posY, tea_posZ;
  ofColor light_ambient;

  ofLight light_directional;
  ofLight light_point;
  ofLight light_spot;

  ofMaterial material_cube;
  ofMaterial material_sphere;
  ofMaterial material_teapot;


  float scale_cube;
  float scale_sphere;
  float scale_teapot;
  bool iscube, isteapot,issphere;
  ofxAssimpModelLoader teapot;

  ofVec3f position_cube;
  ofVec3f position_sphere;
  ofVec3f position_teapot;

  ofQuaternion orientation_directional;
  ofQuaternion orientation_spot;

  

  float camera_offset;

  float oscillation;


  float center_x;
  float center_y;

  float speed_motion;

  float oscillation_frequency;
  float oscillation_amplitude;

  float offset_x;
  float offset_z;

  float delta_x;
  float delta_z;

  float initial_x;
  float initial_z;

  bool is_active_ligh_ambient;
  bool is_active_light_directional;
  bool is_active_light_point;
  bool is_active_light_spot;

  bool use_smooth_lighting;

  RendererDynamicLight();

  void setup();
  void update();
  void draw();

  void reset();

  void lighting_on();
  void lighting_off();

  float oscillate(float time, float frequency, float amplitude);
};
