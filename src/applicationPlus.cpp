// IFT3100H19_ImageExport/applicationPlus.cpp
// Classe principale de l'applicationPlus.

#include "applicationPlus.h"


// fonction appelée quand la position du curseur change pendant qu'un bouton d'un périphérique de pointage est appuyé
void ApplicationPlus::mouseDragged(int x, int y, int button)
{
  rendererPlus.mouse_current_x = x;
  rendererPlus.mouse_current_y = y;

//  if (is_verbose)
//    ofLog() << "<app::mouse dragged  at: (" << x << ", " << y << ") button:" << button <<">";
  if(mode_draw==true && button==2){
    //rendererPlus.select_shapes(x,y);
    cout<<"";//rendererPlus.move_shapes(x,y);
    }
  if(mode_draw_3d==true && button==2){
    rendererPlus.select_shapes3d(x,y,0);
    rendererPlus.move_shapes3d(x,y,0);
    }
}

void ApplicationPlus::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

// fonction appelée quand le curseur entre dans la fenêtre d'affichage
void ApplicationPlus::mouseEntered(int x, int y)
{
  rendererPlus.mouse_current_x = x;
  rendererPlus.mouse_current_y = y;

 // ofLog() << "<app::mouse entered  at: (" << x << ", " << y << ")>";
}

// fonction appelée quand le curseur sort de la fenêtre d'affichage
void ApplicationPlus::mouseExited(int x, int y)
{
  rendererPlus.mouse_current_x = x;
  rendererPlus.mouse_current_y = y;

//  ofLog() << "<app::mouse exited   at: (" << x << ", " << y << ")>";
}

void ApplicationPlus::mouseMoved(int x, int y){

  //cursor
  rendererPlus.mouse_current_x = x;
  rendererPlus.mouse_current_y = y;

  //if (is_verbose)
 //   ofLog() << "<app::mouse moved    at: (" << x << ", " << y << ")>";

  if(!mode_draw && !mode_draw_3d){
    for(auto it: images){
        if(it.getTrueFalse() == true)
              it.x0 = x;
              it.y0 = y;
              for (unsigned int i=0; i<it.loadedImages.size(); i++){
                  it.loadedImages[i].draw(it.x0, it.y0, it.width, it.height);}
        }
    }

}

void ApplicationPlus::mousePressed(int x, int y, int button){

  if(!mode_draw & !mode_draw_3d){
    for(auto it: images){
        if(x>=it.x0 && x<=it.width && y>=it.y0 && y<=it.height && button==0)
          it.setTrueFalse();
        for (unsigned int i=0; i<it.loadedImages.size(); i++){
            it.loadedImages[i].draw(it.x0, it.y0, it.width, it.height);}
      }
    }
  rendererPlus.nBpress++;
  if(rendererPlus.nBpress%2==0 & button ==0){
    rendererPlus.mouse_press_x2 = x;
    rendererPlus.mouse_press_y2 = y;

  }

  //cursor
  rendererPlus.is_mouse_button_pressed = true;

  rendererPlus.mouse_current_x = x;
  rendererPlus.mouse_current_y = y;

  rendererPlus.mouse_press_x = x;
  rendererPlus.mouse_press_y = y;

//  ofLog() << "<app::mouse pressed  at: (" << x << ", " << y << ")>";

  if(mode_draw == true && button == 2){
      cout<<"1";//rendererPlus.select_shapes(x,y);
    }
  if(mode_draw_3d == true && button == 2){
      rendererPlus.select_shapes3d(x,y,0);
    }
}

