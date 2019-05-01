// IFT3100H19_PBR/PBR.cpp
// Classe responsable du rendu de l'application.

#include "PBR.h"

void PBR::setup()
{
  ofSetFrameRate(60);
  ofDisableArbTex();
  ofSetLogLevel(OF_LOG_VERBOSE);
  ofSetSphereResolution(32);
  mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
  // chargement d'un modèle 3D
  //--------------------------------------------------------
  model_one.loadModel("Car.obj");
  model_two.loadModel("Wolf.obj");
  model_three.loadModel("Deer.obj");

  // désactiver le matériau par défaut du modèle
  model_one.disableMaterials();
  model_two.disableMaterials();
  model_three.disableMaterials();//jires modif djr

  models[0] = &model_one;
  models[1] = &model_two;
  models[2] = &model_three;

  //Invisible tant qu'on les appelles pas.
  models[0]->setScale(0.0f, 0.0f, 0.0f);
  models[1]->setScale(0.0f, 0.0f, 0.0f);
  models[2]->setScale(0.0f, 0.0f, 0.0f);
  //------------------------------------------------------

  // charger, compiler et linker les sources des shaders
  shader_pbr.load(
    "shader/pbr_330_vs.glsl",
    "shader/pbr_330_fs.glsl");

  // charger les textures du matériau
  texture_diffuse.load("texture/rock_06_diff_1k.jpg");
  texture_metallic.load("texture/rock_06_mettallic_1k.jpg");
  texture_roughness.load("texture/rock_06_rough_1k.jpg");
  texture_occlusion.load("texture/rock_06_AO_1k.jpg");

  // paramètres des textures du matériau
  texture_diffuse.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_metallic.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_roughness.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
  texture_occlusion.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
}

void PBR::reset()
{
  // paramètres de transformation
  speed_motion = 150.0f;
  speed_rotation = 50.0f;
  initial_x = 0.0f;
  initial_y = -100.0f;
  rotation_y = 0.0f;
 
  // paramètres du matériau
  material_color_ambient = ofColor(63, 63, 63);
  material_color_diffuse = ofColor(255, 255, 255);
  material_color_specular = ofColor(255, 255, 255);

  material_metallic = 0.5f;
  material_roughness = 0.5f;
  material_occlusion = 1.0f;
  material_brightness = 1.0f;

  material_fresnel_ior = glm::vec3(0.04f, 0.04f, 0.04f);

  // paramètres de la lumière
  light_color = ofColor(255, 255, 255);
  light_intensity = 10.0f;
  light_motion = true;

  // paramètres de mappage tonal
  tone_mapping_exposure = 1.0f;
  tone_mapping_toggle = true;

  // initialisation des variables
  offset_x = initial_x;
  offset_z = initial_y;

  delta_x = speed_motion;
  delta_y = speed_rotation;
  delta_z = speed_motion;

  // position initiale de la caméra
  offset_x = initial_x;
  offset_z = initial_y;

  ofLog() << "<reset>";
}

void PBR::update()
{
  // centre de la fenêtre d'affichage
  center_x = ofGetWidth() / 2.0f;
  center_y = ofGetHeight() / 2.0f;

  if (light_motion)
  {
    // transformer la lumière
    light.setGlobalPosition(
      ofMap(ofGetMouseX() / (float) ofGetWidth(), 0.0f, 1.0f, -center_x, center_y),
      ofMap(ofGetMouseY() / (float) ofGetHeight(), 0.0f, 1.0f, -center_y, center_y),
      -offset_z * 1.5f);
  }

  // passer les attributs uniformes au shader de sommets
  shader = &shader_pbr;
  shader->begin();

  shader->setUniform3f("material_color_ambient", material_color_ambient.r/255.0f, material_color_ambient.g/255.0f, material_color_ambient.b/255.0f);
  shader->setUniform3f("material_color_diffuse", material_color_diffuse.r/255.0f, material_color_diffuse.g/255.0f, material_color_diffuse.b/255.0f);
  shader->setUniform3f("material_color_specular", material_color_specular.r/255.0f, material_color_specular.g/255.0f, material_color_specular.b/255.0f);

  shader->setUniform1f("material_brightness", material_brightness);
  shader->setUniform1f("material_metallic", material_metallic);
  shader->setUniform1f("material_roughness", material_roughness);
  shader->setUniform1f("material_occlusion", material_occlusion);

  shader->setUniform3f("material_fresnel_ior", material_fresnel_ior);

  shader->setUniformTexture("texture_diffuse", texture_diffuse.getTexture(), 1);
  shader->setUniformTexture("texture_metallic", texture_metallic.getTexture(), 2);
  shader->setUniformTexture("texture_roughness", texture_roughness.getTexture(), 3);
  shader->setUniformTexture("texture_occlusion", texture_occlusion.getTexture(), 4);

  shader->setUniform1f("light_intensity", light_intensity);
  shader->setUniform3f("light_color", light_color.r/255.0f, light_color.g/255.0f, light_color.b/255.0f);
  shader->setUniform3f("light_position", glm::vec4(light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));

  shader->setUniform1f("tone_mapping_exposure", tone_mapping_exposure);
  shader->setUniform1f("tone_mapping_gamma", tone_mapping_gamma);
  shader->setUniform1i("tone_mapping_toggle", tone_mapping_toggle);

  shader->end();
}

