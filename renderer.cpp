#include "renderer.h"

/*void Renderer::setupCamera() {
	ofEnableDepthTest();
	
	camera_near = 50.0f;
	camera_far = 1550.0f;

	camera_fov = 60.0f;
	camera_fov_delta = 16.0f;

	camera_front.setPosition(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 300.0f);

	projection();
}*/

void Renderer::setup()
{
	ofSetFrameRate(60);

	shapeCount = 100;
	head = 0;
	size = shapeCount * sizeof(VectorPrimitive);
	shapes = (VectorPrimitive*)std::malloc(size);

	radius = 2.0f;
	draw_mode = VectorPrimitiveType::none;
	mouse_press_x = mouse_press_y = mouse_current_x = mouse_current_y = 0;
	is_mouse_button_pressed = false;

	// 5.5 Texture procédural setup.
	w_texture = 250;
	h_texture = 200;
	grayPixels.allocate(w_texture, h_texture, OF_PIXELS_GRAY);
	colorPixels.allocate(w_texture, h_texture, OF_PIXELS_RGB);
	colorAlphaPixels.allocate(w_texture, h_texture, OF_PIXELS_RGBA);
	for (int i = 0; i < w_texture*h_texture; i++) {
		grayPixels[i] = ofRandomuf() * 255;

	}
	for (int y = 0; y < h_texture; y++) {
		for (int x = 0; x < w_texture; x++) {
			colorPixels.setColor(x, y, ofColor(x, y, 0));
		}
	}
	for (int y = 0; y < h_texture; y++) {
		for (int x = 0; x < w_texture; x++) {
			colorAlphaPixels.setColor(x, y, ofColor(255, 133, 200, x));
		}
	}
	texGray.allocate(grayPixels);
	texColor.allocate(colorPixels);
	texColorAlpha.allocate(colorAlphaPixels);

	// 4.2 Primitives geometriques setup.
	//Pyramide
	ofPoint v0 = ofPoint(-200, -100, 0);
	ofPoint v1 = ofPoint(200, -100, 0);
	ofPoint v2 = ofPoint(0, 200, 0);

	ofPoint v3 = ofPoint(0, 0, 100);

	mesh.addVertex(v3); mesh.addVertex(v2); mesh.addVertex(v0);
	mesh.addVertex(v3); mesh.addVertex(v1); mesh.addVertex(v2);
	mesh.addVertex(v3); mesh.addVertex(v0); mesh.addVertex(v1);
	mesh.setupIndicesAuto(); //Set up indices

	//Sphere
	nTri = 1500;
	nVert = nTri * 3;
	float Rad = 250; //rayon de notre sphère

	//(déviation maximale des coordonnées du centre du triangle)
	float rad = 25; // distance entre 2 triangles 

	// Remplit le tableau de sommets
	vertices.resize(nVert); //initialisation de la taille
	for (int i = 0; i < nTri; i++) {
		// Génère le centre du triangle comme un point aléatoire sur la sphère

		// Prends le point aléatoire de cube [-1,1] x [-1,1] x [-1,1]
		ofPoint center(ofRandom(-1, 1),
			ofRandom(-1, 1),
			ofRandom(-1, 1));
		center.normalize(); //Normalise la longueur du vecteur à 1
		center *= Rad;
		// Le vecteur du centre a maintenant la longueur Rad. 

		//on Génère les sommets du triangle en tant
		// que centre, plus un point aléatoire à partir de [- rad, rad] x [-rad, rad] x [-rad, rad]
		for (int j = 0; j < 3; j++) {
			vertices[i * 3 + j] = center + ofPoint(ofRandom(-rad, rad),
				ofRandom(-rad, rad),
				ofRandom(-rad, rad));
		}
	}

	//Remplit le tableau des couleurs des triangles
	colors.resize(nTri);
	for (int i = 0; i < nTri; i++) {
		//on va prendre entre le rouge et du noir
		colors[i] = ofColor(ofRandom(0, 255), 0, 0);
	}

	// 4.3 Modeles 3D setup.
	/*ofEnableLighting();

	light.setDiffuseColor(ofColor(255, 255, 255));
	light.setPosition(0.0f, 0.0f, 1000.0f);
	light.enable();^^^^^^^%&^^6??????????????
	*/
	model_count = 3;
	
	/*model_one.loadModel("Car.obj");
	model_two.loadModel("Wolf.obj");
	model_three.loadModel("Deer.obj");//jires .disableMaterials();

	models[0] = &model_one;
	models[1] = &model_two;
	models[2] = &model_three;

	//Invisible tant qu'on les appelles pas.
	models[0]->setScale(0.0f, 0.0f, 0.0f);
	models[1]->setScale(0.0f, 0.0f, 0.0f);
	models[2]->setScale(0.0f, 0.0f, 0.0f);*/

	// Filtrage
	filtrage = Filtrage::none;

	/*Alex*/
	speed_delta = 100.0f;
	is_camera_perspective = true;
	//default
	material_0.setAmbientColor(ofFloatColor(0.1f, 0.1f, 0.1f));
	material_0.setDiffuseColor(ofFloatColor(0.6f, 0.6f, 0.6f));
	material_0.setSpecularColor(ofFloatColor(1.0f, 1.0f, 0.0f));
	material_0.setShininess(0.6f);
	//emerald
	material_1.setAmbientColor(ofFloatColor(0.0215f, 0.1745f, 0.0215f));
	material_1.setDiffuseColor(ofFloatColor(0.07568f, 0.61424f, 0.07568f));
	material_1.setSpecularColor(ofFloatColor(0.633f, 0.727811f, 0.633f));
	material_1.setShininess(0.6f);

	//ruby
	material_2.setAmbientColor(ofFloatColor(0.1745f, 0.01175f, 0.01175f));
	material_2.setDiffuseColor(ofFloatColor(0.61424f, 0.04136f, 0.04136f));
	material_2.setSpecularColor(ofFloatColor(0.727811f, 0.626959f, 0.626959f));
	material_2.setShininess(0.6f);
	// silver
	material_3.setAmbientColor(ofFloatColor(0.19225f, 0.19225f, 0.19225f));
	material_3.setDiffuseColor(ofFloatColor(0.50754f, 0.50754f, 0.50754f));
	material_3.setSpecularColor(ofFloatColor(0.508273f, 0.508273f, 0.508273f));
	material_3.setShininess(0.4f);
	// gold
	material_4.setAmbientColor(ofFloatColor(0.24725f, 0.1995f, 0.0745f));
	material_4.setDiffuseColor(ofFloatColor(0.75164f, 0.60648f, 0.22648f));
	material_4.setSpecularColor(ofFloatColor(0.628281f, 0.555802f, 0.366065f));
	material_4.setShininess(0.4f);

	material_current = material_0;

	mirror_z = -900.0f;
	v_plane.setPosition(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, mirror_z);
	v_plane.set(100000, 2000);
	v_plane.setResolution(20, 20);
	v_plane.rotate(0, 1, 0, 0);

	material_v_plane.setDiffuseColor(ofColor::aliceBlue);
	material_v_plane.setShininess(0.8);
	ofEnableAlphaBlending();


	/*Alex*/
}

