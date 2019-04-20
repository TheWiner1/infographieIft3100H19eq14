#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("IFT-3100, Equipe 20");

	is_key_press_up = false;
	is_key_press_down = false;
	is_key_press_left = false;
	is_key_press_right = false;

	// Parametres de couleurs.
	int r = 255;
	int g = 255;
	int b = 0;

	int h = getHue(r, g, b);
	int s = getSaturation(r, g, b);
	int br = getBrightness(r, g, b);

	gui.setDefaultWidth(240);

	parameters.setName("Menu");
	parameters.add(check.set("Mode HSB", false));
	parameters.add(check2.set("Selection multiple", false));
	gui.setup(parameters);
	sync.setup((ofParameterGroup&)gui.getParameter(), 6666, "localhost", 6667);
	ofSetVerticalSync(true);

	color_picker_background.set("Couleur du canevas RGB", ofColor(191), ofColor(0, 0), ofColor(255, 255));
	color_picker_background_hsb.set("Couleur du canevas HSB", ofColor::fromHsb(h, s, br));

	color_picker_fill.set("Couleur du remplissage RGB", ofColor(81), ofColor(0, 0), ofColor(255, 255));
	color_picker_fill_hsb.set("Couleur du remplissage HSB", ofColor::fromHsb(h, s, br));

	color_picker_stroke.set("Couleur du trait RGB", ofColor(255), ofColor(0, 0), ofColor(255, 255));
	color_picker_stroke_hsb.set("Couleur du trait HSB", ofColor::fromHsb(h, s, br));

	slider_stroke_weight.set("Epaisseur des lignes", 2.0f, 0.0f, 10.0f);

	group_draw.setup("Modifier un dessin 2D");
	group_draw.add(color_picker_background);
	group_draw.add(color_picker_background_hsb);
	group_draw.add(color_picker_stroke);
	group_draw.add(color_picker_stroke_hsb);
	group_draw.add(color_picker_fill);
	group_draw.add(color_picker_fill_hsb);
	group_draw.add(slider_stroke_weight);
	gui.add(&group_draw);

	// Dessin 2D.
	groupe2.setup("Creer un dessin 2D");
	pixel.setName("Pixel");
	pixel_listener = pixel.newListener([this](bool&) {onChangePrimitiveVectorielle(pixel.getName(), pixel.get()); });
	point.setName("Point");
	point_listener = point.newListener([this](bool&) {onChangePrimitiveVectorielle(point.getName(), point.get()); });
	ligne.setName("Ligne");
	ligne_listener = ligne.newListener([this](bool&) {onChangePrimitiveVectorielle(ligne.getName(), ligne.get()); });
	rect.setName("Rectangle");
	rect_listener = rect.newListener([this](bool&) {onChangePrimitiveVectorielle(rect.getName(), rect.get()); });
	elipse.setName("Ellipse");
	elipse_listener = elipse.newListener([this](bool&) {onChangePrimitiveVectorielle(elipse.getName(), elipse.get()); });
	triangle.setName("Triangle");
	triangle_listener = triangle.newListener([this](bool&) {onChangePrimitiveVectorielle(triangle.getName(), triangle.get()); });
	groupe2.add(pixel);
	groupe2.add(point);
	groupe2.add(ligne);
	groupe2.add(rect);
	groupe2.add(elipse);
	groupe2.add(triangle);
	gui.add(&groupe2);

	checkbox1.setName("Remplissage");
	reset_button.setup("Vider le canvas");
	reset_button.addListener(this, &ofApp::reset_button_pressed);
	gui.add(checkbox1);
	gui.add(&reset_button);
	undo.setup("Annuler");
	redo.setup("Refaire");
	undo.addListener(this, &ofApp::undo_pressed);
	redo.addListener(this, &ofApp::redo_pressed);
	gui.add(&undo);
	gui.add(&redo);

	// Draggable Vertex.
	trans_interactive.setup("Transformations interactives");
	draggable_show.set("Afficher", false);
	trans_interactive.add(draggable_show);
	gui.add(&trans_interactive);

	// Module 4.
	geometrie.setup("Primitives geometriques");
	affiche_pyramide.set("Pyramide", false);
	affiche_sphere.set("Sphere de triangle", false);
	geometrie.add(affiche_pyramide);
	geometrie.add(affiche_sphere);
	gui.add(&geometrie);

	geometry_group.setup("Modeles 3D");
	geometry_group.add(model_one.set("Voiture", false));
	model_one_listener = model_one.newListener([this](bool&) {onChangeGeometryGroup(model_one.getName(), model_one.get()); });
	geometry_group.add(model_two.set("Loup", false));
	model_two_listener = model_two.newListener([this](bool&) {onChangeGeometryGroup(model_two.getName(), model_two.get()); });
	geometry_group.add(model_three.set("Cerf", false));
	model_three_listener = model_three.newListener([this](bool&) {onChangeGeometryGroup(model_three.getName(), model_three.get()); });

	gui.add(&geometry_group);
	model_reset.setup("Effacer modeles");
	model_reset.addListener(this, &ofApp::model_reset_pressed);
	gui.add(&model_reset);
	gui.add(model_box.set("Boite de delimitation", false));

	// Module 5.
	filter_group.setup("Filtrage");
	filter_group.add(aucun_filtre.set("Aucun", true));
	aucun_filtre_listener = aucun_filtre.newListener([this](bool&) {onChangeFiltrage(aucun_filtre.getName(), aucun_filtre.get()); });
	filter_group.add(grayscale.set("Niveau de gris", false));
	grayscale_listener = grayscale.newListener([this](bool&) {onChangeFiltrage(grayscale.getName(), grayscale.get()); });
	filter_group.add(seuil.set("Seuil", false));
	seuil_listener = seuil.newListener([this](bool&) {onChangeFiltrage(seuil.getName(), seuil.get()); });
	filter_group.add(lissage.set("Lissage", false));
	lissage_listener = lissage.newListener([this](bool&) {onChangeFiltrage(lissage.getName(), lissage.get()); });
	filter_group.add(seuilLissage.set("Seuil + lissage", false));
	seuilLissage_listener = seuilLissage.newListener([this](bool&) {onChangeFiltrage(seuilLissage.getName(), seuilLissage.get()); });
	gui.add(&filter_group);

	tonemapping.setup("Mappage tonal");
	tonemapping.addListener(this, &ofApp::tonemapping_pressed);
	gui.add(&tonemapping);

	tex_procedural.setup("Texture procedural");
	affiche_tex.set("Afficher", false);
	tex_procedural.add(affiche_tex);
	gui.add(&tex_procedural);

	gui.add(textboxX);
	gui.add(textboxY);

	// Initialisation variable.
	checkbox1 = true;
	rect = pixel = elipse = point = ligne = triangle = false;
	selected_ctrl_point = 0;

	exportCount = 1;
	nFrames = 0;
	renderer.setup();
	draggableVertex.setup();
	topologieParametrique.setup();
}

