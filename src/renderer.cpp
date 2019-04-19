#include "renderer.h"

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
	ofEnableLighting();

	light.setDiffuseColor(ofColor(255, 255, 255));
	light.setPosition(0.0f, 0.0f, 1000.0f);
	light.enable();

	model_count = 3;

	model_one.loadModel("Car.obj");
	model_two.loadModel("Wolf.obj");
	model_three.loadModel("Deer.obj");

	models[0] = &model_one;
	models[1] = &model_two;
	models[2] = &model_three;

	//Invisible tant qu'on les appelles pas.
	models[0]->setScale(0.0f, 0.0f, 0.0f);
	models[1]->setScale(0.0f, 0.0f, 0.0f);
	models[2]->setScale(0.0f, 0.0f, 0.0f);

	// Filtrage
	filtrage = Filtrage::none;
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
}

void Renderer::draw()
{
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

	// Dessine les modeles 3D.
	ofPushMatrix();
	for (int index = 0; index < model_count; ++index) {
		ofEnableDepthTest();
		models[index]->draw(OF_MESH_FILL);
		ofDisableDepthTest();

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

	// Dessine les curseurs dynamiques.
	draw_cursor(mouse_current_x, mouse_current_y);
}

// Fonction qui defini le remplissage de la figure.
void Renderer::remplissage()
{
	if (check)
		ofFill();
	else
		ofNoFill();
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
		break;
	case ModelToDraw::modelTwo:
		models[1]->setPosition(vector_position.x, vector_position.y, vector_position.z);
		models[1]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
		break;
	case ModelToDraw::modelThree:
		models[2]->setPosition(vector_position.x, vector_position.y, vector_position.z);
		models[2]->setScale(vector_proportion.x, vector_proportion.y, vector_proportion.z);
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

Renderer::~Renderer()
{
	std::free(shapes);
}