void ApplicationPlus::mouseReleased(int x, int y, int button){

  rendererPlus.mouse_current_x = x;
  rendererPlus.mouse_current_y = y;
  rendererPlus.last_mouse_released_x = x;
  rendererPlus.last_mouse_released_x = y;
  rendererPlus.is_mouse_button_pressed = false;

  if(!mode_draw & !mode_draw_3d){
    for(auto it:images){
      if(it.getTrueFalse() == true && button==0)
        it.x0 = x;
        it.y0 = y;
        it.setTrueFalse();
        for (unsigned int i=0; i<it.loadedImages.size(); i++){
          it.loadedImages[i].draw(it.x0, it.y0, it.width, it.height);
          }
    }
  }
  //cursor

  if(mode_draw==true && button==0){
      cout<<"";//rendererPlus.add_vector_shape(rendererPlus.draw_mode);
    }
  if(mode_draw_3d==true && button==0){
      rendererPlus.add_vector_shape3d(rendererPlus.draw_mode3d);
    }
  /*if(mode_draw==true && button==2){
      rendererPlus.select_shapes(x,y);
   }*/

  //ofLog() << "<app::mouse released at: (" << x << ", " << y << ")>";


}

void ApplicationPlus::draw()
{
  rendererPlus.draw();

  if (checkbox)
    gui2.draw();
  int n = 0;
  int j = 0;
  if(!mode_draw){
    int width = 400;
    int height = 400;
    for(auto it:images){
        it.x0 = n;
        it.y0 = j;
        it.width = width;
        it.height = height;
        for (unsigned int i=0; i<it.loadedImages.size(); i++){
            it.loadedImages[i].draw(it.x0, it.y0, it.width, it.height);
            //ofSetWindowShape(it.loadedImages[i].getWidth(), it.loadedImages[i].getHeight());
        }

            n+=width;
            if(n-width>=1300){
                  n=0;
                  j+=height;}
      }
    }
      /*
        for (unsigned int i=0; i<processedImages.size(); i++){
               // it.processedImages[i].draw(it.processedImages[i].getWidth(), 20);
            it.processedImages[i].draw(it.processedImages[i].getWidth(), it.processedImages[i].getHeight());
            i+=100;
            if(i-100>=1500)
              j+=100;
        }  */
      //
  
}

// fonction appelée quand une touche du clavier est relachée
void ApplicationPlus::keyReleased(int key)
{
  if(mode_draw_3d){
      switch (key){
      case '1':  // key 1
        rendererPlus.draw_mode3d = VectorPrimitive3dType::plane;
        ofLog() << "<mode: pixel>";
        break;

      case '2':  // key 2
        rendererPlus.draw_mode3d = VectorPrimitive3dType::box;
        ofLog() << "<mode: point>";
        break;

      case '3':  // key 3
        rendererPlus.draw_mode3d = VectorPrimitive3dType::sphere;
        ofLog() << "<mode: line>";
        break;

      case '4':  // key 4
        rendererPlus.draw_mode3d = VectorPrimitive3dType::icoSphere;
        ofLog() << "<mode: rectangle>";
        break;

      case '5':  // key 5
        rendererPlus.draw_mode3d = VectorPrimitive3dType::cylinder;
        ofLog() << "<mode: ellipse>";
        break;
      case '6':  // key 6
        rendererPlus.draw_mode3d = VectorPrimitive3dType::cone;
        ofLog() << "<mode: ellipse>";
        break;

      case 102: // key f
       // rendererPlus.random_color_fill();
        break;
  /*
      case 114: // key r
        rendererPlus.reset();
        break;*/

      case 115: // key s
        //rendererPlus.random_color_stroke();
        break;
      /*case ' ':
        if(save_by_clic==true)
          rendererPlus.image_export("render", "png");

      case 117: // touche u
          checkbox = !checkbox;
          ofLog() << "<toggle ui: " << checkbox << ">";
      case 118: // touche v
        is_verbose = !is_verbose;
        ofLog() << "<verbose mode: " << is_verbose << ">";
        break;

      default:

        rendererPlus.filter = !rendererPlus.filter;

        if (rendererPlus.filter)
          rendererPlus.shader = rendererPlus.shader_invert;
        else
          rendererPlus.shader = rendererPlus.shader_passthrough;
        break;*/
        }
    }

    ofLog() << "<app::keyReleased: " << key << ">";
  }

