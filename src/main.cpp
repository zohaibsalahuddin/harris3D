/*
	Date Created: 24thNovember
	FILENAME : main.cpp
------------------------------------------------
 *
 *	This file contains implementation for the
 *	main file testing the functionality of the
 *	overall program.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#include <iostream>
#include <vector>
#include "Faces.h"
#include "Vertices.h"
#include "fileread.h"

using namespace std;

int Faces::numFaces =0;
int Vertices::numVertices =0;

int main(void)
{
	
	Faces * ptrfaces;
	Vertices * ptrvertices;
	vector <Vertices> nVert;
	int totalfaces =0;
	int totalVertices =0;

	cout << "Calling the Function to read the Vertex and Faces" << endl;
	readVertFace("/home/zohaib123/Desktop/Harris3D/harris3D/src/epcot.off",ptrfaces,ptrvertices);
	totalfaces = Faces::numFaces;
	totalVertices =Vertices::numVertices ;
	cout << "Printing the Face Information:" << endl;
	cout << "Total Number of Faces:" << totalfaces << endl;
	for (int i =0 ; i < totalfaces; i++)
	{
		cout << ptrfaces[i].memberVerts[0] << " " << ptrfaces[i].memberVerts[1] << " " << ptrfaces[i].memberVerts[2]<< endl;;
	}

	cout << "Printing the Face Information:" << endl;
	cout << "Total Number of Vertices:" << totalVertices << endl;
	for (int i =0 ; i < totalVertices; i++)
	{
		cout << ptrvertices[i].vertx << " " << ptrvertices[i].verty << " " << ptrvertices[i].vertz<< endl;
		
		cout << "Adjacent Vertices for Vertex no. " << i << endl;
		set<int> :: iterator it;
		for(it = ptrvertices[i].adjacentVert.begin(); it!=ptrvertices[i].adjacentVert.end(); it++)
		{
				cout << *it << "  ";
				
		}
		cout << endl;
		
		cout << " Faces part of Vertex no. " << i << endl;

		for(it = ptrvertices[i].facesIncVert.begin(); it!=ptrvertices[i].facesIncVert.end(); it++)
		{
				cout << *it << "  ";
				
		}

		cout << endl;
	}
	set <int> neighbor;
	cout << "The Neighborhood of all the vertices are:" << endl;
	for (int i =0 ; i < totalVertices; i++)
	{

		ptrvertices[i].getRingNeighborhood(1,ptrvertices,neighbor);
		set<int> :: iterator it;
		cout << "Vertex Neighborhood: "<< ptrvertices[i].vertx << " " << ptrvertices[i].verty << " " << ptrvertices[i].vertz<< endl;
		for (it = neighbor.begin(); it!= neighbor.end(); it++)
		{	
			nVert.push_back(ptrvertices[*it]);
			cout << ptrvertices[*it].vertx << " " << ptrvertices[*it].verty << " " << ptrvertices[*it].vertz<< endl;
		}

		
		vector<Vertices> :: iterator itv;
		cout << " Neighborhood Vertices: " << endl;
		for (itv = nVert.begin(); itv!= nVert.end(); itv++)
		{	
			cout << (*itv).vertx << " " << (*itv).verty << " " << (*itv).vertz<< endl;
		}

		nVert.clear();	
		neighbor.clear();
		
	}	
	
	
	return 0;


}
