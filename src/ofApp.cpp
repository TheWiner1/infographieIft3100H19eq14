#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Partie 2: Équipe 14");
	ofEnableSmoothing();

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
	geometry_group.add(model_one_material.setup("Matériau voiture",2,0,4));
	model_one_listener = model_one.newListener([this](bool&) {onChangeGeometryGroup(model_one.getName(), model_one.get()); });

	geometry_group.add(model_two.set("Loup", false));
	model_two_listener = model_two.newListener([this](bool&) {onChangeGeometryGroup(model_two.getName(), model_two.get()); });
	geometry_group.add(model_two_material.setup("Matériau loup", 0, 0, 4));

	geometry_group.add(model_three.set("Cerf", false));
	model_three_listener = model_three.newListener([this](bool&) {onChangeGeometryGroup(model_three.getName(), model_three.get()); });
	geometry_group.add(model_three_material.setup("Matériau cerf", 4, 0, 4));

	gui.add(&geometry_group);
	gui.add(model_lookAt.set("Regarder vers", false));
	gui.add(model_mirror.set("Mirroir modeles", false));
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

	//triangulation de Delaunay
	topologie.setup("triangulation de Delaunay");
	delaunay_show.set("triangulation", false);
	topologie.add(delaunay_show);
	gui.add(&topologie);

	// Initialisation variable.
	checkbox1 = true;
	rect = pixel = elipse = point = ligne = triangle = false;

	exportCount = 1;
	nFrames = 0;
	renderer.setup();
	draggableVertex.setup();
	renderer.setupIllumination();
	//renderer.setupCamera();

	ofEnableDepthTest();

	camera_near = 50.0f;
	camera_far = 1550.0f;

	camera_fov = 60.0f;
	camera_fov_delta = 16.0f;

	camera_front.setPosition(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 300.0f);
	projection();

	is_key_press_up = false;
	is_key_press_down = false;
	is_key_press_left = false;
	is_key_press_right = false;

	selected_ctrl_point = 0;

	topologieParametrique.setup();

 toggle_ui = true;

  PBR.setup();

  reset();

  guiPBR.setup("pbr material");

  // interface pour les couleurs du matériau
  group_material_color.setup("color");

  group_material_color.add(color_picker_ambient);
  group_material_color.add(color_picker_diffuse);
  group_material_color.add(color_picker_specular);

  guiPBR.add(&group_material_color);

  // interface pour les facteurs numériques du matériau
  group_material_factor.setup("factor");

  group_material_factor.add(slider_metallic);
  group_material_factor.add(slider_roughness);
  group_material_factor.add(slider_occlusion);
  group_material_factor.add(slider_brightness);

  group_material_factor.add(slider_fresnel_ior);

  guiPBR.add(&group_material_factor);

  // interface pour les paramètres de la lumière
  group_light.setup("light");

  group_light.add(color_picker_light_color);
  group_light.add(slider_light_intensity);
  group_light.add(toggle_light_motion);

  guiPBR.add(&group_light);

  // interface pour les paramètres de mappage tonal
  group_tone_mapping.setup("tone mapping");

  group_tone_mapping.add(slider_exposure);
  group_tone_mapping.add(slider_gamma);
  group_tone_mapping.add(toggle_tone_mapping);

  guiPBR.add(&group_tone_mapping);

  // interface pour les options de l'application
  toggle_ui.setName("hide ui");
  guiPBR.add(toggle_ui);

  button_reset.setup("reset");
  button_reset.addListener(this, &ofApp::button_reset_pressed);
  guiPBR.add(&button_reset);

  geometry_groupPBR.setup("Modeles 3D");

  geometry_groupPBR.add(model_four.set("Voiture", false));

  model_four_listener = model_four.newListener([this](bool&) {onChangeGeometryGroupPBR(model_four.getName(), model_four.get()); });

  geometry_groupPBR.add(model_five.set("Loup", false));

  model_five_listener = model_five.newListener([this](bool&) {onChangeGeometryGroupPBR(model_five.getName(), model_five.get()); });

  geometry_groupPBR.add(model_six.set("Cerf", false));

  model_six_listener = model_six.newListener([this](bool&) {onChangeGeometryGroupPBR(model_six.getName(), model_six.get()); });



  guiPBR.add(&geometry_groupPBR);

  model_resetPBR.setup("Effacer modeles");

  model_resetPBR.addListener(this, &ofApp::model_reset_pressedPBR);

  guiPBR.add(&model_resetPBR);

  // AJOUT SASSY 6.3-------------------------------------------------
  // Setup cameras
  iMainCamera = 0; // indice camera


  cameras[0] = &camera_front;
  renderer.camera = &camera_front;

  camTop.tiltDeg(-90);
  camLeft.panDeg(-90);

  cameras[1] = &camTop;
  cameras[2] = &camLeft;

  for (size_t i = 1; i != N_CAMERAS; ++i) {
	  cameras[i]->setPosition(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, 20.0f);
	  cameras[i]->enableOrtho();
	  cameras[i]->setNearClip(0.1);
	  cameras[i]->setFarClip(10000);
	  //cameras[i]->setNearClip(camera_near);
	  //cameras[i]->setFarClip(camera_far);
  }

  // Define viewports
  setupViewports();

  //-----------------------------------------------------
}

