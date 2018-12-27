/*
	Date Created: 23rd November
	FILENAME : fileread.cpp
------------------------------------------------
 *
 *	This file contains implementation for functions 
 *	to open the input file containing the vertices
 *	and faces information
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#include <iostream>
#include <string>
#include <fstream>
#include "fileread.h"
#include "Faces.h"
#include "Vertices.h"
#include <set>
#include "pca.h"
#define NOTEST

// TODO: See if namespace is used in the professional set up
using namespace std;

/* 	Function : skipline
-------------------------------------------------
*	This function is taken from the demo C++ code that
*	inorder to skip the lines.
*
*	Parameters:
*	in : istream & : It skills the line of the given input
	     file stream
*
*/
int Faces::numFaces =0;
int Vertices::numVertices =0;
void skipline(istream &in)
{
	char c;
	while(in>>noskipws>>c && c!='\n');
	in>>skipws;
}


/*
 *	Function : ReadVertices
-------------------------------------------------
 *	This function opens the file with the given 
 *	absolute path. 
 * 
 *	Parameters:
 *	absPath : String : The path to the file.
 *	ptrVertices: Vertices *& : This contains
 *		  the vertices information that is read
 *		  from the file.
 *	ptrFaces: Faces *& : This contains
 *		  the face information that is read
 *		  from the file.
 *	Return  : int : Return -1 in case of failure
*/

int readVertFace (const string filePath , Faces *& ptrFaces, Vertices *& ptrVertices)
{

	int total_vertices =0;
	int total_Vertices; // Total Number of Vertices
	int total_Faces; // Total Number of Faces
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


	file>>total_Vertices>>total_Faces;
	skipline(file); // moving to the next file
	
	// instantiating total number of faces and vertices
	ptrFaces = new Faces[total_Faces];
	ptrVertices = new Vertices[total_Vertices];

	// Displaying the Vertices
	cout << "DISPLAYING THE VERTICES" << endl;
	for(int i = 0; i < total_Vertices; i++)
	{
		file>>vertx>>verty>>vertz;
		//cout << vertx << " " << verty << " " << vertz << endl;
		ptrVertices[i].setValues(vertx,verty,vertz);
		ptrVertices[i].index = i;
		skipline(file);
	}
	

	cout << "DISPLAYING THE FACES" << endl;
	// Displaying the Faces
	for(int i = 0; i < total_Faces; i++)
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
		
		//adding information in the respective face.
		ptrFaces[i].addFaceInfo(memface1,memface2,memface3);
		ptrVertices[memface1].addFace(i);
		ptrVertices[memface2].addFace(i);
		ptrVertices[memface3].addFace(i);

		ptrVertices[memface1].addAdjacentVert(memface2,memface3);
		ptrVertices[memface2].addAdjacentVert(memface1,memface3);
		ptrVertices[memface3].addAdjacentVert(memface1,memface2);
		//adding faces in the respective 
		skipline(file);
	}
	

	file.close();	
	return 0;

} 

int cal_interest_points(double ** & result, int & size_result, string filename, double harris_parameter, double fraction, int radius_param, string selection_type)
{

	Faces * ptrfaces;
	Vertices * ptrvertices;
	vector <Vertices> nVert;
	int totalfaces =0;
	int totalVertices =0;
	double k = harris_parameter;

	if (selection_type == "fraction")
	{
		cout << "Selecting Points by Fraction" << endl;
	}
	else if (selection_type == "fraction")
	{
		cout << "Selecting points by Clustering" << endl;
	}
		

	cout << "Calling the Function to read the Vertex and Faces" << endl;
	int ret = readVertFace(filename,ptrfaces,ptrvertices) ;
	if (ret != 0)
	{
		cout << "File Not Supported. Aborting" << endl;
		return -1;
	}

	totalfaces = Faces::numFaces;
	totalVertices =Vertices::numVertices ;

	set <int> neighbor;
	for (int i =0 ; i < totalVertices; i++)
	{
		//cout << "Vertex: "<< ptrvertices[i].vertx << " " << ptrvertices[i].verty << " " << ptrvertices[i].vertz<< endl;
		ptrvertices[i].getRingNeighborhood(radius_param,ptrvertices,neighbor);
		set<int> :: iterator it;
		int i1 =0;
		int vertex_index =0;
		for (it = neighbor.begin(); it!= neighbor.end(); it++)
		{	
			if ((*it) == ptrvertices[i].index)
			{
				vertex_index = i1;
			}
			i1++;
			//cout << ptrvertices[*it].vertx << " " << ptrvertices[*it].verty << " " << ptrvertices[*it].vertz<< endl;
			nVert.push_back(ptrvertices[*it]);
		}


		
		double centerx,centery,centerz;
		double  p1,p2,p3,p4,p5,p6;
		double response =0;
		if (nVert.size() > 6)
		{
			calculate_center (nVert,centerx,centery,centerz);
			shift_center_to_zero (nVert, centerx, centery,centerz );
	
			MatrixXd * eigen_vectors = pca_calculate(nVert);

			direction_check_shift (nVert, eigen_vectors, vertex_index, centerx,centery,centerz);
 			pca_rotate (nVert, eigen_vectors);

			shift_to_vertex_centerxy(nVert, vertex_index);
		
			quadratic_fit (nVert, p1, p2, p3, p4 , p5 ,p6);
			response = get_harris_response (p1, p2, p3, p4 , p5 ,p6,k);
			//cout << "RESPONSE: "<< response << endl;
			ptrvertices[i].harris_response = response;
		}
		else
		{
			ptrvertices[i].harris_response = DBL_MIN;
		}
		
		nVert.clear();	
		neighbor.clear();
		
	}
		vector <Vertices> interest_points_all;
		int flag;
		for (int i =0 ; i < totalVertices; i++)
		{
			int flag = (ptrvertices[i].isMaximum(ptrvertices));

			if (flag == 0)
			{
				interest_points_all.push_back(ptrvertices[i]);
			}
		
		}

		sort(interest_points_all.begin(), interest_points_all.end(), response_compare);

		vector<Vertices> :: iterator itv;
		for(itv = interest_points_all.begin(); itv!=interest_points_all.end(); itv++)
		{
				//cout << (*itv).vertx << "  "<< (*itv).verty  << "  "<< (*itv).vertz << " Response: " << (*itv).harris_response  << endl;
				
				
		}
		int points_fraction = fraction * totalVertices;
		
		cout << "size of points: " << interest_points_all.size() << ", Fraction: " << points_fraction << endl;
		//cout << "Interest Points: " << endl;


		size_result = points_fraction;
		result = new double*[points_fraction];
		for (int i =0 ; i < points_fraction ; i++)
		{
			result[i] = new double[3];
						
		}

		for (int i = 0 ; ((i < points_fraction) & (i < interest_points_all.size()));i++)
		{
			cout << interest_points_all[i].vertx << " "<< interest_points_all[i].verty << " "<< interest_points_all[i].vertz << endl;
			result[i][0] = interest_points_all[i].vertx;
			result[i][1] = interest_points_all[i].verty;
			result[i][2] = interest_points_all[i].vertz;
		}
		

		neighbor.clear();
		interest_points_all.clear();
		delete[] ptrfaces;
		delete[] ptrvertices;
		nVert.clear();
 		Faces::numFaces =0;
 		Vertices::numVertices =0;
		return 0;


}

#ifdef TEST
int main ( void )
{
	cout << "Calling the Function to read the Vertex and Faces" << endl;
	readVertFace("/home/zohaib123/Desktop/Harris3D/harris3D/src/cone.off",0,0);
	return 0;
}
 
#endif