void Renderer::setupIllumination() {
	//ofEnableDepthTest();
	ofSetLogLevel(OF_LOG_VERBOSE);

	// paramètres
	oscillation_amplitude = 42.0f;//32
	oscillation_frequency = 7500.0f;

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

	/*Alex*/
	//Mirror

	model_one_mirror.loadModel("Car.obj");
	model_two_mirror.loadModel("Wolf.obj");
	model_three_mirror.loadModel("Deer.obj");

	// désactiver le matériau par défaut du modèle
	model_one_mirror.disableMaterials();
	model_two_mirror.disableMaterials();
	model_three_mirror.disableMaterials();

	modelsMirror[0] = &model_one_mirror;
	modelsMirror[1] = &model_two_mirror;
	modelsMirror[2] = &model_three_mirror;

	//Invisible tant qu'on les appelles pas.
	modelsMirror[0]->setScale(0.0f, 0.0f, 0.0f);
	modelsMirror[1]->setScale(0.0f, 0.0f, 0.0f);
	modelsMirror[2]->setScale(0.0f, 0.0f, 0.0f);

	/*Alex End*/
	// charger, compiler et linker les sources des shaders
	shader_color_fill.load(
		"shader/color_fill_330_vs.glsl",
		"shader/color_fill_330_fs.glsl");

	shader_lambert.load(
		"shader/lambert_330_vs.glsl",
		"shader/lambert_330_fs.glsl");

	shader_gouraud.load(
		"shader/gouraud_330_vs.glsl",
		"shader/gouraud_330_fs.glsl");

	shader_phong.load(
		"shader/phong_330_vs.glsl",
		"shader/phong_330_fs.glsl");

	shader_blinn_phong.load(
		"shader/blinn_phong_330_vs.glsl",
		"shader/blinn_phong_330_fs.glsl");

	// shader actif au lancement de la scène
	shader_active = ShaderType::blinn_phong;
}

