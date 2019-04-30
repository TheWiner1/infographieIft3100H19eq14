// IFT3100H19_ImageExport/rendererPlus.cpp
// Classe responsable du rendu de l'application.

#include "rendererPlus.h"


// IFT3100H19_DrawVectorPrimitive/rendererPlus.cpp
// Classe responsable du rendu de l'application.

void RendererPlus::setup()
{

  // mode d'échantillonage des coordonnées de texture
  ofDisableArbTex();

  // chargement du code source des shaders
  shader_passthrough.load(
    "/home/win/Bureau/of_v0.10.1_linux64gcc6_release/apps/infographieIft3100H19eq14/bin/data/image_passthrough_330_vs.glsl",
    "/home/win/Bureau/of_v0.10.1_linux64gcc6_release/apps/infographieIft3100H19eq14/bin/data/image_passthrough_330_fs.glsl");

  shader_invert.load(
    "/home/win/Bureau/of_v0.10.1_linux64gcc6_release/apps/infographieIft3100H19eq14/bin/data/image_invert_330_vs.glsl",
    "/home/win/Bureau/of_v0.10.1_linux64gcc6_release/apps/infographieIft3100H19eq14/bin/data/image_invert_330_fs.glsl");

  // shader actif au lancement de l'application
  shader = shader_passthrough;

  image.load("/home/win/Bureau/of_v0.10.1_linux64gcc6_release/apps/infographieIft3100H19eq14/bin/data/teapot.jpg");

 // ofSetWindowShape(image.getWidth(), image.getHeight());
  //ofSetWindowShape(1300,700);
  filter = false;

//    set_primitive_3d();

    mode = 0;

    modeP =0;

    count3d = 50;

    head3d = 0;
    // calculer la taille de la structure générique commune à toutes les primitives vectorielles
    stride3d = sizeof(VectorPrimitive3d);

    // calculer le nombre d'octets à allouer en mémoire pour contenir le tableau de primitives vectorielles
    size3d = count3d * stride3d;

    // allocation d'un espace mémoire suffisamment grand pour contenir les données de l'ensemble des primitives vectorielles
    shapes3d = (VectorPrimitive3d*) std::malloc(size3d);

    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));

    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));

    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );

    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));

    ofSetSphereResolution(24);

}

