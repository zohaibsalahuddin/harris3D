/*
	Date Created: 23rd November
	FILENAME : fileread.cpp
------------------------------------------------
 *
 *	This file contains implementation for functions 
 *	to open the input file containing the vertices
 *	and faces information
*/

#include <iostream>
#include <string>
#include <fstream>
#define TEST

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

void skipline(istream &in){
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
 *	vertexinfo: (TODO) DataStructure : This contains
 *		  the vertices information that is read
 *		  from the file.
 *	faceinfo: (TODO) DataStructure : This contains
 *		  the face information that is read
 *		  from the file.
 *	Return  : int : Return -1 in case of failure
*/

int readVertFace (const string filePath , int dummy_vertex , int dummy_face)
{
	int total_vertices =0;
	int total_Vertices; // Total Number of Vertices
	int total_Faces; // Total Number of Faces
	double vertx, verty, vertz;
	int numface =0; // stores the number of faces in the file.
	int face1, face2,face3;
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


	// Displaying the Vertices
	cout << "DISPLAYING THE VERTICES" << endl;
	for(int i = 0; i < total_Vertices; i++)
	{
		file>>vertx>>verty>>vertz;
		cout << vertx << " " << verty << " " << vertz << endl;
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
		file>>face1>>face2>>face3;
		cout << face1 << " " << face2 << " " << face3 << endl;
		skipline(file);
	}
	

	file.close();	
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