void ApplicationPlus::keyPressed(int key) {
          switch(key) {
          case 'f':
                          ofToggleFullscreen();
                          break;
          case 's':
              bFill = !bFill;
              break;
          case 'w':
              bWireframe = !bWireframe;
              break;
          case '7':
              bSplitFaces=false;
              sphere.setResolution(4);
              // icosahedron //
              icoSphere.setResolution(0); // number of subdivides //
              plane.setResolution(3, 2);
              cylinder.setResolution(4,2,0);
              cone.setResolution(4, 1, 0);
              box.setResolution(1);
              break;
          case '8':
              bSplitFaces=false;
              sphere.setResolution(8);
              icoSphere.setResolution(1);
              plane.setResolution(6, 4);
              cylinder.setResolution(8,4,1);
              cone.setResolution(7, 2, 1);
              box.setResolution(2);
              break;
          case '9':
              bSplitFaces=false;
              sphere.setResolution(16);
              icoSphere.setResolution(2);
              plane.setResolution(8,5);
              cylinder.setResolution(12, 9, 2);
              cone.setResolution(10, 5, 2);
              box.setResolution(6);
              break;
          case '0':
              bSplitFaces=false;
              sphere.setResolution(48);
              icoSphere.setResolution(4);
              plane.setResolution(12, 9);
              cylinder.setResolution(20, 13, 4);
              cone.setResolution(20, 9, 3);
              box.setResolution(10);
              break;
          case 'n':
              bDrawNormals = !bDrawNormals;
              break;
          case OF_KEY_RIGHT:
              mode++;
              if(mode > 3) mode = 0;
              if(mode==3){
                  // to get unique triangles, you have to use triangles mode //
                  sphere.setMode( OF_PRIMITIVE_TRIANGLES );
              }
              break;
          case OF_KEY_LEFT:
              mode--;
              if(mode < 0) mode = 3;
              if(mode==3){
                  // to get unique triangles, you have to use triangles mode //
                  sphere.setMode( OF_PRIMITIVE_TRIANGLES );
              }
              break;
          case 'a':
              bDrawAxes = !bDrawAxes;
              break;
          case 'l':
              bDrawLights = !bDrawLights;
              break;
          case 'h':
              bHelpText=!bHelpText;
              break;
          case 'z':
              bSplitFaces = !bSplitFaces;

              if(mode == 3) bSplitFaces = false;

              if(bSplitFaces) {
                  sphere.setMode( OF_PRIMITIVE_TRIANGLES );
                  vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
                  sphere.getMesh().setFromTriangles( triangles, true );

                  icoSphere.setMode( OF_PRIMITIVE_TRIANGLES );
                  triangles = icoSphere.getMesh().getUniqueFaces();
                  icoSphere.getMesh().setFromTriangles(triangles, true);

                  plane.setMode( OF_PRIMITIVE_TRIANGLES );
                  triangles = plane.getMesh().getUniqueFaces();
                  plane.getMesh().setFromTriangles(triangles, true);

                  cylinder.setMode( OF_PRIMITIVE_TRIANGLES );
                  triangles = cylinder.getMesh().getUniqueFaces();
                  cylinder.getMesh().setFromTriangles(triangles, true);

                  cone.setMode( OF_PRIMITIVE_TRIANGLES );
                  triangles = cone.getMesh().getUniqueFaces();
                  cone.getMesh().setFromTriangles(triangles, true);

                  box.setMode( OF_PRIMITIVE_TRIANGLES );
                  triangles = box.getMesh().getUniqueFaces();
                  box.getMesh().setFromTriangles(triangles, true);

              } else {
                  // vertex normals are calculated with creation, set resolution //
                  sphere.setResolution( sphere.getResolution() );

                  icoSphere.setResolution( icoSphere.getResolution() );
                  plane.setResolution( plane.getNumColumns(), plane.getNumRows() );

                  cylinder.setResolution( cylinder.getResolutionRadius(), cylinder.getResolutionHeight(), cylinder.getResolutionCap() );
                  cone.setResolution( cone.getResolutionRadius(), cone.getResolutionHeight(), cone.getResolutionCap() );
                  box.setResolution( box.getResolutionWidth() );
              }
              break;

              case '1':
              //  if(mode_draw_3d == true)
                  rendererPlus.modeP = '1';
                break;
              case '2':
              //  if(mode_draw_3d == true)
                  rendererPlus.modeP = '2';
                break;
              case '3':
               // if(mode_draw_3d == true)
                  rendererPlus.modeP = '3';
                break;
              case '4':
                //if(mode_draw_3d == true)
                  rendererPlus.modeP = '4';
                break;
              case '5':
               // if(mode_draw_3d == true)
                  rendererPlus.modeP = '5';
                break;
              case '6':
                //if(mode_draw_3d == true)
                  rendererPlus.modeP = '6';
                break;
          }

      if(mode == 1) {
          // resize the plane to the size of the texture //
          plane.resizeToTexture( texture.getTexture() );
          // setTexCoordsFromTexture sets normalized or non-normalized tex coords based on an ofTexture passed in.
          box.mapTexCoordsFromTexture( texture.getTexture() );
          sphere.mapTexCoordsFromTexture( texture.getTexture() );
          icoSphere.mapTexCoordsFromTexture( texture.getTexture() );
          cylinder.mapTexCoordsFromTexture( texture.getTexture() );
          cone.mapTexCoordsFromTexture( texture.getTexture() );
      }

      if(mode == 2) {
          plane.resizeToTexture( vidGrabber.getTexture(), .5 );
          box.mapTexCoordsFromTexture( vidGrabber.getTexture() );
          sphere.mapTexCoordsFromTexture( vidGrabber.getTexture() );
          icoSphere.mapTexCoordsFromTexture( vidGrabber.getTexture() );
          cylinder.mapTexCoordsFromTexture( vidGrabber.getTexture() );
          cone.mapTexCoordsFromTexture( vidGrabber.getTexture() );
      }

      //
      if( mode == 3 ) {

          bSplitFaces = false;

          // if the faces were split, we can get some weird results, since we
          // might not know what the new strides were,
          // so reset the primitives by calling their setMode function
          // which recreates the mesh with the proper indicies //
          sphere.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
          icoSphere.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
          cylinder.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
          cone.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
          // box only supports triangles //
          box.setMode( OF_PRIMITIVE_TRIANGLES );

          plane.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
          plane.mapTexCoords(0, 0, 5, 5);

          // rebuild the box,
          box.mapTexCoords(0, 0, 5, 5);
          sphere.mapTexCoords(0, 0, 5, 5);
          icoSphere.mapTexCoords(0, 0, 5, 5);
          cylinder.mapTexCoords(0, 0, 5, 5);
          cone.mapTexCoords(0, 0, 5, 5);

          // store the box sides so that we can manipulate them later //
          for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
              rendererPlus.boxSides[i] = box.getSideMesh( i );
          }
      }

  }

