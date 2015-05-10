#include <stdio.h>
#include <iostream>
#include <GL/freeglut.h> 
#include <GL/gl.h> 
#include <algorithm>
#include <cmath>
#include <string> 
#include <fstream> 

//#import "functionGrapher.h"
#include "Vector3f.h" 
#include "displayBox.h"
#include "graphPoints.h"
//#import "grandomPoints.h"
//#import "perceptronPlane.h"
//#import "frameHighlight.h"

float yRand = -10.0+(float)(rand())/(float)(RAND_MAX/(10.0-(-10.0)));

using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Awsome Box of Electric Field!!!";

const float WALKING_SPEED = 10.0;
const float MOUSE_SENSITIVITY = 0.2;

float LAST_TIME;
float CURRENT_TIME;
float DELTA_TIME;

float graphTime = 0; 

int MOUSE_LAST_X;
int MOUSE_LAST_Y;
int MOUSE_CURRENT_X;
int MOUSE_CURRENT_Y;
int MOUSE_DELTA_X;
int MOUSE_DELTA_Y;

Vector3f CAMERA_POSITION;
Vector3f CAMERA_ROTATION;

bool KEYS[256];

void display();
void reshape(int screenDisplayWidth, int screenDisplayHeight);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void mouseMove(int x, int y);
void mouseClick(int button, int state, int x, int y);
void preprocessEvents();

bool leftButton = false;

// GLOABLA GRAPH VARIABLES
Graph graph; 
void loadGraphFromDatafile(Graph &graph, string datafilename); 

int main(int argc, char** argv) {
	// Load Graph from datafile
	loadGraphFromDatafile(graph, "dataPoints.csv"); 	
	
	graph.setLines(); 
	
	glutInit(&argc, argv);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Create the window
	glutCreateWindow(WINDOW_TITLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable( GL_BLEND ); 
	glEnable( GL_POINT_SMOOTH ); 	
	glClearColor(0.6,0.6,0.8,0.0);
	
	// Set GLUT Callback Functions
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);

	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);

	glutMouseFunc(mouseClick);

	// Enter Main Loop
	glutMainLoop();

	return 0;
}

void keyboardDown(unsigned char key, int x, int y){
	KEYS[key] = true;
}

void keyboardUp(unsigned char key, int x, int y){
	KEYS[key] = false;
}

void mouseMove(int x, int y){
	MOUSE_CURRENT_X = x;
	MOUSE_CURRENT_Y = y;
}

void mouseClick(int botton, int state, int x, int y){
	if (botton == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			leftButton = true;
		}
	if (state == GLUT_UP){
			leftButton = false;
		}	
	}
}

void preprocessEvents(){
    
	// Update Globals
	CURRENT_TIME = ((float)glutGet(GLUT_ELAPSED_TIME)) / 1000;
	DELTA_TIME = CURRENT_TIME - LAST_TIME;
	LAST_TIME = CURRENT_TIME;

	MOUSE_DELTA_X = MOUSE_CURRENT_X - MOUSE_LAST_X;
	MOUSE_DELTA_Y = MOUSE_CURRENT_Y - MOUSE_LAST_Y;

	MOUSE_LAST_X = MOUSE_CURRENT_X;
	MOUSE_LAST_Y = MOUSE_CURRENT_Y;

	// Process Keys for Motion
	if (KEYS['w']){
		CAMERA_POSITION.z += WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['s']){
		CAMERA_POSITION.z -= WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['a']){
		CAMERA_POSITION.x -= WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['d']){
		CAMERA_POSITION.x += WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['q']){
		CAMERA_POSITION.y += WALKING_SPEED*DELTA_TIME;
	}
	if (KEYS['e']){
		CAMERA_POSITION.y -= WALKING_SPEED*DELTA_TIME;
	}
   	if (KEYS['n']){
		graphTime += 0.001; 
		graph.stepGraphCoordinates(); 
	}	
   	if (KEYS['b']){
		graphTime += 0.001; 
		graph.stepGraphCoordinates(); 
	}	
	// Process Mouse and Highlight Keys
	// if no highlight keys are pressed, rotate box normally
	if (leftButton && ! KEYS['x'] && ! KEYS['y'] && ! KEYS['z']){
		CAMERA_ROTATION.y += (float)MOUSE_DELTA_X*MOUSE_SENSITIVITY;
		CAMERA_ROTATION.x += (float)MOUSE_DELTA_Y*MOUSE_SENSITIVITY;
	}
}

void reshape(int screenDisplayWidth, int screenDisplayHeight){
	if (screenDisplayHeight == 0){
		screenDisplayHeight = 1;
	}
	float aspectRatio = (float)screenDisplayWidth / (float)screenDisplayHeight;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // reset the identity matrix

	glViewport(0, 0, screenDisplayWidth, screenDisplayHeight);

	gluPerspective(45, aspectRatio, 0.001, 1000); // zNear something small/close zFar something big/Far
	glMatrixMode(GL_MODELVIEW);
}

void display(){
	preprocessEvents();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(CAMERA_POSITION.x, CAMERA_POSITION.y, CAMERA_POSITION.z);
	glRotatef(CAMERA_ROTATION.x, 1, 0, 0);
	glRotatef(CAMERA_ROTATION.z, 0, 0, 1);
	glRotatef(CAMERA_ROTATION.y, 0, 1, 0);
	
	DisplayBox onScreenDisplayBox;
	onScreenDisplayBox.setUnitRange(1.0);
	onScreenDisplayBox.drawDisplayBox();

/*
	Node firstnode(0.0); 
	Node secondnode(0.5); 
	Node thirdnode(0.1); 	
	Graph graph; 
	graph.addNode(firstnode); 
	graph.addNode(secondnode); 
	graph.addNode(thirdnode); 	
	graph.drawGraph(); 
*/	
	
	graph.drawGraph(); 
	
	glutSwapBuffers();
}

void loadGraphFromDatafile(Graph &graph, string datafilename){
	fstream inputfile; 
	inputfile.open(datafilename.c_str()); 
	if (inputfile){
		string line; 
		while(getline(inputfile,line)){
			// get each line. 
			// convert line contents to floats; 
			string firstNumber; 
			bool secondOn = false; 	
			string secondNumber; 
			for (int i = 0; i < line.length(); i++){
				if (secondOn){
					secondNumber += line[i]; 
				}	
				if (line[i] == ','){
					secondOn = true; 	
				}
				if (!secondOn){
					firstNumber += line[i]; 
				}	
			}	
			float xCoordinate = stof(firstNumber); 	
			float yCoordinate = stof(secondNumber); 	
			// create a new node with specific coordinate
			Node newNode(xCoordinate, yCoordinate); 		
			// append new node to graph
			graph.addNode(newNode); 	
		}
	}
}
