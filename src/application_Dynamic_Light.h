// IFT3100H19_Lighting/applicationDynamicLight.h
// Classe principale de l'applicationDynamicLight.

#pragma once

#include "ofMain.h"
#include "renderer_Dynamic_Light.h"

class ApplicationDynamicLight : public ofBaseApp
{
public:

  RendererDynamicLight rendererDynamicLight;

  float time_current;
  float time_last;
  float time_elapsed;

  bool is_key_press_up;
  bool is_key_press_down;
  bool is_key_press_left;
  bool is_key_press_right;

  ApplicationDynamicLight();

  void setup();

  void update();

  void draw();

  void keyPressed(int key);

  void keyReleased(int key);

  void mouseReleased(int x, int y, int button);

  void windowResized(int w, int h);

  void exit();
};