void RendererPlus::draw()
{

  // sélectionner le shader
  shader.begin();

  // passer la texture au shader sous forme d'un attribut uniforme
  shader.setUniformTexture("image", image.getTexture(), 1);

  // afficher l'image sur toute la surface de la fenêtre d'affichage
  image.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

  // désélectionner le shader
  shader.end();

  // épaisseur du trait
  ofSetLineWidth(2);


  // couleur d'arrière-plan
  ofClear(background_color);
/*
  // dessiner le texte au centre de la fenêtre d'affichage
  font.drawString(
    text,
    (ofGetWidth() / 2.0f) - (bounding_box.getWidth() / 2.0f),
    (ofGetHeight() / 2.0f) + (bounding_box.getHeight() / 2.0f));

  // dessiner une ligne sous le texte
  ofDrawLine(
    (ofGetWidth() / 2.0f) - (bounding_box.getWidth() / 2.0f),
    (ofGetHeight() / 2.0f) + (bounding_box.getHeight() / 2.0f) + line_offset,
    (ofGetWidth() / 2.0f) + (bounding_box.getWidth() / 2.0f),
    (ofGetHeight() / 2.0f) + (bounding_box.getHeight() / 2.0f) + line_offset);
*/


  // afficher la zone de sélection
  /*if (is_mouse_button_pressed)
  {
    draw_zone(
      mouse_press_x,
      mouse_press_y,
      mouse_current_x,
      mouse_current_y);
  }

  draw_cursor(mouse_current_x, mouse_current_y);

  // afficher la zone de sélection
  if (is_mouse_button_pressed)
  {
    // dessiner la zone de sélection
    draw_zone(
      mouse_press_x,
      mouse_press_y,
      mouse_current_x,
      mouse_current_y);
  }

  // dessiner le curseur
  //draw_cursor(mouse_current_x, mouse_current_y);
*/
//----------------------------------------------------------------

	cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
	cam.begin();

	ofEnableDepthTest();

	ofEnableLighting();
	pointLight.enable();
	pointLight2.enable();
	pointLight3.enable();

	// draw the outer sphere
	material.begin();
	ofNoFill();
	ofDrawSphere(0, 0, max(ofGetWidth(),ofGetHeight()));
	material.end();

	if (mode == 1 || mode == 3) texture.getTexture().bind();
	if (mode == 2) vidGrabber.getTexture().bind();
//set_primitive_3d();
//	float screenWidth = ofGetWidth();
//	float screenHeight = ofGetHeight();

	set_primitive_3d_plane(mouse_press_x, mouse_current_x, mouse_press_y, mouse_current_y);

	set_primitive_3d_box(mouse_press_x, mouse_current_x, mouse_press_y, mouse_current_y);

	set_primitive_3d_sphere(mouse_press_x, mouse_current_x, mouse_press_y, mouse_current_y);

	set_primitive_3d_icosphere(mouse_press_x, mouse_current_x, mouse_press_y, mouse_current_y);

	set_primitive_3d_cylinder(mouse_press_x, mouse_current_x, mouse_press_y, mouse_current_y);

	set_primitive_3d_cone(mouse_press_x, mouse_current_x, mouse_press_y, mouse_current_y);
	
//-------
//-------
        for (index3d = 0; index3d < count3d; ++index3d)
        {
          switch (shapes3d[index3d].type)
          {
            case VectorPrimitive3dType::plane:

              set_primitive_3d_plane(shapes3d[index3d].position1[0], shapes3d[index3d].position1[1],
                  shapes3d[index3d].position2[0], shapes3d[index3d].position2[1]);

              plane.set(shapes3d[index3d].width,shapes3d[index3d].height);
              break;
/*
              ofFill();
              ofSetLineWidth(0);
              ofSetColor(
                shapes3d[index3d].stroke_color[0],
                shapes3d[index3d].stroke_color[1],
                shapes3d[index3d].stroke_color[2]);
              draw_pixel(
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              break;
*/
            case VectorPrimitive3dType::box:

              set_primitive_3d_box(shapes3d[index3d].position1[0], shapes3d[index3d].position1[1],
                  shapes3d[index3d].position2[0], shapes3d[index3d].position2[1]);

              break;
/*
              ofFill();
              ofSetLineWidth(0);
              ofSetColor(
                shapes3d[index3d].stroke_color[0],
                shapes3d[index3d].stroke_color[1],
                shapes3d[index3d].stroke_color[2]);
              draw_point(
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1],
                shapes3d[index3d].stroke_width);
              break;

            case VectorPrimitive3dType::sphere:

              ofNoFill();
              ofSetLineWidth(shapes3d[index3d].stroke_width);
              ofSetColor(
                shapes3d[index3d].stroke_color[0],
                shapes3d[index3d].stroke_color[1],
                shapes3d[index3d].stroke_color[2]);
              draw_line(
                shapes3d[index3d].position1[0],
                shapes3d[index3d].position1[1],
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              break;

*/            case VectorPrimitive3dType::sphere:

              set_primitive_3d_sphere(shapes3d[index3d].position1[0], shapes3d[index3d].position1[1],
                  shapes3d[index3d].position2[0], shapes3d[index3d].position2[1]);
              break;
              case VectorPrimitive3dType::icoSphere:

                set_primitive_3d_icosphere(shapes3d[index3d].position1[0], shapes3d[index3d].position1[1],
                    shapes3d[index3d].position2[0], shapes3d[index3d].position2[1]);
              break;
/*
              ofFill();
              ofSetLineWidth(0);
              ofSetColor(
                shapes3d[index3d].fill_color[0],
                shapes3d[index3d].fill_color[1],
                shapes3d[index3d].fill_color[2]);
              draw_rectangle(
                shapes3d[index3d].position1[0],
                shapes3d[index3d].position1[1],
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              ofNoFill();
              ofSetLineWidth(shapes3d[index3d].stroke_width);
              ofSetColor(
                shapes3d[index3d].stroke_color[0],
                shapes3d[index3d].stroke_color[1],
                shapes3d[index3d].stroke_color[2]);
              draw_rectangle(
                shapes3d[index3d].position1[0],
                shapes3d[index3d].position1[1],
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              break;
*/
            case VectorPrimitive3dType::cylinder:
              set_primitive_3d_cylinder(shapes3d[index3d].position1[0], shapes3d[index3d].position1[1],
                  shapes3d[index3d].position2[0], shapes3d[index3d].position2[1]);
              break;
/*
              ofFill();
              ofSetLineWidth(0);
              ofSetCircleResolution(48);
              ofSetColor(
                shapes3d[index3d].fill_color[0],
                shapes3d[index3d].fill_color[1],
                shapes3d[index3d].fill_color[2]);
              draw_ellipse(
                shapes3d[index3d].position1[0],
                shapes3d[index3d].position1[1],
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              ofNoFill();
              ofSetLineWidth(shapes3d[index3d].stroke_width);
              ofSetColor(
                shapes3d[index3d].stroke_color[0],
                shapes3d[index3d].stroke_color[1],
                shapes3d[index3d].stroke_color[2]);
              draw_ellipse(
                shapes3d[index3d].position1[0],
                shapes3d[index3d].position1[1],
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              break;
*/            case VectorPrimitive3dType::cone:
                set_primitive_3d_cone(shapes3d[index3d].position1[0], shapes3d[index3d].position1[1],
                    shapes3d[index3d].position2[0], shapes3d[index3d].position2[1]);
              break;
/*
              ofFill();
              ofSetLineWidth(0);
              ofSetCircleResolution(48);
              ofSetColor(
                shapes3d[index3d].fill_color[0],
                shapes3d[index3d].fill_color[1],
                shapes3d[index3d].fill_color[2]);
              draw_ellipse(
                shapes3d[index3d].position1[0],
                shapes3d[index3d].position1[1],
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              ofNoFill();
              ofSetLineWidth(shapes3d[index3d].stroke_width);
              ofSetColor(
                shapes3d[index3d].stroke_color[0],
                shapes3d[index3d].stroke_color[1],
                shapes3d[index3d].stroke_color[2]);
              draw_ellipse(
                shapes3d[index3d].position1[0],
                shapes3d[index3d].position1[1],
                shapes3d[index3d].position2[0],
                shapes3d[index3d].position2[1]);
              break;*/

            default:
              break;
          }
        }
//-------
//-------

	if (!bFill && bWireframe) {
		material.end();
	}

	if (mode == 1 || mode == 3) texture.getTexture().unbind();
	if (mode == 2) vidGrabber.getTexture().unbind();

	material.end();
	ofDisableLighting();

	if (bDrawLights) {
		ofFill();
		ofSetColor(pointLight.getDiffuseColor());
		pointLight.draw();
		ofSetColor(pointLight2.getDiffuseColor());
		pointLight2.draw();
		ofSetColor(pointLight3.getDiffuseColor());
		pointLight3.draw();
	}

	if (bDrawNormals) {
		ofSetColor(225, 0, 255);
		plane.drawNormals(20, bSplitFaces);
		box.drawNormals(20, bSplitFaces);
		sphere.drawNormals(20, bSplitFaces);
		icoSphere.drawNormals(20, bSplitFaces);
		cylinder.drawNormals(20, bSplitFaces);
		cone.drawNormals(20, bSplitFaces);
	}
	if (bDrawAxes) {
		plane.drawAxes(plane.getWidth()*.5 + 30);
		box.drawAxes(box.getWidth() + 30);
		sphere.drawAxes(sphere.getRadius() + 30);
		icoSphere.drawAxes(icoSphere.getRadius() + 30);
		cylinder.drawAxes(cylinder.getHeight() + 30);
		cone.drawAxes(cone.getHeight() + 30);
	}

	ofDisableDepthTest();

	ofFill();

	cam.end();

	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);

	ofDrawBitmapStringHighlight("ofPlanePrimitive"    , glm::vec3(-90,-160,0) + cam.worldToScreen(plane.getGlobalPosition()) );
	ofDrawBitmapStringHighlight("ofBoxPrimitive"      , glm::vec3(-90,-160,0) + cam.worldToScreen( box.getPosition()));
	ofDrawBitmapStringHighlight("ofSpherePrimitive"   , glm::vec3(-90,-160,0) + cam.worldToScreen( sphere.getPosition()));
	ofDrawBitmapStringHighlight("ofIcoSpherePrimitive", glm::vec3(-90,+160,0) + cam.worldToScreen( icoSphere.getPosition()));
	ofDrawBitmapStringHighlight("ofCylinderPrimitive" , glm::vec3(-90,+160,0) + cam.worldToScreen( cylinder.getPosition()));
	ofDrawBitmapStringHighlight("ofConePrimitive"     , glm::vec3(-90,+160,0) + cam.worldToScreen( cone.getPosition()));

    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),0) << endl << endl;
        ss << "(f): Toggle Fullscreen"<<endl<<"(s): Draw Solid Shapes"<<endl<<"(w): Draw Wireframes"<<endl;
        ss <<"(1/2/3/4): Set Resolutions" <<endl<<"(n): Draw Normals"<<"\n(LEFT/RIGHT): Set Mode "<<ofToString(mode,0)<<endl;
        ss <<"(z): Split Faces " <<bSplitFaces<<endl;
        ss <<"(a): Draw Axes"<<endl<<"(l): Render lights" << endl <<"(h): Toggle help."<<endl;
        ofDrawBitmapStringHighlight(ss.str().c_str(), 20, 20);
    }

}


