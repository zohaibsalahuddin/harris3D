/*
	Date Created: 23rd November
	FILENAME : mesh.cpp
------------------------------------------------
 *
 *	This file contains implementation for functions 
 *	to construct the mesh.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#include <iostream>
#include <string>
#include <fstream>
#include "mesh.h"
#include "Faces.h"
#include "Vertices.h"
#include <set>
#include "pca.h"
#include <math.h>
#include "Edges.h"
#define NOTEST

// TODO: See if namespace is used in the professional set up
using namespace std;

/*
 *	Constructor to Mesh Class
-------------------------------------------------
 *	This is a constructor to Mesh
 *	Class
 * 
 *	Parameters:
 *	None
 *	Return  : None
*/

Mesh::Mesh()
{

	totalVertices = 0;
	totalFaces = 0;
	totalEdges = 0;
}

/*
 *	Function : ReadVertices
-------------------------------------------------
 *	This function opens the file with the given 
 *	absolute path. 
 * 
 *	Parameters:
 *	absPath : String : The path to the file.
 *	Return  : int : Return -1 in case of failure
*/

int Mesh::readVertFace (const string filePath)
{

	double vertx, verty, vertz;
	int numface =0; // stores the number of faces in the file.
	int memface1, memface2,memface3;
	string check_off;
	ifstream file;
	

	const char * path= filePath.c_str();	
	file.open(path);// opening the file



	// checking and skipping the line containing the OFF 
	getline(file,check_off);
	if (check_off.find("OFF") == string::npos)
	{
		cout << "This is not an OFF file as it does not"
			"contain OFF in the first line" << endl;
		cout << "Aborting" << endl;
		return -1;	
	} 


	file>>totalVertices>>totalFaces>>totalEdges;
	skipline(file); // moving to the next file
	
	// instantiating total number of faces and vertices
	ptrfaces = new Faces[totalFaces];
	ptrvertices = new Vertices[totalVertices];
	ptredges = new Edges[totalEdges];


	// Displaying the Vertices
	cout << "DISPLAYING THE VERTICES" << endl;
	for(int i = 0; i < totalVertices; i++)
	{
		file>>vertx>>verty>>vertz;
		//cout << vertx << " " << verty << " " << vertz << endl;
		ptrvertices[i].setValues(vertx,verty,vertz);
		ptrvertices[i].index = i;
		skipline(file);
	}
	

	cout << "DISPLAYING THE FACES" << endl;
	// Displaying the Faces
	for(int i = 0; i < totalFaces; i++)
	{		
		// This contains implementation of only triangular faces
		file >> numface;		
		if (numface !=3)
		{
			cout << "This contains only implementation for"
				"triangular faces.ABORT!" << endl;
			return -1;
		}
		file>>memface1>>memface2>>memface3;
		//cout << memface1 << " " << memface2 << " " << memface3 << endl;
		
		//adding edges information
		ptredges[3*i].addEdges(memface1,memface2);
		ptredges[(3*i)+1].addEdges(memface2,memface3);
		ptredges[(3*i)+2].addEdges(memface3,memface1);
	
		//adding information in the respective face.
		ptrfaces[i].addFaceInfo(memface1,memface2,memface3);
		ptrvertices[memface1].addFace(i);
		ptrvertices[memface2].addFace(i);
		ptrvertices[memface3].addFace(i);

		ptrvertices[memface1].addAdjacentVert(memface2,memface3);
		ptrvertices[memface2].addAdjacentVert(memface1,memface3);
		ptrvertices[memface3].addAdjacentVert(memface1,memface2);
		//adding faces in the respective 
		skipline(file);
	}
	

	file.close();	
	return 0;

} 

/* 	Function : skipline
-------------------------------------------------
*	This function is taken from the demo C++ code that
*	inorder to skip the lines.
*
*	Parameters:
*	in : istream & : It skills the line of the given input
*	     file stream
*
*/

void skipline(istream &in)
{
	char c;
	while(in>>noskipws>>c && c!='\n');
	in>>skipws;
}

/* 	Function : deleteMesh()
-------------------------------------------------
*	This function is used to delete all the dynamic allocations
*	in the Mesh Class.
*	
*/

void Mesh::deleteMesh ()
{

	delete[] ptrfaces;
	delete[] ptrvertices;
	delete[] ptredges;

}

/* FUNCTION: get_cluster_diag
 * --------------------------
 * calculate the diagonal of the object by creating a bounding box around it and measuring its diagonal
 * Inputs:
 * - Vertices * &: a pointer to the object Verticies that include all the vertices in the file
 * - int: the number of total vertices
 *
 * Output: returns a float: value of the object diagonal
 */

float get_cluster_diag ( Vertices * & vertex_in , int total_num)
{
	float x_min=0,x_max=0,y_min=0,y_max=0,z_min=0,z_max=0;
	float x,y,z;
	float diagonal=0;

	for (int i =0; i < total_num ; i++)
	{
		x = vertex_in[i].vertx;
		y = vertex_in[i].verty;
		z = vertex_in[i].vertz;

		if (x < x_min)
		{
			x_min = x;
		}

		if ( x > x_max)
		{
			x_max = x;
		}

		if ( y < y_min)
		{
			y_min =y;
		}

		if ( y > y_max)
		{
			y_max = y;
		}

		if ( z < z_min)
		{
			z_min = z;
		}

		if ( z > z_max)
		{
			z_max = z;
		}

	}

		 diagonal = sqrt((x_max - x_min)*(x_max - x_min) + (y_max - y_min)*(y_max - y_min) + (z_max - z_min)*(z_max - z_min));
		return diagonal;
}

using namespace std;

/*
 *	Destructor to Mesh Class
-------------------------------------------------
 *	This is a destructor to Mesh
 *	Class
 * 
 *	Parameters:
 *	None
 *	Return  : None
*/

Mesh::~Mesh()
{

}


// The following code was deployed for initial testing

#ifdef TEST
int main ( void )
{
	cout << "Calling the Function to read the Vertex and Faces" << endl;
	readVertFace("/home/zohaib123/Desktop/Harris3D/harris3D/src/cone.off",0,0);
	return 0;
}
 
#endif



