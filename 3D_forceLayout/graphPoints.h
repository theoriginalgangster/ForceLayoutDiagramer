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
	int _id; 
public: 	
	Node(float,float,float); 	
	float xCoordinate(float); 
	float yCoordinate(float); 
	float zCoordinate(float); 	
	int id(int); 
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

int Node::id(int idIN = -1){
	if (idIN != -1){
		_id = idIN;  
	}
	else{
		return _id; 
	}
}

// -----------------------------ADJCENCY MATRIX--------------------

class AdjMtrx{
private: 
        int nodeCount = 0;
        vector<vector<int> > _matrix;
public:
        void addNode();
        void addEdge(int, int);
        bool hasEdge(int, int);
        void print();
};

void AdjMtrx::addNode(){
        vector<int> newRow;
        nodeCount++;
        for (int i = 0; i < nodeCount; i++){
                newRow.push_back(0);
        }
        _matrix.push_back(newRow);
        for (int i = 0; i < nodeCount; i++){
                _matrix[i].push_back(0);
        }
}

void AdjMtrx::print(){
        for (int i = 0; i < nodeCount; i++){
                cout << "[ ";
                for (int j = 0; j < nodeCount; j++){
                        cout << _matrix[i][j] << " ";
                }
                cout << "]" << endl;
        }
}

void AdjMtrx::addEdge(int row, int column){
        _matrix[row][column] = 1;
}

bool  AdjMtrx::hasEdge(int row, int column){
        if (_matrix[row][column] == 1){
                return true;
        }
        else {
                return false;
        }
}

// -----------------------------GRAPH------------------------------
class Graph{
private: 
	vector<Node> nodeVector; 
	vector<Node> trajLine; 	
	AdjMtrx blackEdges; 
public: 	
	Graph(); 	
	void addNode(Node); 	
	void setLines(); 
	void drawGraph(); 
	void stepGraphCoordinates(float); 
}; 

Graph::Graph(){
	// idk
}

void Graph::addNode(Node nodeToAdd){
	nodeVector.push_back(nodeToAdd); 
	blackEdges.addNode(); 	
} 

void Graph::setLines(){
	// set all 0 id
	for (int i = 0; i < nodeVector.size(); i++){
		nodeVector[i].id(0); 
	}	
	for (int i = 0; i < nodeVector.size(); i++){
		for (int j = 0; j < 10; j++){
			blackEdges.addEdge(i,j); 
			nodeVector[j].id(1); 	
		}		
	}
}

void Graph::drawGraph(){
	/*	
	// LINES	
	for (int i = 0; i < nodeVector.size(); i++){	
		for (int j = 0; j < nodeVector.size(); j++){
			if (greenEdges.hasEdge(i,j)){	
				glBegin(GL_LINES); 
				glColor4f(0.0,1.0,0.0,0.1); 
				glVertex3f(nodeVector[i].xCoordinate(), nodeVector[i].yCoordinate(),0.0); 
				glVertex3f(nodeVector[j].xCoordinate(), nodeVector[j].yCoordinate(),0.0); 
				glEnd(); 
			}
		}
	}
	*/	
	// DRAW EDGES	
	for (int i = 0; i < nodeVector.size(); i++){	
		for (int j = 0; j < nodeVector.size(); j++){
			if (blackEdges.hasEdge(i,j)){	
				glBegin(GL_LINES); 
				glColor4f(0.0,0.0,0.0,0.1); 
				glVertex3f(nodeVector[i].xCoordinate(), nodeVector[i].yCoordinate(),nodeVector[i].zCoordinate()); 
				glVertex3f(nodeVector[j].xCoordinate(), nodeVector[j].yCoordinate(),nodeVector[j].zCoordinate()); 
				glEnd(); 
			}
		}
	}
	// DRAW NODES 
	for (int i = 0; i < nodeVector.size(); i++){
		glPointSize(6); 	
		glBegin(GL_POINTS); 
			//glColor4f((float)i/(float)nodeVector.size(),0.0,0.0,1.0); 
			glColor4f(0.0,0.0,0.0,1.0); 
//			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),0.0); 
			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),nodeVector[i].zCoordinate()); 
		glEnd(); 			
		glPointSize(4); 	
		glBegin(GL_POINTS); 
			if (nodeVector[i].id() == 1){
				glColor4f(0.0,0.0,1.0,1.0); 
			}	
			else {
				glColor4f(1.0,1.0,1.0,1.0); 
			}	
//			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),0.0); 
			glVertex3f(nodeVector[i].xCoordinate(),nodeVector[i].yCoordinate(),nodeVector[i].zCoordinate()); 
		glEnd(); 	
	}
	// DRAW TRAJECTORY
	for (int i = 0; i < trajLine.size(); i++){
		if (i != trajLine.size()-1){
			glBegin(GL_LINES); 
			glColor4f(0.38,1.0,0.0,1.0); 
			glVertex3f(trajLine[i].xCoordinate(), trajLine[i].yCoordinate(), trajLine[i].zCoordinate()); 
			glVertex3f(trajLine[i+1].xCoordinate(), trajLine[i+1].yCoordinate(), trajLine[i+1].zCoordinate()); 
			glEnd(); 	
		}
	}	
}

