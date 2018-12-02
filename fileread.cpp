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
#include "Faces.h"
#include "Vertices.h"
#include <set>
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

#ifdef TEST
int main ( void )
{
    cout << "Calling the Function to read the Vertex and Faces" << endl;
    readVertFace("/home/zohaib123/Desktop/Harris3D/harris3D/src/cone.off",0,0);
    return 0;
}

#endif