void RendererPlus::update()
{
  // paramètres des outils de dessin
  ofSetColor(stroke_color);
  ofSetLineWidth(stroke_weight);

  // résolution du rectangle qui délimite la zone de texte
  bounding_box = font.getStringBoundingBox(text, 0, 0);
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);

    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
    );

        //ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
    if(mode == 2 || ofGetElapsedTimef() < 10) {
        vidGrabber.update();
    }
}



// fonction qui vide le tableau de primitives vectorielles
void RendererPlus::reset()
{
  for (index3d = 0; index3d < count3d; ++index3d)
    shapes3d[index3d].type = VectorPrimitive3dType::none;

  head3d = 0;

  ofLog() << "<reset>";
}

// fonction qui détermine une couleur aléatoire pour les lignes de contour
/*void RendererPlus::random_color_stroke()
{

  stroke_color_r = (int) ofRandom(0, 256);
  stroke_color_g = (int) ofRandom(0, 256);
  stroke_color_b = (int) ofRandom(0, 256);
  stroke_color_a = 255;


  ofLog() << "<random color stroke>";
}*/

RendererPlus::~RendererPlus()
{
  std::free(shapes3d);
}
// fonction qui exporte une image à partir de son nom et de son extension, à partir du répertoire ./bin/data ou d'un chemin absolue
void RendererPlus::image_export(const string name, const string extension) const
{
  ofImage image;

  // extraire des données temporelles formatées
  string time_stamp = ofGetTimestampString("-%y%m%d-%H%M%S-%i");

  // générer un nom de fichier unique et ordonné
  string file_name = name + time_stamp + "." + extension;

  // capturer le contenu du framebuffer actif
  image.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

  // sauvegarder le fichier image
  image.save(file_name);

  ofLog() << "<export image: " << file_name << ">";
}

