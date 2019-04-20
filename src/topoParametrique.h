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

	GLfloat ctrlpoints[4][4][3] = {
	 {{-50, -150, 300}, {-50, -150, 200},
		{50, -150, -100}, {150, -150, 200}},
	 {{-50, -50, 100}, {-50, -50, 300},
		{50, -50, 0}, {150, -50, -10}},
	 {{-150, 50, 300}, {-50, 50, 0},
		{50, 50, 300}, {150, 50, 300}},
	 {{-150, 150, -200}, {-50, 150, -200},
		{50, 150, 0}, {150, 150, -100}}
	};

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
