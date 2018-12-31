#include <map>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <cassert>
#include <cfloat>
#include "Mesh.h"
#include "util.h"
#include "Clock.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

using namespace std;

Mesh::Mesh(){
       vertices = NULL;
       faces = NULL;
       numVertices = 0;
       numFaces = 0;
}

Mesh::Mesh(const char* filename)
{
	   vertices = NULL;
	   faces = NULL;
	   numVertices = 0;
	   numFaces = 0;
	   
	   xmin = xmax = ymin = ymax = zmin = zmax = 0.0;

	   loadFromFile(filename);
       
       //Finding bounding box...
       for(register int i = 0; i < getNumberVertices(); i++){
       		if(vertices[i].getX() < xmin)
       			xmin = vertices[i].getX();
       		else if(vertices[i].getX() > xmax)
       			xmax = vertices[i].getX();
       		
       		if(vertices[i].getY() < ymin)
       			ymin = vertices[i].getY();
       		else if(vertices[i].getY() > ymax)
       			ymax = vertices[i].getY();
       		
       		if(vertices[i].getZ() < zmin)
       			zmin = vertices[i].getZ();
       		else if(vertices[i].getZ() > zmax)
       			zmax = vertices[i].getZ();
       }
       
       diag = sqrt((xmax - xmin)*(xmax - xmin) + (ymax - ymin)*(ymax - ymin) + (zmax - zmin)*(zmax - zmin));
}

Mesh::~Mesh(){
    	cleanMesh();
}

//Clean up the object
void Mesh::cleanMesh(){
	if(faces){
	           delete[] faces;
			   faces = NULL;
	     }
	if(vertices){
	           delete[] vertices;
			   vertices = NULL;
	     }
}


void Mesh::loadFromFile(const char* filename){

	int numEdges;
		
	ifstream in(filename);
	
	string format;
	getline(in, format);
	assert(format.find("OFF") != string::npos);
	
	in>>numVertices>>numFaces>>numEdges;
	skipline(in);
	
	vertices = new Vertex[numVertices];
	faces = new Face[numFaces];
	
	
	for(register int i = 0; i < numVertices; i++){
		double x, y, z;
		in>>x>>y>>z;
		skipline(in);
		
		vertices[i].setX(x);	vertices[i].setY(y);	vertices[i].setZ(z);
		vertices[i].setIndex(i);
	}
	
	
	for(register int i = 0;  i < numFaces; i++){
		int numVert;
		in >> numVert;
		
		assert(numVert == 3);
		
		int p1, p2, p3;
			
		in>>p1>>p2>>p3;
		skipline(in);
		
		faces[i].addVertex(p1);				faces[i].addVertex(p2);				faces[i].addVertex(p3);
		
		vertices[p1].addFace(i);			vertices[p2].addFace(i);			vertices[p3].addFace(i);
		
		vertices[p1].addVertex(p2);	vertices[p1].addVertex(p3);
		vertices[p2].addVertex(p1);	vertices[p2].addVertex(p3);
		vertices[p3].addVertex(p1);	vertices[p3].addVertex(p2);
	}
	
	in.close();
	
}

ostream& operator<<(ostream& os, Mesh& obj){
         os<<"OFF"<<endl;
         os<<obj.getNumberVertices()<<" "<<obj.getNumFaces()<<" "<<0<<endl;
         for(int i = 0; i<obj.getNumberVertices(); i++){
             os<<obj.vertices[i].getX()<<" "<<obj.vertices[i].getY()<<" "<<obj.vertices[i].getZ()<<endl;
         }
		 for(int i = 0; i < obj.getNumFaces(); i++){
				os<<3<<" ";
				vector<int> vert = obj.faces[i].getVertices();
				for(int j = 0; j < (vert.size() - 1); j++){
					os<<vert[j]<<" ";
				}
				os<<vert[vert.size()-1]<<endl;
		 }
		 return os;
 }
