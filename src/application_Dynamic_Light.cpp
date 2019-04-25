// IFT3100H19_Lighting/applicationDynamicLight.cpp
// Classe principale de l'applicationDynamicLight.

#include "application_Dynamic_Light.h"

ApplicationDynamicLight::ApplicationDynamicLight()
{
  is_key_press_up = false;
  is_key_press_down = false;
  is_key_press_left = false;
  is_key_press_right = false;
  
}

void ApplicationDynamicLight::setup()
{/*
  ofLog() << "<app::setup>";

  ofSetWindowTitle("éclairage dynamique (1-4 ↑↓←→ s r)");*/

  rendererDynamicLight.setup();
}

void ApplicationDynamicLight::update()
{
  time_current = ofGetElapsedTimef();
  time_elapsed = time_current - time_last;
  time_last = time_current;

  if (is_key_press_up)
    rendererDynamicLight.offset_z += rendererDynamicLight.delta_z * time_elapsed;
  if (is_key_press_down)
    rendererDynamicLight.offset_z -= rendererDynamicLight.delta_z * time_elapsed;
  if (is_key_press_left)
    rendererDynamicLight.offset_x += rendererDynamicLight.delta_x * time_elapsed;
  if (is_key_press_right)
    rendererDynamicLight.offset_x -= rendererDynamicLight.delta_x * time_elapsed;

  rendererDynamicLight.update();
}

void ApplicationDynamicLight::draw()
{
  rendererDynamicLight.draw();
}

void ApplicationDynamicLight::mouseReleased(int x, int y, int button)
{
  rendererDynamicLight.reset();
}

void ApplicationDynamicLight::keyPressed(int key)
{
  switch (key)
  {
    case OF_KEY_LEFT: // key ←
      is_key_press_left = true;
      break;

    case OF_KEY_UP: // key ↑
      is_key_press_up = true;
      break;

    case OF_KEY_RIGHT: // key →
      is_key_press_right = true;
      break;

    case OF_KEY_DOWN: // key ↓
      is_key_press_down = true;
      break;

    default:
      break;
  }
}

void ApplicationDynamicLight::keyReleased(int key)
{
  switch (key)
  {
    /*
    case 49: // key 1
      rendererDynamicLight.is_active_ligh_ambient = !rendererDynamicLight.is_active_ligh_ambient;
      ofLog() << "<light ambient: " << rendererDynamicLight.is_active_ligh_ambient << ">";
      break;

    case 50: // key 2
      rendererDynamicLight.is_active_light_directional = !rendererDynamicLight.is_active_light_directional;
      ofLog() << "<light directional: " << rendererDynamicLight.is_active_light_directional << ">";
      break;

    case 51: // key 3
      rendererDynamicLight.is_active_light_point = !rendererDynamicLight.is_active_light_point;
      ofLog() << "<light point: " << rendererDynamicLight.is_active_light_point << ">";
      break;

    case 52: // key 4
      rendererDynamicLight.is_active_light_spot = !rendererDynamicLight.is_active_light_spot;
      ofLog() << "<light spot: " << rendererDynamicLight.is_active_light_spot << ">";
      break;
      */

    case 114: // key r
      rendererDynamicLight.reset();
      break;

    case 115: // key s
      rendererDynamicLight.use_smooth_lighting = !rendererDynamicLight.use_smooth_lighting;
      ofSetSmoothLighting(rendererDynamicLight.use_smooth_lighting);
      ofLog() << "<smooth lighting: " << rendererDynamicLight.use_smooth_lighting << ">";
      break;

    case OF_KEY_LEFT: // key ←
      is_key_press_left = false;
      break;

    case OF_KEY_UP: // key ↑
      is_key_press_up = false;
      break;

    case OF_KEY_RIGHT: // key →
      is_key_press_right = false;
      break;

    case OF_KEY_DOWN: // key ↓
      is_key_press_down = false;
      break;

    default:
      break;
  }
}

void ApplicationDynamicLight::windowResized(int w, int h)
{
  rendererDynamicLight.reset();

  //ofLog() << "<app::windowResized: (" << w << ", " << h << ")>";
}

void ApplicationDynamicLight::exit()
{
  //ofLog() << "<app::exit>";
}