void ApplicationPlus::windowResized(int w, int h)
  {
    ofLog() << "<app::windowResized to: (" << w << ", " << h << ")>";
  }

void ApplicationPlus::dragEvent(ofDragInfo dragInfo)
  {

    if(drag_to_import ==true && !mode_draw){
        ofLog() << "<app::ofDragInfo file[0]: " << dragInfo.files.at(0)
        << " at : " << dragInfo.position << ">";
        rendererPlus.image.load(dragInfo.files.at(0));
        if (rendererPlus.image.getWidth() > 0 && rendererPlus.image.getHeight() > 0)
          ofSetWindowShape(rendererPlus.image.getWidth(), rendererPlus.image.getHeight());
    }
  }

void ApplicationPlus::setup()
{
    is_verbose = false;

    //ofSetWindowTitle("interface (u)");
    ofNoFill();

    ofLog() << "<app::setup>";

    rendererPlus.setup();

    gui2.setup("interface");

    gestion_fichiers.setup("gestion fichiers");
    dessin_vectoriel.setup("dessin vectoriel");
    dessin_vectoriel_3D.setup("dessin vectoriel 3D");
    color_picker_background.set("couleur du canevas", ofColor(31), ofColor(0, 0), ofColor(255, 255));
    color_picker_stroke.set("couleur du trait", ofColor(255), ofColor(0, 0), ofColor(255, 255));

    slider_stroke_weight.set("largeur de la ligne", 4.0f, 0.0f, 10.0f);
    //textbox.set("text", "ift3100");
    //gui2.add(textbox);

    button.setup("button");
    button.addListener(this, &ApplicationPlus::button_pressed);

    button_import.setup("importer une image");
    button_import.addListener(this, &ApplicationPlus::buttonimportPressed);
    gestion_fichiers.add(&button_import);
    button_export.setup("enregistrer");
    button_export.addListener(this, &ApplicationPlus::buttonExportPressed);
    gestion_fichiers.add(&button_export);
    button_delete_shapes.setup("supprimer les objets");
    button_delete_shapes.addListener(this, &ApplicationPlus::buttonDeleteShapesPressed);
    mode_draw.setName("dessin vectoriel");
    button_select_all.setup("selectionner tout");
    button_deselect_all.setup("deselectionner tout");

    button_select_all.addListener(this,&ApplicationPlus::buttonSelectAllPressed);
    button_deselect_all.addListener(this,&ApplicationPlus::buttonDeselectAllPressed);

    save_by_clic.setName("appuyer la touche 'espace' pour sauvegarder");


    button_delete_shapes_3D.setup("supprimer les objets");
    button_delete_shapes_3D.addListener(this, &ApplicationPlus::buttonDeleteShapesPressed);
    mode_draw_3d.setName("dessin vectoriel 3D");
    mode_rotate_3d.setName("rotation des elements");
    //mode_rotate_3d.addListener(this, &ApplicationPlus::rotation_3D);
    group_draw.setup("outils de dessin");
    group_draw.add(color_picker_background);
    group_draw.add(color_picker_stroke);
    group_draw.add(slider_stroke_weight);
    gui2.add(&group_draw);
    gui2.add(&gestion_fichiers);
    drag_to_import.setName("glisser une image");
    gestion_fichiers.add(drag_to_import);
    gestion_fichiers.add(save_by_clic);
    dessin_vectoriel.add(mode_draw);
    dessin_vectoriel.add(&button_delete_shapes);
    dessin_vectoriel.add(&button_select_all);
    dessin_vectoriel.add(&button_deselect_all);
    dessin_vectoriel_3D.add(mode_rotate_3d);
    dessin_vectoriel_3D.add(mode_draw_3d);
    dessin_vectoriel_3D.add(&button_delete_shapes_3D);
    gui2.add(&dessin_vectoriel);
    gui2.add(&dessin_vectoriel_3D);
    gui2.add(checkbox);
    gui2.add(&button);

    mode_draw = false;
    save_by_clic = false;
    drag_to_import = false;
    mode_draw_3d=false;
    mode_rotate_3d=false;
    //checkbox.setName("visible");
    //checkbox = true;

  }