//--------------------------------------------------------------
void ofApp::update() {
	sync.update();
	nFrames++;

	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;

	if (topologieParametrique.afficher_courbe_parametrique) {
		if (is_key_press_up)
			topologieParametrique.selected_ctrl_point->y -= topologieParametrique.delta_y * time_elapsed;
		if (is_key_press_down)
			topologieParametrique.selected_ctrl_point->y += topologieParametrique.delta_y * time_elapsed;
		if (is_key_press_left)
			topologieParametrique.selected_ctrl_point->x -= topologieParametrique.delta_x * time_elapsed;
		if (is_key_press_right)
			topologieParametrique.selected_ctrl_point->x += topologieParametrique.delta_x * time_elapsed;
	}

	//RGB
	if (!check) {
		renderer.background_color = color_picker_background;
		renderer.stroke_fill = color_picker_fill;
		renderer.stroke_color = color_picker_stroke;
	}
	//HSB
	else {
		renderer.background_color = color_picker_background_hsb;
		renderer.stroke_fill = color_picker_fill_hsb;
		renderer.stroke_color = color_picker_stroke_hsb;
	}
	renderer.epaisseur = slider_stroke_weight;
	renderer.check = checkbox1;
	renderer.affiche_tex = affiche_tex;
	renderer.affiche_pyramide = affiche_pyramide;
	renderer.affiche_sphere = affiche_sphere;
	renderer.selection_multiple = check2;

	renderer.model_box = model_box;
	renderer.update();
	topologieParametrique.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	renderer.draw();
	if (draggable_show)
		draggableVertex.draw();
	gui.draw();

	topologieParametrique.draw();

	// Logique de capture d'ecran.
	if (nFrames < recFrames && nFrames % 3 == 0) {
		captureFrame();
	}
	if (nFrames == recFrames) {
		recFrames = 0;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// Sauvegarde image simple.
	if (key == 's') {
		ofImage monImage;
		monImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		monImage.save("selectColor.png", OF_IMAGE_QUALITY_BEST);
	};

	// Sauvegarde rafale d'image.
	if (key == 'x') {
		recFrames = nFrames + 120;  // 2 sec @ 60 FrameRate
	}

	switch (key)
	{
	case OF_KEY_LEFT: // touche ←
		is_key_press_left = true;
		break;

	case OF_KEY_UP: // touche ↑
		is_key_press_up = true;
		break;

	case OF_KEY_RIGHT: // touche →
		is_key_press_right = true;
		break;

	case OF_KEY_DOWN: // touche ↓
		is_key_press_down = true;
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key)
	{
	case OF_KEY_RIGHT_SHIFT:
		selected_ctrl_point += 1;
		if (selected_ctrl_point == 6)
			selected_ctrl_point = 0;
		topologieParametrique.selected_ctrl_point = &topologieParametrique.ctrl_points[selected_ctrl_point];
		break;

	case 109: // touche m
		topologieParametrique.afficher_surface_parametrique = !topologieParametrique.afficher_surface_parametrique;
		break;

	case 110: // touche n
		topologieParametrique.afficher_courbe_parametrique = !topologieParametrique.afficher_courbe_parametrique;
		break;

	case OF_KEY_LEFT: // touche ←
		is_key_press_left = false;
		break;

	case OF_KEY_UP: // touche ↑
		is_key_press_up = false;
		break;

	case OF_KEY_RIGHT: // touche →
		is_key_press_right = false;
		break;

	case OF_KEY_DOWN: // touche ↓
		is_key_press_down = false;
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	textboxX.set("X", to_string(x));
	textboxY.set("Y", to_string(y));

	if (affiche_tex)
		renderer.change_texture(x, y);
	draggableVertex.mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	textboxX.set("X", to_string(x));
	textboxY.set("Y", to_string(y));

	draggableVertex.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	renderer.is_mouse_button_pressed = true;

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	renderer.mouse_press_x = x;
	renderer.mouse_press_y = y;

	draggableVertex.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	renderer.is_mouse_button_pressed = false;

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;

	if (renderer.draw_mode != VectorPrimitiveType::none)
		renderer.add_vector_shape(renderer.draw_mode);
	if (model_one.get() || model_two.get() || model_three.get()) { renderer.add_3d_model(renderer.model_draw_mode); }

	draggableVertex.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	renderer.import.load(dragInfo.files.at(0));
}


int ofApp::getBrightness(float r, float g, float b) {
	return MAX(MAX(r, g), b);
}

int ofApp::getLightness(float r, float g, float b) {
	return (r + g + b) / 3.f;
}

int ofApp::getHue(float r, float g, float b) {
	float max = MAX(MAX(r, g), b);
	float min = MIN(MIN(r, g), b);
	float delta = max - min;
	if (r == max) return (0 + (g - b) / delta) * 42.5;  //yellow...magenta
	if (g == max) return (2 + (b - r) / delta) * 42.5;  //cyan...yellow
	if (b == max) return (4 + (r - g) / delta) * 42.5;  //magenta...cyan
	return 0;
}

int ofApp::getSaturation(float r, float g, float b) {
	float min = MIN(MIN(r, g), b);
	float max = MAX(MAX(r, g), b);
	float delta = max - min;
	if (max != 0) return int(delta / max * 255);
	return 0;
}

void ofApp::ofxSetColorHSB(float h, float s, float b, float a = 255) {
	if (b == 0) return ofSetColor(0, 0, 0, a); //black
	if (s == 0) return ofSetColor(b, b, b, a); //gray

	h *= 6.0 / 255.0;
	s *= 1.0 / 255.0;

	float x = b * (1 - s);
	float y = b * (1 - s * (h - int(h)));
	float z = b * (1 - s * (1 - (h - int(h))));

	switch (int(h)) {
	case 0: return ofSetColor(b, z, x, a); //red
	case 1: return ofSetColor(y, b, x, a); //green
	case 2: return ofSetColor(x, b, z, a);
	case 3: return ofSetColor(x, y, b, a); //blue
	case 4: return ofSetColor(z, x, b, a);
	case 5: return ofSetColor(b, x, y, a); //back to red
	}
}

void ofApp::captureFrame() {
	string filename = "export_" + to_string(exportCount) + ".png";
	exportImg.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
	exportImg.save(filename);
	exportCount++;
}

void ofApp::reset_button_pressed() {
	renderer.reset();
}

void ofApp::onChangePrimitiveVectorielle(string name, bool value) {
	if (name == "Pixel") {
		if (value) {
			rect = elipse = point = ligne = triangle = false;
			renderer.draw_mode = VectorPrimitiveType::pixel;
		}
	}
	else if (name == "Point") {
		if (value) {
			rect = pixel = elipse = ligne = triangle = false;
			renderer.draw_mode = VectorPrimitiveType::point;
		}
	}
	else if (name == "Ligne") {
		if (value) {
			rect = pixel = elipse = point = triangle = false;
			renderer.draw_mode = VectorPrimitiveType::line;
		}
	}
	else if (name == "Rectangle") {
		if (value) {
			pixel = elipse = point = ligne = triangle = false;
			renderer.draw_mode = VectorPrimitiveType::rectangle;
		}
	}
	else if (name == "Ellipse") {
		if (value) {
			rect = pixel = point = ligne = triangle = false;
			renderer.draw_mode = VectorPrimitiveType::ellipse;
		}
	}
	else if (name == "Triangle") {
		if (value) {
			rect = pixel = point = ligne = elipse = false;
			renderer.draw_mode = VectorPrimitiveType::triangle;
		}
	}

	if (!value)
		renderer.draw_mode = VectorPrimitiveType::none;
}

void ofApp::tonemapping_pressed()
{
	renderer.toneMap();
}

void ofApp::onChangeGeometryGroup(string name, bool value) {
	if (name == "Voiture") {
		if (value) {
			model_two.set(false);
			model_three.set(false);
			renderer.model_draw_mode = ModelToDraw::modelOne;
		}
	}
	else if (name == "Loup") {
		if (value) {
			model_one.set(false);
			model_three.set(false);
			renderer.model_draw_mode = ModelToDraw::modelTwo;
		}
	}
	else if (name == "Cerf") {
		if (value) {
			model_one.set(false);
			model_two.set(false);
			renderer.model_draw_mode = ModelToDraw::modelThree;
		}
	}
}

void ofApp::onChangeFiltrage(string name, bool value) {
	if (name == "Aucun") {
		if (value) {
			grayscale = seuil = lissage = seuilLissage = false;
			renderer.filtrage = Filtrage::none;
		}
	}
	else if (name == "Niveau de gris") {
		if (value) {
			aucun_filtre = seuil = lissage = seuilLissage = false;
			renderer.filtrage = Filtrage::grayscale;
		}
	}
	else if (name == "Seuil") {
		if (value) {
			aucun_filtre = grayscale = lissage = seuilLissage = false;
			renderer.filtrage = Filtrage::seuil;
		}
	}
	else if (name == "Lissage") {
		if (value) {
			aucun_filtre = grayscale = seuil = seuilLissage = false;
			renderer.filtrage = Filtrage::lissage;
		}
	}
	else if (name == "Seuil + lissage") {
		if (value) {
			aucun_filtre = grayscale = seuil = lissage = false;
			renderer.filtrage = Filtrage::seuilLissage;
		}
	}
}

void ofApp::model_reset_pressed()
{
	renderer.model_reset();
}

void ofApp::undo_pressed()
{
	renderer.undo();
}

void ofApp::redo_pressed()
{
	renderer.redo();
}