#include "mainRaytrace.h"

void ofAppRT::setup(){
	ofSetFrameRate(150);
			lanceRayon.load("raytracerIFT");
			w=ofGetScreenWidth();
			h=ofGetScreenHeight();
			fbo.allocate(w,h);
			fbo.begin();
			ofClear(0,0,0,0);
			fbo.end();
			guiRay.setup();
			s1.setName("sphere 1");
			s2.setName("sphere 2");
			s3.setName("sphere 3");
			s4.setName("cube");
			guiRay.add(s1.set("sphere 1"));
			guiRay.add(s2.set("sphere 2"));
			guiRay.add(s3.set("sphere 3"));
			guiRay.add(s4.set("cube"));
			guiRay.add(posCubemin.set("cube min", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			guiRay.add(posCubemax.set("cube max", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));

			guiRay.add(couleur1.set("couleur primitive", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			guiRay.add(rayon1.set("rayon primitive", .6,.0,10. ));
			guiRay.add(posSphere1.set("position primitive", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			guiRay.add(intLight.set("intensite de la lumiere",.4,.0,5.  ));
			guiRay.add(speLight.set("reflexion speculaire", ofVec2f(1.,50.), ofVec2f(0.,0.), ofVec2f(100.,100.) ));
			guiRay.add(colLight.set("couleur de la lumiere", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			guiRay.add(ambient.set("lumiere ambiante", .2,.0,1. ));
			guiRay.add(position_lumiere.set("position de la lumiere", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			s1=s2=s3=s4=true;

/*			ofSetFrameRate(150);
			lanceRayon.load("ray");
			w=ofGetScreenWidth();
			h=ofGetScreenHeight();
			fbo.allocate(w,h);
			fbo.begin();
			ofClear(0,0,0,0);
			fbo.end();
			guiRay.setup();
			guiRay.add(couleur1.set("couleur sphere 1", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			//guiRay.add(couleur2.set("couleur sphere 2", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			//guiRay.add(couleur2.set("couleur sphere 3", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			guiRay.add(rayon1.set("rayon sphere 1", .6,.0,10. ));
			//guiRay.add(rayon2.set("rayon sphere 2", .6,.0,10. ));
			//guiRay.add(rayon3.set("rayon sphere 3", .6,.0,10. ));
			guiRay.add(intLight.set("intensity light",.4,.0,5.  ));
			guiRay.add(speLight.set("specular light", ofVec2f(1.,50.), ofVec2f(0.,0.), ofVec2f(100.,100.) ));
			guiRay.add(colLight.set("couleur light", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			guiRay.add(ambient.set("ambient light", .2,.0,1. ));

			guiRay.add(posSphere1.set("position sphere 1", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			//guiRay.add(posSphere2.set("position sphere 2", ofVec3f(-.75,.1,2.25),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			//guiRay.add(posSphere3.set("position sphere 3", ofVec3f(-.75,.1,2.25),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
*/
		}

		void ofAppRT::update(){
			ofSetWindowTitle(ofToString(ofGetFrameRate()));
		}

		void ofAppRT::draw(){
			ofBackgroundGradient(255,0);

			lanceRayon.begin();

			lanceRayon.setUniform1f("u_aspect_ratio",w/h);
			if(s1){
				lanceRayon.setUniform3f("sphere_position_0",posSphere1->x, posSphere1->y, posSphere1->z);
				lanceRayon.setUniform1f("sphere_rayon_0",rayon1);
				lanceRayon.setUniform3f("sphere_couleur_0",couleur1->x,couleur1->y,couleur1->z);
			}
			if(s2){			
				lanceRayon.setUniform3f("sphere_position_1",posSphere1->x, posSphere1->y, posSphere1->z);
				lanceRayon.setUniform1f("sphere_rayon_1",rayon1);
				lanceRayon.setUniform3f("sphere_couleur_1", couleur1->x,couleur1->y,couleur1->z);
			}
			if(s3){
				lanceRayon.setUniform3f("sphere_position_2",posSphere1->x, posSphere1->y, posSphere1->z);
				lanceRayon.setUniform1f("sphere_rayon_2",rayon1);
				lanceRayon.setUniform3f("sphere_couleur_2", couleur1->x,couleur1->y,couleur1->z);
			}
			if(s4){
				lanceRayon.setUniform3f("cube_position",posSphere1->x, posSphere1->y, posSphere1->z);
				lanceRayon.setUniform3f("cube_couleur", couleur1->x,couleur1->y,couleur1->z);
				lanceRayon.setUniform3f("cube_min",posCubemin->x, posCubemin->y, posCubemin->z);
				lanceRayon.setUniform3f("cube_max",posCubemax->x, posCubemax->y, posCubemax->z);
			}
			lanceRayon.setUniform3f("plan_position",0., -.5, 0.);
			lanceRayon.setUniform3f("plan_normal",0.,1.,0.043);

			lanceRayon.setUniform1f("light_intensity",intLight);
			lanceRayon.setUniform2f("light_specular",speLight->x,speLight->y);
//			lanceRayon.setUniform3f("light_position",5.,5.,-5.);
			lanceRayon.setUniform3f("light_position",position_lumiere->x,position_lumiere->y,
			position_lumiere->z);
			lanceRayon.setUniform3f("light_couleur",colLight->x,colLight->y,colLight->z);
			lanceRayon.setUniform1f("ambient",ambient);
			lanceRayon.setUniform3f("O", 0., 0., -1.);

			fbo.draw(-100,-100);

			lanceRayon.end();		
			guiRay.draw();
		}

		void ofAppRT::keyPressed(int key){
			if(key == 'f')
				ofToggleFullscreen();
		}