void ofApp::reset()
{
  PBR.reset();

  color_picker_ambient.set("ambient", PBR.material_color_ambient, ofColor(0, 0), ofColor(255, 255));
  color_picker_diffuse.set("diffuse", PBR.material_color_diffuse, ofColor(0, 0), ofColor(255, 255));
  color_picker_specular.set("specular", PBR.material_color_specular, ofColor(0, 0), ofColor(255, 255));

  slider_metallic.set("metallic", PBR.material_metallic, 0.0f, 1.0f);
  slider_roughness.set("roughness", PBR.material_roughness, 0.0f, 1.0f);
  slider_occlusion.set("occlusion", PBR.material_occlusion, 0.0f, 5.0f);
  slider_brightness.set("brightness", PBR.material_brightness, 0.0f, 5.0f);

  slider_fresnel_ior.set("fresnel ior", PBR.material_fresnel_ior, glm::vec3(0.0f), glm::vec3(1.0f));

  color_picker_light_color.set("color", PBR.light_color, ofColor(0, 0), ofColor(255, 255));
  slider_light_intensity.set("intensity", PBR.light_intensity, 0.0f, 100.0f);
  toggle_light_motion.set("motion", PBR.light_motion);

  slider_exposure.set("exposure", 1.0f, 0.0f, 5.0f);
  slider_gamma.set("gamma", 2.2f, 0.0f, 5.0f);

  if (PBR.tone_mapping_toggle)
    toggle_tone_mapping.set("aces filmic", true);
  else
    toggle_tone_mapping.set("reinhard", false);
}