void Graph::stepGraphCoordinates(float steppingRate = 0.0002){
	float forceXSum = 0; 
	float forceYSum = 0; 	
	float forceZSum = 0; 	
	// change the graph coordinates
	for (int i = 0; i < nodeVector.size(); i++){
		// get sum of forces on object for every frame
		float forceTotalX = 0; 	
		float forceTotalY = 0; 	
		float forceTotalZ = 0; 	
		// CREATE ALL VERTEX REPULSION FORCES	
		for (int r = 0; r < nodeVector.size(); r++){
			// get distance between nodes; 
			float deltaX = nodeVector[i].xCoordinate() - nodeVector[r].xCoordinate();
			float deltaY = nodeVector[i].yCoordinate() - nodeVector[r].yCoordinate(); 
			float deltaZ = nodeVector[i].zCoordinate() - nodeVector[r].zCoordinate(); 	
			float distance = sqrt(pow(deltaX,2)+pow(deltaY,2)+pow(deltaZ,2)); 
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
		// CREATE EDGE ATTRACTIVE FORCES	
		// blackEdges
		for (int r = 0; r < nodeVector.size(); r++){
			// check for connection between each node
			if (blackEdges.hasEdge(i,r)){
				// find the force between those nodes
				float deltaX = nodeVector[i].xCoordinate() - nodeVector[r].xCoordinate();
				float deltaY = nodeVector[i].yCoordinate() - nodeVector[r].yCoordinate(); 
				float deltaZ = nodeVector[i].zCoordinate() - nodeVector[r].zCoordinate(); 	
				float distance = sqrt(pow(deltaX,2)+pow(deltaY,2)+pow(deltaZ,2)); 
				float forceScalar = -1.0/(distance+0.5); 
				float forceX = forceScalar*deltaX; 
				float forceY = forceScalar*deltaY; 	
				float forceZ = forceScalar*deltaZ;  	
				// multiply the forces by some constant
				forceX *= 100.0; 
				forceY *= 100.0; 	
				forceZ *= 100.0; 	
				// add it to the total force	
				if (i != r){	
					forceTotalX += forceX; 
					forceTotalY += forceY;
					forceTotalZ += forceZ; 
				} 	
			}
		}	
		for (int r = 0; r < nodeVector.size(); r++){
			if (nodeVector[i].id() == 1 && nodeVector[r].id() == 1){
				// find the force between those nodes
				float deltaX = nodeVector[i].xCoordinate() - nodeVector[r].xCoordinate();
				float deltaY = nodeVector[i].yCoordinate() - nodeVector[r].yCoordinate(); 
				float deltaZ = nodeVector[i].zCoordinate() - nodeVector[r].zCoordinate(); 
				float distance = sqrt(pow(deltaX,2)+pow(deltaY,2)+pow(deltaZ,2)); 
				float forceScalar = -1.0/(distance+0.5); 
				float forceX = forceScalar*deltaX; 
				float forceY = forceScalar*deltaY; 	
				float forceZ = forceScalar*deltaZ; 	
				// multiply the forces by some constant
				forceX *= 20.0; 
				forceY *= 20.0; 	
				forceZ *= 20.0; 	
				// add it to the total force	
				if (i != r){	
					forceTotalX += forceX; 
					forceTotalY += forceY;
					forceTotalZ += forceZ;
				} 	
			}	
		}		
	
		// change point location based on sum of forces
		nodeVector[i].xCoordinate(nodeVector[i].xCoordinate()+forceTotalX*steppingRate); 					
		nodeVector[i].yCoordinate(nodeVector[i].yCoordinate()+forceTotalY*steppingRate); 					
		nodeVector[i].zCoordinate(nodeVector[i].zCoordinate()+forceTotalZ*steppingRate); 					
		// ADJUST FOR SUM	
		forceXSum += forceTotalX; 	
		forceYSum += forceTotalY; 
		forceZSum += forceTotalZ; 	
	}
	// COMPENSATE FOR TOTAL MOVEMENT	
	// get average move of each and compensate
	float deltaXAverage = forceXSum / nodeVector.size(); 
	float deltaYAverage = forceYSum / nodeVector.size(); 	
	float deltaZAverage = forceZSum / nodeVector.size(); 	
	for (int i = 0; i < nodeVector.size(); i++){
		// change points according to sum so it stays in the center	
		nodeVector[i].xCoordinate(nodeVector[i].xCoordinate()-deltaXAverage*steppingRate); 					
		nodeVector[i].yCoordinate(nodeVector[i].yCoordinate()-deltaYAverage*steppingRate); 					
		nodeVector[i].zCoordinate(nodeVector[i].zCoordinate()-deltaZAverage*steppingRate); 					
	}
	// ADD TRAJECTORY NODE
	/*	
	// based on average blue nodes every time
	float blueSumX = 0; 
	float blueSumY = 0; 
	float blueSumZ = 0; 
	int blueCount = 0; 	
	for (int i = 0; i < nodeVector.size(); i++){
		if (nodeVector[i].id() == 1){
			blueSumX += nodeVector[i].xCoordinate(); 
			blueSumY += nodeVector[i].yCoordinate(); 
			blueSumZ += nodeVector[i].zCoordinate(); 
			blueCount++; 	
		}
	}
	float blueAverageX = blueSumX/(float)blueCount; 
	float blueAverageY = blueSumY/(float)blueCount; 
	float blueAverageZ = blueSumZ/(float)blueCount;
	// add node to the trajector object at this time
	Node trajNode(blueAverageX, blueAverageY, blueAverageZ);
	trajLine.push_back(trajNode); 
	*/
}