//--------------------------------------------------------------
void RendererPlus::set_primitive_3d_plane(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2){//int modeP);


  plane.set(abs(mouse_press_x -mouse_press_x2),
         abs(mouse_press_y2-mouse_press_y) );
/*
  if (nBpress%2==0){
    const int a =last_mouse_released_x -last_mouse_released_x;//mouse_current_x;
    const int b = mouse_current_y-last_mouse_released_x;//mouse_press_y;
    plane.set(a,b);}*/

  plane.setPosition(mouse_press_x - screenWidth* 0.5,
                    -mouse_press_y + screenHeight* 0.5, 0);

    if (mode == 3) {
            deformPlane = plane.getMesh();
            // x = columns, y = rows //
            ofVec3f planeDims = plane.getResolution();
            float planeAngleX = ofGetElapsedTimef()*3.6;
            float planeAngleInc = 3.f / (float)planeDims.x;
            ofVec3f vert;
            for (size_t i = 0; i < deformPlane.getNumIndices(); i++) {
                    planeAngleX += planeAngleInc;
                    int ii = deformPlane.getIndex(i);
                    vert = deformPlane.getVertex(ii);
                    vert.z += cos(planeAngleX) * 50;
                    deformPlane.setVertex(ii, vert);
            }
    }

    if (!bFill && bWireframe) {
            // if we are only drawing the wireframe, use
            // the material to draw it, otherwise the material
            // will be bound and unbound for every geometry
            // and the wireframe will be drawn in black
            material.begin();
    }

    if (bFill) {
            material.begin();
            ofFill();
            if (mode == 3) {
                    plane.transformGL();
                    deformPlane.draw();
                    plane.restoreTransformGL();
            }
            else {
                    plane.draw();
            }
            material.end();
    }

    if (bWireframe) {
            //lumiere
            ofNoFill();
            //couleur des lignes
            ofSetColor(0, 0, 0);
            //plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z + 1);

            //maillage triangulaire
            plane.drawWireframe();
            //plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z - 1);

    }
    // Box //

}