//--------------------------------------------------------------
void ofApp::update() {
	sync.update();
	nFrames++;

		time_current = ofGetElapsedTimef();
		time_elapsed = time_current - time_last;
		time_last = time_current;

		PBR.material_color_ambient = color_picker_ambient;
		PBR.material_color_diffuse = color_picker_diffuse;
		PBR.material_color_specular = color_picker_specular;

		PBR.material_metallic = slider_metallic;
		PBR.material_roughness = slider_roughness;
		PBR.material_occlusion = slider_occlusion;
		PBR.material_brightness = slider_brightness;

		PBR.material_fresnel_ior = slider_fresnel_ior;

		PBR.light_color = color_picker_light_color;
		PBR.light_intensity = slider_light_intensity;
		PBR.light_motion = toggle_light_motion;

		PBR.tone_mapping_exposure = slider_exposure;
		PBR.tone_mapping_gamma = slider_gamma;
		PBR.tone_mapping_toggle = toggle_tone_mapping;

		if (PBR.tone_mapping_toggle)
			toggle_tone_mapping.set("aces filmic", true);
		else
			toggle_tone_mapping.set("reinhard", false);

		PBR.update();

	//RGB
	if (!check) {
		renderer.background_color = color_picker_background;
		renderer.stroke_fill = color_picker_fill;
		renderer.stroke_color = color_picker_stroke;
	}
	//HSB
	else {
		renderer.background_color = color_picker_background_hsb;//couleur de la 3D
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
	renderer.updateCamera();
	update_materials(); // Alex
	time_current = ofGetElapsedTimef();
	time_elapsed = time_current - time_last;
	time_last = time_current;

	if (is_camera_perspective)
	{
		if (is_camera_fov_narrow)
		{
			camera_fov = std::max(camera_fov -= camera_fov_delta * time_elapsed, 0.0f);
			//camera->setFov(camera_fov);
			cameras[iMainCamera]->setFov(camera_fov);
			for (size_t i = 0; i != N_CAMERAS; ++i) {
				cameras[i]->setFov(camera_fov);
			}
		}

		if (is_camera_fov_wide)
		{
			camera_fov = std::min(camera_fov += camera_fov_delta * time_elapsed, 180.0f);
			//camera->setFov(camera_fov);
			cameras[iMainCamera]->setFov(camera_fov);
			for (size_t i = 0; i != N_CAMERAS; ++i) {
				cameras[i]->setFov(camera_fov);
			}
		}
	}
	renderer.updateIllumination();	
		/*Alex TODO if avec camera*/
	renderer.is_camera_move_forward = is_key_press_plus;
	renderer.is_camera_move_backward = is_key_press_minus;
	if (!topologieParametrique.afficher_courbe_parametrique) //Alex
	{
		renderer.is_camera_move_left = is_key_press_left;
		renderer.is_camera_move_right = is_key_press_right;

		renderer.is_camera_move_up = is_key_press_up;
		renderer.is_camera_move_down = is_key_press_down;
	}
	renderer.is_camera_roll_left = is_key_press_eight;
	renderer.is_camera_roll_right = is_key_press_nine;


	renderer.is_camera_pan_left = is_key_press_div;
	renderer.is_camera_pan_right = is_key_press_mul;

	renderer.is_camera_look_at = model_lookAt;
	renderer.model_mirror = model_mirror;

	if (topologieParametrique.afficher_courbe_parametrique) {
		if (is_key_press_up)
			topologieParametrique.selected_ctrl_point->y -= topologieParametrique.speed * time_elapsed;
		if (is_key_press_down)
			topologieParametrique.selected_ctrl_point->y += topologieParametrique.speed * time_elapsed;
		if (is_key_press_left)
			topologieParametrique.selected_ctrl_point->x -= topologieParametrique.speed * time_elapsed;
		if (is_key_press_right)
			topologieParametrique.selected_ctrl_point->x += topologieParametrique.speed * time_elapsed;
	}
	topologieParametrique.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	//MODIFICATION DE TOUTE LA METHODE SASSY 6.3
    // Highlight background of selected camera

	ofPushStyle();
	ofDisableDepthTest();
	ofSetColor(0, 250, 100);
	ofDrawRectangle(viewGrid[iMainCamera]);
	ofEnableDepthTest();

	ofSetColor(ofColor::white);
	//----------------------------------------

	cameras[iMainCamera]->begin(viewMain);
	cameras[iMainCamera]->setScale(1, -1, 1);
	//camera->begin();
	//camera->setScale(1, -1, 1);

	drawScene(iMainCamera);
	//camera->end();
	cameras[iMainCamera]->end();

	// draw side viewports--------------------------
	for (int i = 0; i < N_CAMERAS; i++) {
		cameras[i]->setScale(3, -3, 3);
		cameras[i]->begin(viewGrid[i]);
		drawScene(i);
		cameras[i]->end();
	}

	ofPopStyle();
	//---------------------------------------------

	// Draw annotations (text, gui, etc)

	ofPushStyle();
	ofDisableDepthTest();

	// draw some labels
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Press keys 1-3 pour choisir une camera", viewMain.x + 20, 30);
	ofDrawBitmapString("Camera choisie: " + ofToString(iMainCamera + 1), viewMain.x + 20, 50);
	ofDrawBitmapString("Press 'f' pour plein ecran", viewMain.x + 20, 70);

	ofDrawBitmapString("Principale (*Selectionner 'p'", viewGrid[0].x + 20, viewGrid[0].y + 30);
	ofDrawBitmapString("perspective)(*Selectionner", viewGrid[0].x + 20, viewGrid[0].y + 45);
	ofDrawBitmapString(" 'o'orthogonale)", viewGrid[0].x + 20, viewGrid[0].y + 60);
	ofDrawBitmapString("Haut", viewGrid[1].x + 20, viewGrid[1].y + 30);
	ofDrawBitmapString("Gauche", viewGrid[2].x + 20, viewGrid[2].y + 30);

	// draw outlines on views
	ofSetLineWidth(5);
	ofNoFill();
	ofSetColor(255, 255, 255);
	//
	for (int i = 0; i < N_CAMERAS; i++) {
		ofDrawRectangle(viewGrid[i]);
	}

	ofDrawRectangle(viewMain);

	// restore the GL depth function

	ofPopStyle();
	//---------------------------------------------------------


	topologieParametrique.draw();


	if (!toggle_ui) {
		gui.draw();
	    gui.setPosition(ofGetWidth() - 250, 0); 
    }   
	else {
        guiPBR.draw();
        guiPBR.setPosition(ofGetWidth() - 250, 0); 
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

	if (key == 'r') {
		triangulation.reset();
	}


	//AJOUT SASSY 6.3------------------------------------------------
	if (key >= '1' && key <= '3') {
		iMainCamera = key - '1';
	}

	if (key == 'f') {
		ofToggleFullscreen();
	}

	//-------------------------------------------------------

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

	case '+': // touche +
		is_key_press_plus = true;
		break;

	case '-': // touche -
		is_key_press_minus = true;
		break;

	case '/': // touche /
		is_key_press_div = true;
		break;

	case '*': // touche -
		is_key_press_mul = true;
		break;

	case '7': // touche 7
		is_key_press_seven = true;
		break;

	case '8': // touche 8
		is_key_press_eight = true;
		break;

	case '9': // touche 9
		is_key_press_nine = true;
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key)
	{
		case 111: // touche o
			is_camera_perspective = false;
			projection();
			ofLog() << "<orthographic projection>";
			break;

		case 112: // touche p
			is_camera_perspective = true;
			projection();
			ofLog() << "<perpective projection>";
			break;

		case 49: // touche 1
			renderer.shader_active = ShaderType::color_fill;
			ofLog() << "<shader: color fill>";
			break;

		case 50: // touche 2
			renderer.shader_active = ShaderType::lambert;
			ofLog() << "<shader: lambert>";
			break;

		case 51: // touche 3
			renderer.shader_active = ShaderType::gouraud;
			ofLog() << "<shader: gouraud>";
			break;

		case 52: // touche 4
			renderer.shader_active = ShaderType::phong;
			ofLog() << "<shader: phong>";
			break;

		case 53: // touche 5
			renderer.shader_active = ShaderType::blinn_phong;
			ofLog() << "<shader: blinn-phong>";
			break;

		case OF_KEY_RIGHT_SHIFT:
			selected_ctrl_point += 1;
			if (selected_ctrl_point == 6)
				selected_ctrl_point = 0;
			topologieParametrique.selected_ctrl_point = &topologieParametrique.ctrl_points[selected_ctrl_point];
			break;

		case 98: // touche b : reinitialise la position des points de la courbe parametrique. 
			topologieParametrique.reset();
			break;

		case 109: // touche m 
			topologieParametrique.afficher_surface_parametrique = !topologieParametrique.afficher_surface_parametrique;
			break;

		case 110: // touche n 
			topologieParametrique.afficher_courbe_parametrique = !topologieParametrique.afficher_courbe_parametrique;
			break;
/*Alex*/
		case 56: // touche 8
			is_key_press_eight = false;
			break;

		case 57: // touche 9
			is_key_press_nine = false;
			break;

		case '/': // touche /
			is_key_press_div = false;
			break;

		case '*': // touche -
			is_key_press_mul = false;
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

		case '+': // touche +
			is_key_press_plus = false;
			break;

		case '-': // touche -
			is_key_press_minus = false;
			break;
/*Alex*/

                    case 113: // touche e
      toggle_light_motion = !toggle_light_motion;
      ofLog() << "<toggle light motion: " << toggle_light_motion << ">";
      break;

    case 114: // touche r
      reset();
      ofLog() << "<reset PBR>";
      break;

    case 115: // touche s
      is_key_press_down = false;
      break;

    case 117: // touche u
      toggle_ui = !toggle_ui;
      ofLog() << "<toggle ui: " << toggle_ui << ">";
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

	PBR.mouse_current_x = x;
	PBR.mouse_current_y = y;

	renderer.mouse_press_x = x;
	renderer.mouse_press_y = y;


	draggableVertex.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	renderer.is_mouse_button_pressed = false;

	renderer.mouse_current_x = x;
	renderer.mouse_current_y = y;


	if (model_four.get() || model_five.get() || model_six.get()) { PBR.add_3d_model(PBR.model_draw_mode); }
	if (renderer.draw_mode != VectorPrimitiveType::none)
		renderer.add_vector_shape(renderer.draw_mode);
	if (model_one.get() || model_two.get() || model_three.get()) { renderer.add_3d_model(renderer.model_draw_mode); }
	

	draggableVertex.mouseReleased(x, y, button);

	//pour la gestion de la triangularisation
	triangulation.addPoint(ofPoint(x, y));
	triangulation.triangulate();
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
	setupViewports(); // AJOUT SASSY 6.3
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

void ofApp::reset_button_pressedPBR() {
	PBR.reset();
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
	renderer.selectedModel = 0;
	if (name == "Voiture") {
		if (value) {
			model_two.set(false);
			model_three.set(false);
			renderer.model_draw_mode = ModelToDraw::modelOne;
			renderer.selectedModel = 1;
		}
	}
	else if (name == "Loup") {
		if (value) {
			model_one.set(false);
			model_three.set(false);
			renderer.model_draw_mode = ModelToDraw::modelTwo;
			renderer.selectedModel = 2;
		}
	}
	else if (name == "Cerf") {
		if (value) {
			model_one.set(false);
			model_two.set(false);
			renderer.model_draw_mode = ModelToDraw::modelThree;
			renderer.selectedModel = 3;

		}
	}
}

void ofApp::onChangeGeometryGroupPBR(string name, bool value) {

	if (name == "Voiture") {
		if (value) {
			model_four.set(true);
			model_five.set(false);
			model_six.set(false);
			PBR.model_draw_mode = ModelToDrawPBR::modelOne;
			
		}
	}
	else if (name == "Loup") {
		if (value) {
			model_five.set(true);
			model_four.set(false);
			model_six.set(false);
			PBR.model_draw_mode = ModelToDrawPBR::modelTwo;
			
		}
	}
	else if (name == "Cerf") {
		if (value) {
			model_six.set(true);
			model_four.set(false);
			model_five.set(false);
			PBR.model_draw_mode = ModelToDrawPBR::modelThree;
	

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

void ofApp::model_reset_pressedPBR()
{
	PBR.model_reset();
	PBR.reset();
}

void ofApp::undo_pressed()
{
	renderer.undo();
}

void ofApp::projection() {
	camera = &camera_front;
	renderer.camera = &camera_front;

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
}

void ofApp::redo_pressed()
{
	renderer.redo();
}void ofApp::setupViewports() {

	//AJOUT METHODE SASSY 6.3
	//call here whenever we resize the window


	//--
	// Define viewports

	float xOffset = ofGetWidth() / 4;
	float yOffset = ofGetHeight() / N_CAMERAS;

	viewMain.x = xOffset;
	viewMain.y = 0;
	viewMain.width = xOffset * 2;
	viewMain.height = ofGetHeight();

	for (int i = 0; i < N_CAMERAS; i++) {

		viewGrid[i].x = 0;
		viewGrid[i].y = yOffset * i;
		viewGrid[i].width = xOffset;
		viewGrid[i].height = yOffset;
	}

}

//--------------------------------------------------------------
void ofApp::drawScene(int cameraIndex) {

	renderer.draw();
         PBR.draw();
	if (draggable_show)
		draggableVertex.draw();
	if (delaunay_show) {
		triangulation.draw();
		ofDrawBitmapString("'r' to reset", ofPoint(10, 20));
	}

	// Logique de capture d'ecran.
	if (nFrames < recFrames && nFrames % 3 == 0) {
		captureFrame();
	}
	if (nFrames == recFrames) {
		recFrames = 0;
	}

}

void ofApp::update_materials()
{
	switch (model_one_material)
	{
	case 1:
		renderer.model_material[0] = renderer.material_1;
		break;
	case 2:
		renderer.model_material[0] = renderer.material_2;
		break;
	case 3:
		renderer.model_material[0] = renderer.material_3;
		break;
	case 4:
		renderer.model_material[0] = renderer.material_4;
		break;
	default:
		renderer.model_material[0] = renderer.material_0;
		break;
	}

	switch (model_two_material)
	{
	case 1:
		renderer.model_material[1] = renderer.material_1;
		break;
	case 2:
		renderer.model_material[1] = renderer.material_2;
		break;
	case 3:
		renderer.model_material[1] = renderer.material_3;
		break;
	case 4:
		renderer.model_material[1] = renderer.material_4;
		break;
	default:
		renderer.model_material[1] = renderer.material_0;
		break;
	}
	switch (model_three_material)
	{
	case 1:
		renderer.model_material[2] = renderer.material_1;
		break;
	case 2:
		renderer.model_material[2] = renderer.material_2;
		break;
	case 3:
		renderer.model_material[2] = renderer.material_3;
		break;
	case 4:
		renderer.model_material[2] = renderer.material_4;
		break;
	default:
		renderer.model_material[2] = renderer.material_0;
		break;
	}
}
void ofApp::button_reset_pressed()
{
  reset();
}

void ofApp::exit()
{
  button_reset.removeListener(this, &ofApp::button_reset_pressed);

  ofLog() << "<app::exit>";
}
