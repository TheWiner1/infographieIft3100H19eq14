#include "mainRaytrace.h"

void ofAppRT::setup(){
			ofSetFrameRate(150);
			lanceRayon.load("ray");
			w=ofGetScreenWidth();
			h=ofGetScreenHeight();
			fbo.allocate(w,h);
			fbo.begin();
			ofClear(0,0,0,0);
			fbo.end();
			gui.setup();
			s1.setName("sphere 1");
			s2.setName("sphere 2");
			s3.setName("sphere 3");
			s4.setName("cube");
			gui.add(s1.set("sphere 1"));
			gui.add(s2.set("sphere 2"));
			gui.add(s3.set("sphere 3"));
			gui.add(s4.set("cube"));
			gui.add(posCubemin.set("cube min", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			gui.add(posCubemax.set("cube max", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));

			gui.add(color1.set("couleur primitive", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			gui.add(radius1.set("rayon primitive", .6,.0,10. ));
			gui.add(posSphere1.set("position primitive", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			gui.add(intLight.set("intensite de la lumiere",.4,.0,5.  ));
			gui.add(speLight.set("reflexion speculaire", ofVec2f(1.,50.), ofVec2f(0.,0.), ofVec2f(100.,100.) ));
			gui.add(colLight.set("couleur de la lumiere", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			gui.add(ambient.set("lumiere ambiante", .2,.0,1. ));
			gui.add(position_lumiere.set("position de la lumiere", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			s1=s2=s3=s4=true;

/*			ofSetFrameRate(150);
			lanceRayon.load("ray");
			w=ofGetScreenWidth();
			h=ofGetScreenHeight();
			fbo.allocate(w,h);
			fbo.begin();
			ofClear(0,0,0,0);
			fbo.end();
			gui.setup();
			gui.add(color1.set("color sphere 1", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			//gui.add(color2.set("color sphere 2", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			//gui.add(color2.set("color sphere 3", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			gui.add(radius1.set("radius sphere 1", .6,.0,10. ));
			//gui.add(radius2.set("radius sphere 2", .6,.0,10. ));
			//gui.add(radius3.set("radius sphere 3", .6,.0,10. ));
			gui.add(intLight.set("intensity light",.4,.0,5.  ));
			gui.add(speLight.set("specular light", ofVec2f(1.,50.), ofVec2f(0.,0.), ofVec2f(100.,100.) ));
			gui.add(colLight.set("color light", ofVec3f(0.),ofVec3f(0.),ofVec3f(1.) ));
			gui.add(ambient.set("ambient light", .2,.0,1. ));

			gui.add(posSphere1.set("position sphere 1", ofVec3f(.75,.1,1.),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			//gui.add(posSphere2.set("position sphere 2", ofVec3f(-.75,.1,2.25),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
			//gui.add(posSphere3.set("position sphere 3", ofVec3f(-.75,.1,2.25),ofVec3f(-5.,-5.,-5.),ofVec3f(5.,5.,5.) ));
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
				lanceRayon.setUniform1f("sphere_radius_0",radius1);
				lanceRayon.setUniform3f("sphere_color_0",color1->x,color1->y,color1->z);
			}
			if(s2){			
				lanceRayon.setUniform3f("sphere_position_1",posSphere1->x, posSphere1->y, posSphere1->z);
				lanceRayon.setUniform1f("sphere_radius_1",radius1);
				lanceRayon.setUniform3f("sphere_color_1", color1->x,color1->y,color1->z);
			}
			if(s3){
				lanceRayon.setUniform3f("sphere_position_2",posSphere1->x, posSphere1->y, posSphere1->z);
				lanceRayon.setUniform1f("sphere_radius_2",radius1);
				lanceRayon.setUniform3f("sphere_color_2", color1->x,color1->y,color1->z);
			}
			if(s4){
				lanceRayon.setUniform3f("cube_position",posSphere1->x, posSphere1->y, posSphere1->z);
				lanceRayon.setUniform3f("cube_color", color1->x,color1->y,color1->z);
				lanceRayon.setUniform3f("cube_min",posCubemin->x, posCubemin->y, posCubemin->z);
				lanceRayon.setUniform3f("cube_max",posCubemax->x, posCubemax->y, posCubemax->z);
			}
			lanceRayon.setUniform3f("plane_position",0., -.5, 0.);
			lanceRayon.setUniform3f("plane_normal",0.,1.,0.043);

			lanceRayon.setUniform1f("light_intensity",intLight);
			lanceRayon.setUniform2f("light_specular",speLight->x,speLight->y);
//			lanceRayon.setUniform3f("light_position",5.,5.,-5.);
			lanceRayon.setUniform3f("light_position",position_lumiere->x,position_lumiere->y,
			position_lumiere->z);
			lanceRayon.setUniform3f("light_color",colLight->x,colLight->y,colLight->z);
			lanceRayon.setUniform1f("ambient",ambient);
			lanceRayon.setUniform3f("O", 0., 0., -1.);

			fbo.draw(-100,-100);

			lanceRayon.end();		
			gui.draw();
		}

		void ofAppRT::keyPressed(int key){
			if(key == 'f')
				ofToggleFullscreen();
		}