void RendererPlus::set_primitive_3d_box(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2){//int modeP);

  //box.set(abs(mouse_press_x-mouse_current_y));
  box.set(abs(mouse_press_x-mouse_press_x2));
  box.setPosition(mouse_press_x - screenWidth* 0.5,
                    -mouse_press_y + screenHeight* 0.5, 0);
    if (bFill) {
            material.begin();
            ofFill();
            if (mode == 3) {
                    box.transformGL();
                    for (int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++) {
                            ofPushMatrix();
                            ofTranslate(boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50);
                            boxSides[i].draw();
                            ofPopMatrix();
                    }
                    box.restoreTransformGL();
            }
            else {
                    box.draw();
            }
            material.end();
    }

    if (bWireframe) {
            ofNoFill();
            ofSetColor(0, 0, 0);
            box.setScale(1.01f);
            box.drawWireframe();
            box.setScale(1.f);
    }
}

void RendererPlus::set_primitive_3d_sphere(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2){//int modeP);

  //sphere.setRadius(abs(mouse_press_x-mouse_current_y)/2);
  sphere.setRadius(abs(mouse_press_x-mouse_press_x2)/2);
  sphere.setPosition(mouse_press_x - screenWidth* 0.5,
                     -mouse_press_y + screenHeight* 0.5, 0);


  if (mode == 3) {
            sphere.setMode(OF_PRIMITIVE_TRIANGLES);
            triangles = sphere.getMesh().getUniqueFaces();
    }

    if (bFill) {
            material.begin();
            ofFill();
            if (mode == 3) {
                    float angle = ofGetElapsedTimef()*3.2;
                    float strength = (sin(angle + .25)) * .5f * 5.f;
                    ofVec3f faceNormal;
                    for (size_t i = 0; i < triangles.size(); i++) {
                            // store the face normal here.
                            // we change the vertices, which makes the face normal change
                            // every time that we call getFaceNormal //
                            faceNormal = triangles[i].getFaceNormal();
                            for (int j = 0; j < 3; j++) {
                                    triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * strength);
                            }
                    }
                    sphere.getMesh().setFromTriangles(triangles);
            }
            sphere.draw();
            material.end();
    }

    if (bWireframe) {
            ofNoFill();
            ofSetColor(0, 0, 0);
            sphere.setScale(1.01f);
            sphere.drawWireframe();
            sphere.setScale(1.f);
    }
    // ICO Sphere //
}