void ApplicationPlus::update()
  {
    // assigner les états courants de l'interface
    rendererPlus.background_color = color_picker_background;
    rendererPlus.stroke_color = color_picker_stroke;
    rendererPlus.stroke_weight = slider_stroke_weight;
    rendererPlus.text = textbox;

    rendererPlus.update();
  }

void ApplicationPlus::buttonimportPressed(){

          //if (key == ' '){
                  //Open the Open File Dialog
                  ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a jpg or png");

                  //Check if the user opened a file
                  if (openFileResult.bSuccess){

                          ofLogVerbose("User selected a file");

                          //We have a file, check it and process it
                          processOpenFileSelection(openFileResult);

                  }else {
                          ofLogVerbose("User hit cancel");
                  }
          //}
  }

void ApplicationPlus::buttonExportPressed(){
          //if (){

                  if (processedImages.size()==0 || loadedImages.size()==0){
                          //User is trying to save without anything to output - bail
                          return;
                  }

                  //
                  ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + "." + ofToLower(originalFileExtension), "Save your file");
                  if (saveFileResult.bSuccess){
                          processedImages[0].save(saveFileResult.filePath);
                          loadedImages[0].save(saveFileResult.filePath);
                  }
          //}


  }

//Sort function for stl::sort http://www.cplusplus.com/reference/algorithm/sort/
bool sortColorFunction (ofColor i,ofColor j) {
	return (i.getBrightness()<j.getBrightness());
}