void Renderer::update()
{
	if (affiche_tex) {
		for (int i = 0; i < w_texture; i++) {
			for (int j = 0; j < h_texture; j++) {
				grayPixels[j*w_texture + i] = (unsigned char)(ofRandomuf() * 255);
			}
		}
		texGray.loadData(grayPixels.getData(), w_texture, h_texture, GL_LUMINANCE);
	}

	camera_position = camera->getGlobalPosition();
	camera_to_mirror_angle = camera_position.angle(ofVec3f(1, 0, 0).normalize());

	modelsMirror[0]->setRotation(1.0f, +camera_to_mirror_angle/2, 0.0f, 1.0f, 0.0f);
	modelsMirror[1]->setRotation(1.0f, 180 - camera_to_mirror_angle / 2, 0.0f, 1.0f, 0.0f);
	modelsMirror[2]->setRotation(1.0f, 180 - camera_to_mirror_angle / 2, 0.0f, 1.0f, 0.0f);
}

void Renderer::updateIllumination()
{
	// transformer la lumière
	light.setGlobalPosition(
		ofMap(ofGetMouseX() / (float)ofGetWidth(), 0.0f, 1.0f, -ofGetWidth() / 2.0f, ofGetWidth() / 2.0f),
		ofMap(ofGetMouseY() / (float)ofGetHeight(), 0.0f, 1.0f, -ofGetHeight() / 2.0f, ofGetHeight() / 2.0f),
		-100.0f * 1.5f);//100 = offset_z

	// mise à jour d'une valeur numérique animée par un oscillateur
	float oscillation = oscillate(ofGetElapsedTimeMillis(), oscillation_frequency, oscillation_amplitude) + oscillation_amplitude;

	// passer les attributs uniformes au shader de sommets
	switch (shader_active)
	{
	case ShaderType::color_fill:
		shader_name = "Color Fill";
		shader = &shader_color_fill;
		shader->begin();
		shader->setUniform3f("color", glm::vec3(material_current.getAmbientColor().r, material_current.getAmbientColor().g, 0.0f)); /*Alex*/
		shader->end();
		break;

	case ShaderType::lambert:
		shader_name = "Lambert";
		shader = &shader_lambert;
		shader->begin();
		shader->setUniform3f("color_ambient", glm::vec3(material_current.getAmbientColor().r, material_current.getAmbientColor().g, material_current.getAmbientColor().b)); 
		shader->setUniform3f("color_diffuse", glm::vec3(material_current.getDiffuseColor().r, material_current.getDiffuseColor().g, material_current.getDiffuseColor().b));
		shader->setUniform3f("light_position", glm::vec4(light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
		shader->end();
		break;

	case ShaderType::gouraud:
		shader_name = "Gouraud";
		shader = &shader_gouraud;
		shader->begin();
		shader->setUniform3f("color_ambient", glm::vec3(material_current.getAmbientColor().r, material_current.getAmbientColor().g, material_current.getAmbientColor().b));
		shader->setUniform3f("color_diffuse", glm::vec3(material_current.getDiffuseColor().r, material_current.getDiffuseColor().g, material_current.getDiffuseColor().b));
		shader->setUniform3f("color_specular", glm::vec3(material_current.getSpecularColor().r, material_current.getSpecularColor().g, material_current.getSpecularColor().b));
		shader->setUniform1f("brightness", oscillation);
		shader->setUniform3f("light_position", glm::vec4(light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
		shader->end();
		break;

	case ShaderType::phong:
		shader_name = "Phong";
		shader = &shader_phong;
		shader->begin();
		shader->setUniform3f("color_ambient", glm::vec3(material_current.getAmbientColor().r, material_current.getAmbientColor().g, material_current.getAmbientColor().b));
		shader->setUniform3f("color_diffuse", glm::vec3(material_current.getDiffuseColor().r, material_current.getDiffuseColor().g, material_current.getDiffuseColor().b));
		shader->setUniform3f("color_specular", glm::vec3(material_current.getSpecularColor().r, material_current.getSpecularColor().g, material_current.getSpecularColor().b));
		shader->setUniform1f("brightness", oscillation);
		shader->setUniform3f("light_position", glm::vec4(light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
		shader->end();
		break;

	case ShaderType::blinn_phong:
		shader_name = "Blinn-Phong";
		shader = &shader_blinn_phong;
		shader->begin();
		shader->setUniform3f("color_ambient", glm::vec3(material_current.getAmbientColor().r, material_current.getAmbientColor().g, material_current.getAmbientColor().b));
		shader->setUniform3f("color_diffuse", glm::vec3(material_current.getDiffuseColor().r, material_current.getDiffuseColor().g, material_current.getDiffuseColor().b));
		shader->setUniform3f("color_specular", glm::vec3(material_current.getSpecularColor().r, material_current.getSpecularColor().g, material_current.getSpecularColor().b));
		shader->setUniform1f("brightness", oscillation);
		shader->setUniform3f("light_position", glm::vec4(light.getGlobalPosition(), 0.0f) * ofGetCurrentMatrix(OF_MATRIX_MODELVIEW));
		shader->end();
		break;

	default:
		break;
	}
}

void Renderer::updateCamera() {

	/*Alex*/
	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;

	speed_translation = speed_delta * time_elapsed;
	speed_rotation = speed_translation / 8.0f;


	if (is_camera_move_left)
		camera->truck(-speed_translation);
	if (is_camera_move_right)
		camera->truck(speed_translation);

	if (is_camera_move_up)
		camera->boom(speed_translation);
	if (is_camera_move_down)
		camera->boom(-speed_translation);

	if (is_camera_move_forward)
	{
		camera->dolly(-speed_translation * 2.0f);
	}
	if (is_camera_move_backward)
		camera->dolly(speed_translation * 5.0f);

	if (is_camera_roll_left)
		camera->rollDeg(-speed_rotation);
	if (is_camera_roll_right)
		camera->rollDeg(speed_rotation);

	if (is_camera_pan_left)
		camera->panDeg(speed_rotation);
	if (is_camera_pan_right)
		camera->panDeg(-speed_rotation); 
	if (is_camera_look_at)
	{
		if (selectedModel == 0)
		{
			if (addedModels[0] == true && addedModels[1] == true && addedModels[1] == true)
			{
				camera->lookAt((modelsMirror[0]->getPosition() + modelsMirror[1]->getPosition() + modelsMirror[2]->getPosition()) / 3.0f);
			}
			else
			{
				//camera->lookAt(ofVec3f(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 0.0f));
				camera->lookAt(ofVec3f(0, 0, 0));
			}
		}
		else
		{
			for (int index = 0; index < model_count; ++index)
			{
				if (addedModels[index] && selectedModel == index + 1)
				{
					camera->lookAt(modelsMirror[index]->getPosition());
				}
			}
		}
	}
	else
	{
		//camera->lookAt(ofVec3f(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 0.0f));
		//camera->lookAt(v_plane.getPosition());
	}

	/*Alex*/
	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	if (is_camera_perspective)
	{
		if (is_camera_fov_narrow)
		{
			camera_fov = std::max(camera_fov -= camera_fov_delta * time_elapsed, 0.0f);
			camera->setFov(camera_fov);
		}

		if (is_camera_fov_wide)
		{
			camera_fov = std::min(camera_fov += camera_fov_delta * time_elapsed, 180.0f);
			camera->setFov(camera_fov);
		}
	}
}

void Renderer::draw()
{
	// activer la caméra
	camera->begin();

	ofSetBackgroundColor(background_color);

	// Dessin d'une image importee.
	ofSetColor(255);
	//Définir les pixels de l'image
	image.setFromPixels(import.getPixelsRef());
	//convertir en image de niveau gris
	grayImage = image;
	filtered = grayImage;
	ofVec2f drawImageLocation = ofVec2f((ofGetWindowWidth() - image.getWidth()) / 2, (ofGetWindowHeight() - image.getHeight()) / 2);
	
	switch (filtrage)
	{
	case Filtrage::none:
		import.draw(drawImageLocation);
		break;
	case Filtrage::grayscale:
		grayImage.draw(drawImageLocation);
		break;
	case Filtrage::seuil:
		filtered.threshold(128);
		filtered.draw(drawImageLocation);
		break;
	case Filtrage::lissage:
		filtered.blurGaussian(9);
		filtered.draw(drawImageLocation);
		break;
	case Filtrage::seuilLissage:
		filtered.threshold(128);
		filtered.blurGaussian(9);
		filtered.draw(drawImageLocation);
		break;
	default:
		break;
	}

	// Dessin de primitives vectorielles.
	for (index = 0; index < shapeCount; ++index)
	{
		switch (shapes[index].type)
		{
		case VectorPrimitiveType::pixel:

			ofFill();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].stroke_color);
			draw_pixel(
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::point:

			ofFill();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].stroke_color);
			draw_point(
				shapes[index].position2[0],
				shapes[index].position2[1],
				shapes[index].stroke_width);
			break;

		case VectorPrimitiveType::line:

			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			ofSetColor(shapes[index].stroke_color);
			draw_line(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::rectangle:

			remplissage();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].fill_color);
			draw_rectangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			ofSetColor(shapes[index].stroke_color);
			draw_rectangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::ellipse:

			remplissage();
			ofSetLineWidth(0);
			ofSetCircleResolution(48);
			ofSetColor(shapes[index].fill_color);
			draw_ellipse(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			ofSetColor(shapes[index].stroke_color);
			draw_ellipse(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		case VectorPrimitiveType::triangle:

			remplissage();
			ofSetLineWidth(0);
			ofSetColor(shapes[index].fill_color);
			draw_triangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			ofNoFill();
			ofSetLineWidth(shapes[index].stroke_width);
			ofSetColor(shapes[index].stroke_color);
			draw_triangle(
				shapes[index].position1[0],
				shapes[index].position1[1],
				shapes[index].position2[0],
				shapes[index].position2[1]);
			break;

		default:
			break;
		}
	}

	// Changement de parametres si selection multiple.
	if (selection_multiple) {
		for (index = 0; index < shapeCount; ++index) {
			shapes[index].stroke_color = stroke_color;
			shapes[index].fill_color = stroke_fill;
			shapes[index].stroke_width = epaisseur;
			if (shapes[index].type == VectorPrimitiveType::point)
				shapes[index].stroke_width = epaisseur * epaisseur;
		}
	}

	// Dessine la texture procedural.
	if (affiche_tex) {
		ofSetHexColor(0xffffff);
		texColor.draw((ofGetWidth() - w_texture) / 2, (ofGetHeight() - h_texture) / 2, w_texture, h_texture);
	}

	// Dessine les primitives geometriques.
	if (affiche_pyramide) {
		ofPushMatrix();
		ofEnableDepthTest();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);

		float time = ofGetElapsedTimef();
		float angle = time * 30;
		ofRotate(angle, 0, 1, 1);
		ofSetColor(0, 128, 0);
		mesh.draw();
		ofPopMatrix();
	}
	if (affiche_sphere) {
		//Stocke le système de coordonnées
		ofPushMatrix();

		// Activer la mise en tampon z
		ofEnableDepthTest();

		//Déplace le centre de coordonnées vers le centre de l'écran
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);

		//**********Calculer l' angle de rotation************

		//Obtenir le temps en secondes
		float time = ofGetElapsedTimef();

		// Calculer l' angle. Nous tournons à grande vitesse, 10 degrés par seconde
		float angle = time * 10;

		// Faire pivoter le système de coordonnées suivant l'axe des y
		ofRotate(angle, 0, 1, 0);

		// Dessiner les triangles
		ofFill();
		for (int i = 0; i < nTri; i++) {
			//definir la couleur
			ofSetColor(colors[i]);
			//Draw triangle
			ofTriangle(vertices[i * 3],
				vertices[i * 3 + 1],
				vertices[i * 3 + 2]);
		}

		//Restaurer le système de coordonnées
		ofPopMatrix();
	}

	// Afficher la zone de selection.
	if (is_mouse_button_pressed)
	{
		draw_zone(
			mouse_press_x,
			mouse_press_y,
			mouse_current_x,
			mouse_current_y);
	}
//===============================================ilumunation ============================================
	// activer l'éclairage dynamique
	ofEnableLighting();

	// activer la lumière dynamique
	light.enable();

	// Dessine les modeles 3D.
	ofPushMatrix();
	/*Alex*/

	for (int index = 0; index < model_count; ++index) {
		material_current = model_material[index];
		updateIllumination();
		ofEnableDepthTest();
		
		// activer le shader
		shader->begin();

		models[index]->draw(OF_MESH_FILL);//djr modiif

		if (model_mirror) {
			ofMaterial mat = material_current;
			mat.setDiffuseColor(mat.getDiffuseColor() + 0.15f*ofFloatColor(ofColor::aliceBlue));
			updateIllumination();
			mat.begin();
			modelsMirror[index]->draw(OF_MESH_FILL);
			mat.end();
			material_v_plane.begin(); //mirroir
			v_plane.draw();
			material_v_plane.end();
		}

		if (model_box) {
			ofPushMatrix();
			get_bounding_box(index);
			ofEnableDepthTest();
			ofBoxPrimitive box;
			box.set(size_box.x, size_box.y, size_box.z);
			box.setPosition(models[index]->getPosition() - ofVec3f(0, size_box.y / 2, 0));
			box.drawWireframe();
			ofDisableDepthTest();
			ofPopMatrix();
		}
	}
	ofPopMatrix();

	// désactiver le shader
	shader->end();

	// désactiver la lumière
	light.disable();

	// désactiver l'éclairage
	ofDisableDepthTest();
//=============================================fin illumination====================================================
	// Dessine les curseurs dynamiques.
	draw_cursor(mouse_current_x, mouse_current_y);

	ofPopMatrix();

	// désactiver le shader
	shader->end();

	camera->end();
}

// Fonction qui defini le remplissage de la figure.
void Renderer::remplissage()
{
	if (check)
		ofFill();
	else
		ofNoFill();
}

// fonction d'oscillation
float Renderer::oscillate(float time, float frequency, float amplitude)
{
	return sinf(time * 3.0f * PI / frequency) * amplitude;//2
}

// Fonction qui dessine le curseur dynamique.
void Renderer::draw_cursor(float x, float y) const
{
	float length = 8.0f;
	float offsetX = 12.0f;
	float offsetY = 15.0f;

	ofSetColor(0);
	ofSetLineWidth(radius);

	switch (draw_mode)
	{
	case VectorPrimitiveType::pixel:
		ofFill();
		ofDrawEllipse(x + offsetX + (length / 2), y + offsetY + (length / 2), radius, radius);
		break;

	case VectorPrimitiveType::point:
		ofFill();
		ofDrawCircle(x + offsetX + (length / 2), y + offsetY + (length / 2), radius);
		break;

	case VectorPrimitiveType::line:
		ofDrawLine(x + offsetX, y + offsetY, x + offsetX + length, y + offsetY + length);
		break;

	case VectorPrimitiveType::rectangle:
		ofNoFill();
		ofDrawRectangle(x + offsetX, y + offsetY, length, length);
		ofFill();
		ofDrawEllipse(x + offsetX, y + offsetY, radius, radius);
		ofDrawEllipse(x + offsetX, y + offsetY + length, radius, radius);
		ofDrawEllipse(x + offsetX + length, y + offsetY, radius, radius);
		ofDrawEllipse(x + offsetX + length, y + offsetY + length, radius, radius);
		break;

	case VectorPrimitiveType::ellipse:
		ofNoFill();
		ofSetCircleResolution(100);
		ofDrawCircle(x + offsetX + (length / 2), y + offsetY + (length / 2), length / 2);
		ofFill();
		break;

	case VectorPrimitiveType::triangle:
		ofNoFill();
		ofDrawTriangle(x + offsetX, y + offsetY + length, x + offsetX + length / 2, y + offsetY, x + offsetX + length, y + offsetY + length);
		ofFill();
		break;

	default:
		break;
	}
}

// Fonction qui vide le tableau de primitives vectorielles.
void Renderer::reset()
{
	for (index = 0; index < shapeCount; ++index)
		shapes[index].type = VectorPrimitiveType::none;

	head = 0;
}

// Fonction qui ajoute une primitive vectorielle au tableau.
void Renderer::add_vector_shape(VectorPrimitiveType type)
{
	shapes[head].type = type;

	shapes[head].position1[0] = mouse_press_x;
	shapes[head].position1[1] = mouse_press_y;

	shapes[head].position2[0] = mouse_current_x;
	shapes[head].position2[1] = mouse_current_y;

	shapes[head].stroke_color = stroke_color;
	shapes[head].fill_color = stroke_fill;
	shapes[head].stroke_width = epaisseur;

	// Pour donner un peu de volume au Point.
	if (shapes[head].type == VectorPrimitiveType::point)
		shapes[head].stroke_width = epaisseur * epaisseur;

	head = ++head >= shapeCount ? 0 : head;
}

// Fonction qui dessine un pixel.
void Renderer::draw_pixel(float x, float y) const
{
	int pixel_x = floorf(x);
	int pixel_y = floorf(y);

	ofDrawRectangle(pixel_x, pixel_y, 1, 1);
}

// Fonction qui dessine un point.
void Renderer::draw_point(float x, float y, float radius) const
{
	ofDrawEllipse(x, y, radius, radius);
}

// Fonction qui dessine une ligne.
void Renderer::draw_line(float x1, float y1, float x2, float y2) const
{
	ofDrawLine(x1, y1, x2, y2);
}

// Fonction qui dessine un rectangle.
void Renderer::draw_rectangle(float x1, float y1, float x2, float y2) const
{
	ofDrawRectangle(x1, y1, x2 - x1, y2 - y1);
}

// Fonction qui dessine une ellipse.
void Renderer::draw_ellipse(float x1, float y1, float x2, float y2) const
{
	float diameter_x = x2 - x1;
	float diameter_y = y2 - y1;

	ofDrawEllipse(x1 + diameter_x / 2.0f, y1 + diameter_y / 2.0f, diameter_x, diameter_y);
}

// Fonction qui dessine un triangle.
void Renderer::draw_triangle(float x1, float y1, float x2, float y2) const
{
	(y1 > y2) ? ofDrawTriangle((x2 + x1) / 2, y2, x1, y1, x2, y1) : ofDrawTriangle((x2 + x1) / 2, y1, x1, y2, x2, y2);
}

// Fonction qui dessine un aper�u de la forme.
void Renderer::draw_zone(float x1, float y1, float x2, float y2) const
{
	float x2_clamp = min(max(0.0f, x2), (float)ofGetWidth());
	float y2_clamp = min(max(0.0f, y2), (float)ofGetHeight());

	ofSetLineWidth(radius);
	ofSetColor(255);
	ofNoFill();

	switch (draw_mode)
	{
	case VectorPrimitiveType::line:
		ofDrawLine(x1, y1, x2, y2);
		break;

	case VectorPrimitiveType::rectangle:
		ofDrawRectangle(x1, y1, x2_clamp - x1, y2_clamp - y1);
		ofFill();
		ofDrawEllipse(x1, y1, radius, radius);
		ofDrawEllipse(x1, y2_clamp, radius, radius);
		ofDrawEllipse(x2_clamp, y1, radius, radius);
		ofDrawEllipse(x2_clamp, y2_clamp, radius, radius);
		break;

	case VectorPrimitiveType::ellipse:
		draw_ellipse(x1, y1, x2, y2);
		break;

	case VectorPrimitiveType::triangle:
		draw_triangle(x1, y1, x2, y2);
		break;

	default:
		break;
	}
}

// Fonction qui fait l'algorithme de mappage tonal.
void Renderer::toneMap() {
	unsigned char* pixels = import.getPixels().getData();

	int bytesPerPixels = import.getPixels().getBytesPerPixel();
	int width = import.getWidth();
	int height = import.getHeight();

	ofPixels toned_pixels;
	toned_pixels.allocate(width, height, OF_PIXELS_RGB);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float red = pixels[(j * width + i) * bytesPerPixels + 0];
			float green = pixels[(j * width + i) * bytesPerPixels + 1];
			float blue = pixels[(j * width + i) * bytesPerPixels + 2];
			glm::vec3 color = glm::vec3(red, green, blue);
			glm::vec3 tone_color = color / (color + glm::vec3(255.0f));
			tone_color = pow(tone_color, glm::vec3(1.0f / 2.2f));
			tone_color = tone_color * color;
			ofColor toned_color;
			toned_color.set(tone_color.x, tone_color.y, tone_color.z);
			toned_pixels.setColor(i, j, toned_color);
		}
	}

	import.setFromPixels(toned_pixels);
}

