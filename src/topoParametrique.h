#pragma once

#include "ofMain.h"

class TopoParametrique
{
public:
  ofPolyline line_renderer;

  ofVec3f* selected_ctrl_point;

	ofVec3f ctrl_points[6];

  ofVec3f initial_position1;
  ofVec3f initial_position2;
  ofVec3f initial_position3;
  ofVec3f initial_position4;
  ofVec3f initial_position5;
	ofVec3f initial_position6;

  ofVec3f position;

  float line_width_outline;
  float line_width_curve;

  float radius;

  float delta_x;
  float delta_y;

  float motion_speed;

  int framebuffer_width;
  int framebuffer_height;

  int line_resolution;

  int index;

	bool afficher_courbe_parametrique, afficher_surface_parametrique;

  void setup();
  void update();
  void draw();
  void reset();
	void bezier_6_points(
		float t,
		float p1x, float p1y, float p1z,
		float p2x, float p2y, float p2z,
		float p3x, float p3y, float p3z,
		float p4x, float p4y, float p4z,
		float p5x, float p5y, float p5z,
		float p6x, float p6y, float p6z,
		float&  x, float& y, float&  z);
};
