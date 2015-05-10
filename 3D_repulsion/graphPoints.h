// graph points made from initially random points
// using force-directed graph drawing
#include <vector> 
#include <GL/freeglut.h> 
#include <GL/gl.h> 
#include <cmath> 
using namespace std; 

// -----------------------------NODE------------------------------
class Node{
private: 	
	float _xCoordinate; 
	float _yCoordinate; 
	float _zCoordinate; 
public: 	
	Node(float,float,float); 	
	float xCoordinate(float); 
	float yCoordinate(float); 
	float zCoordinate(float); 
};

Node::Node(float initialXCoordinate, float initialYCoordinate, float initialZCoordinate){
	_xCoordinate = initialXCoordinate; 
	_yCoordinate = initialYCoordinate; 
	_zCoordinate = initialZCoordinate; 
}

float Node::xCoordinate(float xCoordinateIN = -1000.0){
	if (xCoordinateIN != -1000.0){		
		_xCoordinate = xCoordinateIN; 	
	}
	else{
		return _xCoordinate; 
	}
}

float Node::yCoordinate(float yCoordinateIN = -1000.0){
	if (yCoordinateIN != -1000.0){		
		_yCoordinate = yCoordinateIN; 	
	}
	else{
		return _yCoordinate; 
	}
}

float Node::zCoordinate(float zCoordinateIN = -1000.0){
	if (zCoordinateIN != -1000.0){		
		_zCoordinate = zCoordinateIN; 	
	}
	else{
		return _zCoordinate; 
	}
}
// -----------------------------GRAPH------------------------------
class Graph{
private: 
	vector<Node> nodeVector; 
public: 	
	Graph(); 	
	void addNode(Node); 	
	void drawGraph(); 
	void stepGraphCoordinates(); 
}; 

Graph::Graph(){
	// idk
}

void Graph::addNode(Node nodeToAdd){
	nodeVector.push_back(nodeToAdd); 
} 

void Graph::drawGraph(){
	for (int i = 0; i < nodeVector.size(); i++){
		glPointSize(10); 	
		glBegin(GL_POINTS); 
			glColor4f(1.0,0.0,0.0,1.0); 
			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),nodeVector[i].zCoordinate()); 
		glEnd(); 			
		glPointSize(8); 	
		glBegin(GL_POINTS); 
			glColor4f(1.0,1.0,1.0,1.0); 
			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),nodeVector[i].zCoordinate()); 
		glEnd(); 			
	}
}

void Graph::stepGraphCoordinates(){
	// change the graph coordinates
	for (int i = 0; i < nodeVector.size(); i++){
		//nodeVector[i].xCoordinate(nodeVector[i].xCoordinate()+0.01); 
		// get sum of forces on object
		float forceTotalX = 0; 	
		float forceTotalY = 0; 
		float forceTotalZ = 0; 	
		// find forces from all other points	
		for (int r = 0; r < nodeVector.size(); r++){
			// get distance between nodes; 
			float deltaX = nodeVector[i].xCoordinate() - nodeVector[r].xCoordinate();
			float deltaY = nodeVector[i].yCoordinate() - nodeVector[r].yCoordinate(); 
			float deltaZ = nodeVector[i].zCoordinate() - nodeVector[r].zCoordinate();	
			float distance = sqrt(pow(deltaX,2)+pow(deltaY,2)+pow(deltaZ,2)); 
			// set a distance scalar	
			float forceScalar = 1.0/distance; 
			float forceX = forceScalar*deltaX; 
			float forceY = forceScalar*deltaY; 	
			float forceZ = forceScalar*deltaZ; 	
			if (i != r){	
				forceTotalX += forceX; 
				forceTotalY += forceY;
				forceTotalZ += forceZ; 	
			} 	
		}	
		// change point location based on sum of forces
		nodeVector[i].xCoordinate(nodeVector[i].xCoordinate()+forceTotalX*0.001); 					
		nodeVector[i].yCoordinate(nodeVector[i].yCoordinate()+forceTotalY*0.001); 					
		nodeVector[i].zCoordinate(nodeVector[i].zCoordinate()+forceTotalZ*0.001); 					
	}
}	
