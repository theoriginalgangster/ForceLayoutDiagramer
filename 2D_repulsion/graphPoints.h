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
public: 	
	Node(float,float); 	
	float xCoordinate(float); 
	float yCoordinate(float); 
};

Node::Node(float initialXCoordinate, float initialYCoordinate){
	_xCoordinate = initialXCoordinate; 
	_yCoordinate = initialYCoordinate; 
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
			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),0.0); 
		glEnd(); 			
		glPointSize(8); 	
		glBegin(GL_POINTS); 
			glColor4f(1.0,1.0,1.0,1.0); 
			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),0.0); 
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
		// find forces from all other points	
		for (int r = 0; r < nodeVector.size(); r++){
			// get distance between nodes; 
			float deltaX = nodeVector[i].xCoordinate() - nodeVector[r].xCoordinate();
			float deltaY = nodeVector[i].yCoordinate() - nodeVector[r].yCoordinate(); 
			float distance = sqrt(pow(deltaX,2)+pow(deltaY,2)); 
			float forceScalar = 1.0/distance; 
			float forceX = forceScalar*deltaX; 
			float forceY = forceScalar*deltaY; 	
			if (i != r){	
				forceTotalX += forceX; 
				forceTotalY += forceY;
			} 	
		}	
		// change point location based on sum of forces
		nodeVector[i].xCoordinate(nodeVector[i].xCoordinate()+forceTotalX*0.001); 					
		nodeVector[i].yCoordinate(nodeVector[i].yCoordinate()+forceTotalY*0.001); 					
	}
}	
