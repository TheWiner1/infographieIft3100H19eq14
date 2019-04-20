// IFT3100H19_CurveBezier/renderer.cpp
// Classe responsable du rendu de l'application.

#include "topoParametrique.h"

void TopoParametrique::setup()
{
	// Courbe parametrique
	afficher_courbe_parametrique = afficher_surface_parametrique = false;

  ofSetSphereResolution(32);
  ofDisableDepthTest();

  line_resolution = 100;
  line_width_outline = 2.0f;
  line_width_curve = 4.0f;

  radius = 16.0f;

  motion_speed = 250.0f;

  for (index = 0; index <= line_resolution; ++index)
    line_renderer.addVertex(ofPoint());

	selected_ctrl_point = &ctrl_points[0];

  reset();

	// Surface parametrique
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glMapGrid2f(20, 0, 1, 20, 0, 1);
}

void TopoParametrique::reset()
{
  framebuffer_width  = ofGetWidth();
  framebuffer_height = ofGetHeight();

  // ratios de positionnement dans la fenêtre
  float w_1_6 = framebuffer_width / 6.0f;
  float w_1_4 = framebuffer_width / 4.0f;
  float w_1_3 = framebuffer_width / 3.0f;
	float w_2_3 = framebuffer_width * 2.0f / 3.0f;
  float w_3_4 = framebuffer_width * 3.0f / 4.0f;
  float w_5_6 = framebuffer_width * 5.0f / 6.0f;
  float h_1_5 = framebuffer_height / 5.0f;
  float h_1_3 = framebuffer_height / 3.0f;
  float h_4_5 = framebuffer_height * 4.0f / 5.0f;

  initial_position1 = {w_1_6, h_4_5, 0};
  initial_position2 = {w_1_4, h_1_3, 0};
  initial_position3 = {w_1_3, h_1_5, 0};
  initial_position4 = {w_2_3, h_1_5, 0};
  initial_position5 = {w_3_4, h_1_3, 0};
	initial_position6 = {w_5_6, h_4_5, 0};

	ctrl_points[0] = initial_position1;
	ctrl_points[1] = initial_position2;
	ctrl_points[2] = initial_position3;
	ctrl_points[3] = initial_position4;
	ctrl_points[4] = initial_position5;
	ctrl_points[5] = initial_position6;

  delta_x = motion_speed;
  delta_y = motion_speed;
}

void TopoParametrique::update()
{
  for (index = 0; index <= line_resolution; ++index)
  {
		bezier_6_points(
			index / (float)line_resolution,
			ctrl_points[0].x, ctrl_points[0].y, ctrl_points[0].z,
			ctrl_points[1].x, ctrl_points[1].y, ctrl_points[1].z,
			ctrl_points[2].x, ctrl_points[2].y, ctrl_points[2].z,
			ctrl_points[3].x, ctrl_points[3].y, ctrl_points[3].z,
			ctrl_points[4].x, ctrl_points[4].y, ctrl_points[4].z,
			ctrl_points[5].x, ctrl_points[5].y, ctrl_points[5].z,
			position.x, position.y, position.z);

    line_renderer[index] = position;
  }
}

void TopoParametrique::draw()
{
	if (afficher_courbe_parametrique) {
		ofPushMatrix();
		ofTranslate(130, 0);
		ofFill();

		// dessiner la ligne contour
		ofSetColor(0, 0, 200);
		ofSetLineWidth(line_width_outline);
		ofDrawLine(ctrl_points[0].x, ctrl_points[0].y, ctrl_points[1].x, ctrl_points[1].y);
		ofDrawLine(ctrl_points[1].x, ctrl_points[1].y, ctrl_points[2].x, ctrl_points[2].y);
		ofDrawLine(ctrl_points[2].x, ctrl_points[2].y, ctrl_points[3].x, ctrl_points[3].y);
		ofDrawLine(ctrl_points[3].x, ctrl_points[3].y, ctrl_points[4].x, ctrl_points[4].y);
		ofDrawLine(ctrl_points[4].x, ctrl_points[4].y, ctrl_points[5].x, ctrl_points[5].y);
		ofDrawLine(ctrl_points[5].x, ctrl_points[5].y, ctrl_points[0].x, ctrl_points[0].y);

		// dessiner la courbe
		ofSetColor(0, 0, 0);
		ofSetLineWidth(line_width_curve);
		line_renderer.draw();

		// dessiner les points de contrôle
		ofSetColor(200, 0, 0);
		ofDrawEllipse(ctrl_points[0].x, ctrl_points[0].y, radius, radius);
		ofDrawEllipse(ctrl_points[1].x, ctrl_points[1].y, radius, radius);
		ofDrawEllipse(ctrl_points[2].x, ctrl_points[2].y, radius, radius);
		ofDrawEllipse(ctrl_points[3].x, ctrl_points[3].y, radius, radius);
		ofDrawEllipse(ctrl_points[4].x, ctrl_points[4].y, radius, radius);
		ofDrawEllipse(ctrl_points[5].x, ctrl_points[5].y, radius, radius);

		ofSetColor(0, 200, 0);
		ofDrawEllipse(selected_ctrl_point->x, selected_ctrl_point->y, radius, radius);

		ofPopMatrix();
	}

	if (afficher_surface_parametrique) {
		ofPushMatrix();
		ofFill();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofSetColor(150);
		glEvalMesh2(GL_FILL, 0, 20, 0, 20);
		ofPopMatrix();
	}
}

void TopoParametrique::bezier_6_points(
	float t,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float p3x, float p3y, float p3z,
	float p4x, float p4y, float p4z,
	float p5x, float p5y, float p5z,
	float p6x, float p6y, float p6z,
	float&  x, float& y, float&  z)
{
	float u = 1 - t;
	float uu = u * u;
	float uuu = uu * u;
	float uuuu = uuu * u;
	float uuuuu = uuuu * u;
	float tt = t * t;
	float ttt = tt * t;
	float tttt = ttt * t;
	float ttttt = tttt * t;

	x = uuuuu * p1x + 5 * t * uuuu * p2x + 10 * tt * uuu * p3x + 10 * ttt * uu * p4x + 5 * tttt * u * p5x + ttttt * p6x;
	y = uuuuu * p1y + 5 * t * uuuu * p2y + 10 * tt * uuu * p3y + 10 * ttt * uu * p4y + 5 * tttt * u * p5y + ttttt * p6y;
	z = uuuuu * p1z + 5 * t * uuuu * p2z + 10 * tt * uuu * p3z + 10 * ttt * uu * p4z + 5 * tttt * u * p5z + ttttt * p6z;
}