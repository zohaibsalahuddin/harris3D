/*
	Date Created: 23rd November
	FILENAME : mesh.h
------------------------------------------------
 *
 *	This file contains declarations for functions 
 *	to open the input file containing the vertices
 *	and faces information
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#ifndef __FILE_READ__
#define __FILE_READ__

#include "Faces.h"
#include "Vertices.h"
#include "pca.h"
#include <iostream>
#include <cfloat>
#include <vector>
#include "Faces.h"
#include "Vertices.h"
#include "Edges.h"


using namespace std;
class Mesh
{
	public:
		Faces * ptrfaces;
		Vertices * ptrvertices;
		Edges * ptredges;

		int totalVertices;
		int totalFaces;
		int totalEdges;

		Mesh();
		int readVertFace (const string filePath);
		void deleteMesh ();
		~Mesh();

};

void skipline(istream &in);
float get_cluster_diag ( Vertices * & vertex_in , int total_num);

#endif
