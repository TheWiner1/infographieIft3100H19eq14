#pragma once

#include "ofMain.h"

typedef struct {

	float 	x;
	float 	y;
	bool 	  bBeingDragged;
	bool 	  bOver;
	float 	radius;
	
}draggableVertex;


class DraggableVertex : public ofBaseApp{
	
	public:
		
		void setup();
		void draw();

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		int nCurveVertices;
		draggableVertex curveVertices[7];
		draggableVertex bezierVertices[4];
};

	
