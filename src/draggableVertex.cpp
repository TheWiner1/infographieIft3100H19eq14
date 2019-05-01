#include "draggableVertex.h"

void DraggableVertex::setup(){
	nCurveVertices = 7;
	
	curveVertices[0].x = 326;
	curveVertices[0].y = 209;
	curveVertices[1].x = 306;
	curveVertices[1].y = 279;
	curveVertices[2].x = 265;
	curveVertices[2].y = 331;
	curveVertices[3].x = 304;
	curveVertices[3].y = 383;
	curveVertices[4].x = 374;
	curveVertices[4].y = 383;
	curveVertices[5].x = 418;
	curveVertices[5].y = 309;
	curveVertices[6].x = 345;
	curveVertices[6].y = 279;
	
	for (int i = 0; i < nCurveVertices; i++){
		curveVertices[i].bOver 			= false;
		curveVertices[i].bBeingDragged 	= false;
		curveVertices[i].radius = 4;
	}
}

void DraggableVertex::draw(){
	ofFill();
	ofSetHexColor(0xe0be21);

	ofBeginShape();
		for (int i = 0; i < nCurveVertices; i++){
			if (i == 0){
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y); 
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y); 
			} else if (i == nCurveVertices-1){
				ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	
				ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	
			} else {
				ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
			}
		}
	ofEndShape();
	
	ofEnableAlphaBlending();
	ofNoFill();
	ofSetColor(0,0,0,40);
	ofBeginShape();
		for (int i = 0; i < nCurveVertices; i++){
			ofVertex(curveVertices[i].x, curveVertices[i].y);
		}
	ofEndShape(true);
		
	ofSetColor(0,0,0,80);
	for (int i = 0; i < nCurveVertices; i++){
		if (curveVertices[i].bOver == true) ofFill();
		else ofNoFill();
		ofDrawCircle(curveVertices[i].x, curveVertices[i].y,4);
	}
	ofDisableAlphaBlending();
}

void DraggableVertex::mouseMoved(int x, int y ){
	for (int i = 0; i < nCurveVertices; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bOver = true;
		} else {
			curveVertices[i].bOver = false;
		}	
	}
}

void DraggableVertex::mouseDragged(int x, int y, int button){
	for (int i = 0; i < nCurveVertices; i++){
		if (curveVertices[i].bBeingDragged == true){
			curveVertices[i].x = x;
			curveVertices[i].y = y;
		}
	}
}

void DraggableVertex::mousePressed(int x, int y, int button){
	for (int i = 0; i < nCurveVertices; i++){
		float diffx = x - curveVertices[i].x;
		float diffy = y - curveVertices[i].y;
		float dist = sqrt(diffx*diffx + diffy*diffy);
		if (dist < curveVertices[i].radius){
			curveVertices[i].bBeingDragged = true;
		} else {
			curveVertices[i].bBeingDragged = false;
		}	
	}
}

void DraggableVertex::mouseReleased(int x, int y, int button){

	for (int i = 0; i < nCurveVertices; i++){
		curveVertices[i].bBeingDragged = false;	
	}
}