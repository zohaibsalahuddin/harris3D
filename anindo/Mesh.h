#ifndef __MESH_H_
#define __MESH_H_

#include "Vertex.h"
#include "Face.h"
#include <vector>

using namespace std;

class Mesh{

      //Topological information
      Vertex* vertices;
      Face* faces;
	  
	  int numVertices;
      int numFaces;
	  
	  float** D;
      
      //BBox Information
      double xmin, xmax, ymin, ymax, zmin, zmax;
      double diag;


	  public:
	  		void cleanMesh();
      
             Mesh();
             Mesh(const char* nombreArchivo);
             ~Mesh();

             void loadFromFile(const char* filename);
             
             friend ostream& operator<<(ostream& os, Mesh &obj);
			 Vertex* getVertices(){return vertices;}
			 Face* getFaces(){return faces;}
			 
			 int getNumberVertices(){return numVertices;}
			 int getNumFaces(){return numFaces;}
			 
			 double getDiagonal(){return diag;}
			 			 
};
#endif