void ApplicationPlus::processOpenFileSelection(ofFileDialogResult openFileResult){
	ofLogVerbose("getName(): "  + openFileResult.getName());
	ofLogVerbose("getPath(): "  + openFileResult.getPath());

	ofFile file (openFileResult.getPath());

	if (file.exists()){
		//Limiting this example to one image so we delete previous ones
		processedImages.clear();
		loadedImages.clear();

		ofLogVerbose("The file exists - now checking the type via file extension");
		string fileExtension = ofToUpper(file.getExtension());

		//We only want images
		if (fileExtension == "JPG" || fileExtension == "PNG") {

			//Save the file extension to use when we save out
			originalFileExtension = fileExtension;

			//Load the selected image
			ofImage image;
			image.load(openFileResult.getPath());
			if (image.getWidth()>ofGetWidth() || image.getHeight() > ofGetHeight())
			{
				image.resize(image.getWidth()/2, image.getHeight()/2);
			}
			loadedImages.push_back(image);

			//Make some short variables
			int w = image.getWidth();
			int h = image.getHeight();

			//Make a new image to save manipulation by copying the source
			ofImage processedImage = image;

			//Walk through the pixels
			for (int y = 0; y < h; y++){

				//Create a vector to store and sort the colors
				vector<ofColor> colorsToSort;

				for (int x = 0; x < w; x++){

					//Capture the colors for the row
					ofColor color = image.getColor(x, y);
					colorsToSort.push_back(color);
				}

				//Sort the colors for the row
				sort (colorsToSort.begin(), colorsToSort.end(), sortColorFunction);

				for (int x = 0; x < w; x++)
				{
					//Put the sorted colors back in the new image
					processedImage.setColor(x, y, colorsToSort[x]);
				}
			}
			//Store the processed image
			processedImages.push_back(processedImage);
		}
	}
	Imagelist a = Imagelist(loadedImages, processedImages);
	images.push_back(a);

}

void ApplicationPlus::button_pressed()
{
  // réinitialiser la zone de texte
  textbox.set("text", "Ervin Anoh");

  ofLog() << "<button pressed>";
}

void ApplicationPlus::buttonDeleteShapesPressed(){
  if(mode_draw==true)
    cout<<"";//rendererPlus.delete_selected_shapes();
  else if(mode_draw_3d==true)
    rendererPlus.delete_selected_shapes3d();
}

void ApplicationPlus::buttonSelectAllPressed(){
  if(mode_draw==true)
    cout<<"";//rendererPlus.selectAll();
  else if(mode_draw_3d==true)
    rendererPlus.selectAll3d();
}

void ApplicationPlus::buttonDeselectAllPressed(){
  if(mode_draw==true)
    cout<<"";//rendererPlus.deselectAll();
  else if(mode_draw_3d==true)
    rendererPlus.deselectAll3d();
}

/*  */

void ApplicationPlus::exit()
{
  ofLog() << "<app::exit>";
}