void PBR::draw()
{
	// activer l'occlusion en profondeur
	ofEnableDepthTest();

	// activer l'éclairage dynamique
	ofEnableLighting();

	// activer la lumière dynamique
	light.enable();

	// activer le shader
	shader->begin();

	ofPushMatrix();

	// transformer l'origine de la scène au milieu de la fenêtre d'affichage
	//ofTranslate(center_x + offset_x, center_y, offset_z);

	// rotation globale
	ofRotateDeg(rotation_y, 0.0f, 1.0f, 0.0f);



	ofPushMatrix();

	// positionner 
	//models[0]->setPosition(
		//position_teapot.x + 10.0f,
		//position_teapot.y + 15.0f,
		//position_teapot.z);

	// dimension
	models[0]->setScale(
		0.3,
		-0.3,
		0.3);

	// dessiner
	models[0]->draw(OF_MESH_FILL);

	ofPopMatrix();
	
	
	ofPushMatrix();

	// positionner
	//models[1]->setPosition(
		//position_teapot.x + 15.0f,
		//position_teapot.y + 15.0f,
		//position_teapot.z);

	// dimension
	models[1]->setScale(
		0.3,
		-0.3,
		0.3);

	// dessiner un teapot
	models[1]->draw(OF_MESH_FILL);

	ofPopMatrix();
	
	
	
	
	
	
	
	
	
	
	
	
	ofPushMatrix();

	// positionner 
	//models[2]->setPosition(
		//position_teapot.x + 25.0f,
		//position_teapot.y + 15.0f,
		//position_teapot.z);

	// dimension
	models[2]->setScale(
		0.3,
		-0.3,
		0.3);

	// dessiner 
	models[2]->draw(OF_MESH_FILL);

	ofPopMatrix();

	ofPopMatrix();

	// désactiver le shader
	shader->end();

	// désactiver la lumière
	light.disable();

	// désactiver l'éclairage dynamique
	ofDisableLighting();

	// désactiver l'occlusion en profondeur
	ofDisableDepthTest();
}

// Fonction qui efface les modeles 3D.
void PBR::model_reset() {
	models[0]->setScale(0.0f, 0.0f, 0.0f);
	models[1]->setScale(0.0f, 0.0f, 0.0f);
	models[2]->setScale(0.0f, 0.0f, 0.0f);

	addedModels[0] = false;
	addedModels[1] = false;
	addedModels[2] = false;

}
// Fonction qui ajoute un modele 3D.
void PBR::add_3d_model(ModelToDrawPBR model) {


	vector_position.x = mouse_current_x;

	vector_position.y = mouse_current_y;

	vector_position.z = 0.0f;


	vector_proportion.x = 0.1f;

	vector_proportion.y = 0.1f;

	vector_proportion.z = 0.1f;


	switch (model)

	{

	case ModelToDrawPBR::modelOne:

		models[0]->setPosition(vector_position.x, vector_position.y, vector_position.z);

		models[0]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);

		break;

	case ModelToDrawPBR::modelTwo:

		models[1]->setPosition(vector_position.x, vector_position.y, vector_position.z);

		models[1]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);

		break;

	case ModelToDrawPBR::modelThree:

		models[2]->setPosition(vector_position.x, vector_position.y, vector_position.z);

		models[2]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);

		break;

	default:

		break;

	}
}

void PBR::get_bounding_box(int model) {

	ofMesh mesh = models[model]->getMesh(0);

	float min_x, max_x, min_y, max_y, min_z, max_z;

	min_x = max_x = mesh.getVertex(0).x;

	min_y = max_y = mesh.getVertex(0).y;

	min_z = max_z = mesh.getVertex(0).z;



	for (int i = 0; i < mesh.getNumVertices(); i++) {

		if (mesh.getVertex(i).x < min_x) min_x = mesh.getVertex(i).x;

		if (mesh.getVertex(i).x > max_x) max_x = mesh.getVertex(i).x;

		if (mesh.getVertex(i).y < min_y) min_y = mesh.getVertex(i).y;

		if (mesh.getVertex(i).y > max_y) max_y = mesh.getVertex(i).y;

		if (mesh.getVertex(i).z < min_z) min_z = mesh.getVertex(i).z;

		if (mesh.getVertex(i).z > max_z) max_z = mesh.getVertex(i).z;

	}



	size_box = ofVec3f(max_x - min_x, max_y - min_y, max_z - min_z);

}