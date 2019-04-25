// IFT3100H19_ImageExport/applicationPlus.h
// Classe principale de l'applicationPlus.

#pragma once

#include "ofMain.h"
#include "rendererPlus.h"
#include "ofxGui.h"
#include "Imagelist.h"

class ApplicationPlus : public ofBaseApp
{
public:
//----------------------------------------------------------------
  RendererPlus rendererPlus;

  void setup();
  void draw();

  void mouseScrolled(int x, int y, float scrollX, float scrollY);
  void keyReleased(int key);
  void keyPressed(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);

  void dragEvent(ofDragInfo dragInfo);

  void exit();
  void update();


  ofxPanel gui2;

  ofxGuiGroup group_draw;

  ofParameter<ofColor> color_picker_background;
  ofParameter<ofColor> color_picker_stroke;

  ofParameter<float> slider_stroke_weight;

  ofParameter<string> textbox;

  ofParameter<bool> checkbox;

  ofxButton button;
  bool is_verbose;


  ofxGuiGroup gestion_fichiers;
  ofParameter<bool> drag_to_import;
  ofParameter<bool> save_by_clic;  
  ofxButton button_import;
  ofxButton button_export;
  ofxButton button_select_all;
  ofxButton button_deselect_all;

  ofxGuiGroup dessin_vectoriel;
  ofxButton button_delete_shapes;
  ofParameter<bool> mode_draw;
  ofParameter<bool> mode_rotate;

  ofxGuiGroup dessin_vectoriel_3D;
  ofxButton button_delete_shapes_3D;
  ofParameter<bool> mode_draw_3d;
  ofParameter<bool> mode_rotate_3d;


  void button_pressed();
  void buttonimportPressed();
  void buttonExportPressed();
  void buttonDeleteShapesPressed();
  void buttonSelectAllPressed();
  void buttonDeselectAllPressed();
  void processOpenFileSelection(ofFileDialogResult openFileResult);
  void rotation_3D();
  //bool sortColorFunction (ofColor i,ofColor j);
  vector<ofImage>loadedImages;
  vector<ofImage>processedImages;
  vector<Imagelist>images;

  string originalFileExtension;
//--------------------------------------------------------------

  bool bFill = rendererPlus.bFill;
  bool bWireframe = rendererPlus.bWireframe;
  bool bDrawNormals =rendererPlus.bDrawNormals;
  bool bDrawAxes = rendererPlus.bDrawAxes;
  bool bDrawLights= rendererPlus.bDrawLights;
  bool bHelpText=rendererPlus.bHelpText;
  bool bMousePressed=rendererPlus.bMousePressed;
  bool bSplitFaces=rendererPlus.bSplitFaces;

  ofImage texture = rendererPlus.texture;
  ofVideoGrabber vidGrabber=rendererPlus.vidGrabber;
  int mode=rendererPlus.mode;

  ofSpherePrimitive sphere=rendererPlus.sphere;
  ofIcoSpherePrimitive icoSphere=rendererPlus.icoSphere;
  ofPlanePrimitive plane=rendererPlus.plane;
  ofCylinderPrimitive cylinder=rendererPlus.cylinder;
  ofConePrimitive cone=rendererPlus.cone;
  ofBoxPrimitive box=rendererPlus.box;

  ofLight pointLight=rendererPlus.pointLight;
  ofLight pointLight2=rendererPlus.pointLight2;
  ofLight pointLight3=rendererPlus.pointLight3;
  ofMaterial material=rendererPlus.material;

  // place to store the sides of the box //
  ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
  ofVboMesh deformPlane=rendererPlus.deformPlane;
  ofVboMesh topCap=rendererPlus.topCap, bottomCap=rendererPlus.bottomCap, body=rendererPlus.body;
  vector<ofMeshFace> triangles=rendererPlus.triangles;
  ofCamera cam=rendererPlus.cam;
//-------------------------------------------------------------
};