// Fonction qui ajoute un modele 3D.
void Renderer::add_3d_model(ModelToDraw model) {

	vector_position.x = mouse_current_x;
	vector_position.y = mouse_current_y;
	vector_position.z = 0.0f;

	vector_proportion.x = 0.5f;
	vector_proportion.y = 0.5f;
	vector_proportion.z = 0.5f;

	switch (model)
	{
	case ModelToDraw::modelOne:
		models[0]->setPosition(vector_position.x, vector_position.y, vector_position.z);
		models[0]->setRotation(0.0f, 180.0f, 0.0f, 1.0f, 0.0f);
		models[0]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);

		modelsMirror[0]->setPosition(vector_position.x, vector_position.y, vector_position.z/1.5 + mirror_z/1.5);
		//modelsMirror[0]->setRotation(0.0f, 180.0f, 0.0f, 0.0f, 1.0f);
		modelsMirror[0]->setRotation(1.0f, +camera_to_mirror_angle / 2, 0.0f, 1.0f, 0.0f);
		modelsMirror[0]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
		addedModels[0] = true;
		break;
	case ModelToDraw::modelTwo:
		models[1]->setPosition(vector_position.x, vector_position.y, vector_position.z);
		models[1]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
		//models[1]->setRotation(0.0f, 180.0f, 0.0f, 0.0f, 1.0f);

		camera_position = camera->getGlobalPosition();
		camera_to_mirror_angle = camera_position.angle(ofVec3f(0,1,0));
		modelsMirror[1]->setPosition(vector_position.x, vector_position.y, vector_position.z / 1.5 + mirror_z/1.5);
		modelsMirror[1]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
		//modelsMirror[1]->setRotation(0.0f, 180.0f, 0.0f, 0.0f, 1.0f);
		modelsMirror[1]->setRotation(1.0f, 180-camera_to_mirror_angle/2, 0.0f, 1.0f, 0.0f);

		addedModels[1] = true;
		break;
	case ModelToDraw::modelThree:
		models[2]->setPosition(vector_position.x, vector_position.y, vector_position.z);
		models[2]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
		//models[2]->setRotation(0.0f, 180.0f, 0.0f, 0.0f, 1.0f);

		modelsMirror[2]->setPosition(vector_position.x, vector_position.y, vector_position.z / 1.5 + mirror_z/1.5 );
		modelsMirror[2]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
		//modelsMirror[2]->setRotation(0.0f, 180.0f, 0.0f, 0.0f, 1.0f);
		modelsMirror[2]->setRotation(1.0f, 180 - camera_to_mirror_angle / 2, 0.0f, 1.0f, 0.0f);

		addedModels[2] = true;
		break;
	default:
		break;
	}

}