void RendererPlus::set_primitive_3d_icosphere(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2){//int modeP);
  //icoSphere.setRadius(abs(mouse_press_x-mouse_current_y)/2);
  icoSphere.setRadius(abs(mouse_press_x-mouse_press_x2)/2);
  icoSphere.setPosition(mouse_press_x - screenWidth* 0.5,
                        -mouse_press_y + screenHeight* 0.5, 0);
    if (mode == 3) {
            triangles = icoSphere.getMesh().getUniqueFaces();
    }

    if (bFill) {
            material.begin();
            ofFill();

            if (mode == 3) {
                    float angle = (ofGetElapsedTimef() * 1.4);
                    ofVec3f faceNormal;
                    for (size_t i = 0; i < triangles.size(); i++) {
                            float frc = ofSignedNoise(angle* (float)i * .1, angle*.05) * 4;
                            faceNormal = triangles[i].getFaceNormal();
                            for (int j = 0; j < 3; j++) {
                                    triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc);
                            }
                    }
                    icoSphere.getMesh().setFromTriangles(triangles);
            }

            icoSphere.draw();
            material.end();
    }

    if (bWireframe) {
            ofNoFill();
            ofSetColor(0, 0, 0);
            icoSphere.setScale(1.01f);
            icoSphere.drawWireframe();
            icoSphere.setScale(1.f);
    }
}

void RendererPlus::set_primitive_3d_cylinder(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2){//int modeP);

    // Cylinder //
 // cylinder.set(abs(mouse_press_x -mouse_current_x), abs(mouse_current_y-mouse_press_y));
  cylinder.set(abs(mouse_press_x -mouse_press_x2), abs(mouse_press_y2-mouse_press_y));

  cylinder.setPosition(mouse_press_x - screenWidth* 0.5,
                       -mouse_press_y + screenHeight* 0.5, 0);

    if (mode == 3) {
            topCap = cylinder.getTopCapMesh();
            bottomCap = cylinder.getBottomCapMesh();
            body = cylinder.getCylinderMesh();
    }



    if (bFill) {
            material.begin();
            ofFill();
            if (mode == 3) {
                    cylinder.transformGL();
                    ofPushMatrix(); {
                            if (topCap.getNumNormals() > 0) {
                                    ofTranslate(topCap.getNormal(0) * (cos(ofGetElapsedTimef() * 5) + 1)*.5f * 100);
                                    topCap.draw();
                            }
                    } ofPopMatrix();
                    ofPushMatrix(); {
                            if (bottomCap.getNumNormals() > 0) {
                                    ofTranslate(bottomCap.getNormal(0) * (cos(ofGetElapsedTimef() * 4) + 1)*.5f * 100);
                                    bottomCap.draw();
                            }
                    } ofPopMatrix();
                    ofPushMatrix(); {
                            float scale = (cos(ofGetElapsedTimef() * 3) + 1)*.5f + .2;
                            ofScale(scale, scale, scale);
                            body.draw();
                    } ofPopMatrix();
                    cylinder.restoreTransformGL();
            }
            else {
                    cylinder.draw();
            }
            material.end();
    }

    if (bWireframe) {
            ofNoFill();
            ofSetColor(0, 0, 0);
            cylinder.setScale(1.01f);
            cylinder.drawWireframe();
            cylinder.setScale(1.0f);
    }
}