// Fonction qui efface les modeles 3D.
void Renderer::model_reset() {
	models[0]->setScale(0.0f, 0.0f, 0.0f);
	models[1]->setScale(0.0f, 0.0f, 0.0f);
	models[2]->setScale(0.0f, 0.0f, 0.0f);


	modelsMirror[0]->setScale(0.0f, 0.0f, 0.0f);
	modelsMirror[1]->setScale(0.0f, 0.0f, 0.0f);
	modelsMirror[2]->setScale(0.0f, 0.0f, 0.0f);
	addedModels[0] = false;
	addedModels[1] = false;
	addedModels[2] = false;
}

// Fonction qui retourne la grandeur de la boite de delimitation d'un modele.
void Renderer::get_bounding_box(int model) {
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

void Renderer::change_texture(int x, int y) {
	float pct = (float)x / (float)ofGetWidth();
	for (int y = 0; y < h_texture; y++) {
		for (int x = 0; x < w_texture; x++) {
			colorPixels.setColor(x, y, ofColor(x, y, pct * 255));
		}
	}
	texColor.loadData(colorPixels);
}

// Fontion qui annule la derniere primitive vectorielle dessinee.
void Renderer::undo()
{
	if (head != 0) {
		--head;
		primitiveStack.push(shapes[head]);
		shapes[head].type = VectorPrimitiveType::none;
	}
}

// Fontion qui refait la derniere primitive vectorielle annulee.
void Renderer::redo()
{
	if (!primitiveStack.empty()) {
		shapes[head].type = primitiveStack.top().type;
		primitiveStack.pop();
		++head;
	}
}

/*void Renderer::projection() {
	camera = &camera_front;
	camera_position = camera->getPosition();
	camera_orientation = camera->getOrientationQuat();

	// mode de projection de la caméra
	if (is_camera_perspective)
	{
		camera->disableOrtho();
		camera->setupPerspective(false, camera_fov, camera_near, camera_far, ofVec2f(0, 0));
		camera_projection = "perspective";
	}
	else
	{
		camera->enableOrtho();
		camera_projection = "orthogonale";
	}

	camera->setPosition(camera_position);
	camera->setOrientation(camera_orientation);
}*/

Renderer::~Renderer()
{
	std::free(shapes);
}