void RendererPlus::set_primitive_3d_cone(int mouse_press_x, int mouse_press_y,int mouse_press_x2, int mouse_press_y2){//int modeP);    // Cone //

 // cone.set(abs(mouse_press_x -mouse_current_x), abs(mouse_current_y-mouse_press_y));
  cone.set(abs(mouse_press_x -mouse_press_x2), abs(mouse_press_y2-mouse_press_y));

  cone.setPosition(mouse_press_x - screenWidth* 0.5,
                   -mouse_press_y + screenHeight* 0.5, 0);
    if (mode == 3) {
            bottomCap = cone.getCapMesh();
            body = cone.getConeMesh();
    }
    if (bFill) {
            material.begin();
            ofFill();
            if (mode == 3) {
                    cone.transformGL();
                    ofPushMatrix();
                    if (bottomCap.getNumNormals() > 0) {
                            ofTranslate(bottomCap.getNormal(0) * cone.getHeight()*.5);
                            ofRotateDeg(sin(ofGetElapsedTimef() * 5) * RAD_TO_DEG, 1, 0, 0);
                            bottomCap.draw();
                    }
                    ofPopMatrix();

                    ofPushMatrix();
                    ofRotateDeg(90, 1, 0, 0);
                    ofRotateDeg((cos(ofGetElapsedTimef() * 6) + 1)*.5 * 360, 1, 0, 0);
                    body.draw();
                    ofPopMatrix();
                    cone.restoreTransformGL();
            }
            else {
                    cone.draw();
            }
            material.end();
    }

    if (bWireframe) {
            ofNoFill();
            ofSetColor(0, 0, 0);
            cone.setScale(1.01f);
            cone.drawWireframe();
            cone.setScale(1.0f);
    }
}

// fonction qui ajoute une primitive vectorielle au tableau
void RendererPlus::add_vector_shape3d(VectorPrimitive3dType type)
{
  shapes3d[head3d].type = type;

 shapes3d[head3d].position1[0] = mouse_press_x;
 shapes3d[head3d].position1[1] = mouse_press_y;
 //shapes3d[head3d].position2[0] = mouse_press_x2;
 //shapes3d[head3d].position2[1] = mouse_press_y2;

 shapes3d[head3d].position2[0] = mouse_current_x;
 shapes3d[head3d].position2[1] = mouse_current_y;

  //shapes3d[head3d].position2[0] = last_mouse_released_x;
   //shapes3d[head3d].position2[1] = last_mouse_released_y;

 shapes3d[head3d].stroke_color[0] = stroke_color_r;
  shapes3d[head3d].stroke_color[1] = stroke_color_g;
 shapes3d[head3d].stroke_color[2] = stroke_color_b;
  shapes3d[head3d].stroke_color[3] = stroke_color_a;

  shapes3d[head3d].fill_color[0] = fill_color_r;
  shapes3d[head3d].fill_color[1] = fill_color_g;
  shapes3d[head3d].fill_color[2] = fill_color_b;
 shapes3d[head3d].fill_color[3] = fill_color_a;

  switch (shapes3d[head3d].type)
  {
    case VectorPrimitive3dType::plane:
      shapes3d[head3d].stroke_width = ofRandom(1, 64);
      break;

    case VectorPrimitive3dType::box:
      shapes3d[head3d].stroke_width = ofRandom(1, 16);
      break;

    case VectorPrimitive3dType::sphere:
      shapes3d[head3d].stroke_width = stroke_width3d_default;
      break;

    case VectorPrimitive3dType::icoSphere:
      shapes3d[head3d].stroke_width = stroke_width3d_default;
      break;
    case VectorPrimitive3dType::cylinder:
      shapes3d[head3d].stroke_width = stroke_width3d_default;
      break;
    case VectorPrimitive3dType::cone:
      shapes3d[head3d].stroke_width = stroke_width3d_default;
      break;
    default:
      shapes3d[head3d].stroke_width = stroke_width3d_default;
      break;
  }

  ofLog() << "<new primitive at index: " << head3d << ">";

  head3d = ++head3d >= count3d ? 0: head3d; // boucler sur le tableau si plein
}

//----------------------------------------------------------------

void RendererPlus::rotate_primitive_3D(){
  float spinX = sin(ofGetElapsedTimef()*.35f);
  float spinY = cos(ofGetElapsedTimef()*.075f);


  if (bMousePressed) {
          spinX = spinY = 0.0f;
  }

  //set rotation
  for(index3d=0; index3d<count3d;++index3d){
     if(shapes3d[index3d].isSelected){
      switch (modeP) {
        case '1':
          plane.rotateDeg(spinX, 1.0, 0.0, 0.0);
          plane.rotateDeg(spinY, 0, 1.0, 0.0);
          break;
        case '2':
          box.rotateDeg(spinX, 1.0, 0.0, 0.0);
          box.rotateDeg(spinY, 0, 1.0, 0.0);
          break;
        case '3':
          sphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
          sphere.rotateDeg(spinY, 0, 1.0, 0.0);

          break;
        case '4':
          icoSphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
          icoSphere.rotateDeg(spinY, 0, 1.0, 0.0);
          break;
        case '5':
          cylinder.rotateDeg(spinX, 1.0, 0.0, 0.0);
          cylinder.rotateDeg(spinY, 0, 1.0, 0.0);
          break;
        case '6':
          cone.rotateDeg(spinX, 1.0, 0.0, 0.0);
          cone.rotateDeg(spinY, 0, 1.0, 0.0);
          break;
        }
       }
    }
}

void RendererPlus::select_shapes3d(int x, int y, int z){

  for(int i=0; i<count3d;++i){

      bool z = ((x > shapes3d[i].position1[0] & x < shapes3d[i].position2[0]) or
          (x < shapes3d[i].position1[0] & x > shapes3d[i].position2[0])) &

          ((y > shapes3d[i].position1[1] & y < shapes3d[i].position2[1]) or
           (y < shapes3d[i].position1[1] & y > shapes3d[i].position2[1]));
      if(z){
          shapes3d[i].isSelected = !shapes3d[i].isSelected;

          if(shapes3d[i].isSelected){
            stroke_color_r = (int) 255;//ofRandom(0, 256);
            stroke_color_g = (int) 0;//ofRandom(0, 256);
            stroke_color_b = (int) 0;//ofRandom(0, 256);
            stroke_color_a = 255;
            shapes3d[head3d].stroke_color[0] = stroke_color_r;
            shapes3d[head3d].stroke_color[1] = stroke_color_g;
            shapes3d[head3d].stroke_color[2] = stroke_color_b;
            shapes3d[head3d].stroke_color[3] = stroke_color_a;
            ofLog() <<"shape "<< i <<" selected";
          }
          else
            ofLog() <<"shape "<< i <<" deselected";

        }
    }
}

void RendererPlus::move_shapes3d(int x, int y, int z){
   float tx, ty, tz;
   for(int i=0; i<count3d;++i){
      if(shapes3d[i].isSelected){

          tx = x - (shapes3d[i].position1[0]+shapes3d[i].position2[0])/2;
          ty = y - (shapes3d[i].position1[1]+shapes3d[i].position2[1])/2;
          tz = z - (shapes3d[i].position1[2]+shapes3d[i].position2[2])/2;
               shapes3d[i].position1[0] += tx;
               shapes3d[i].position1[1] += ty;
               shapes3d[i].position1[2] += tz;
               shapes3d[i].position2[0] += tx;
               shapes3d[i].position2[1] += ty;
               shapes3d[i].position2[1] += tz;

                 }
    }
 }

void RendererPlus::delete_selected_shapes3d(){
  for (index3d = 0; index3d < count3d; ++index3d){
      if(shapes3d[index3d].isSelected)
        shapes3d[index3d].type = VectorPrimitive3dType::none;
      //head-=1;
   }
}

void RendererPlus::selectAll3d(){
  for (index3d = 0; index3d < count3d; ++index3d)
    shapes3d[index3d].isSelected =true;
}

void RendererPlus::deselectAll3d(){
  for (index3d = 0; index3d < count3d; ++index3d)
    shapes3d[index3d].isSelected = false;
}

//-----------------------------------------------------------------





