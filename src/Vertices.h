/*
	Date Created: 24th November
	FILENAME : Vertices.h
------------------------------------------------
 *
 *	This file contains definitions for functions 
 *	to manipulate the information of the Vertices in
 *	in the OFF file.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#ifndef __VERTICES__
#define __VERTICES__

#include <iostream>
#include <vector>
#include <set>
#include "Faces.h"

class Vertices
{
	public:
		static int numVertices;
		Vertices()
		{
			numVertices++;
		}
		set <int> facesIncVert;
		set <int> adjacentVert;
		double vertx,verty,vertz;
		void setValues ( double x , double y , double z)
		{
			vertx = x;
			verty = y;
			vertz = z;
		}
		void addFace(int face)
		{
			facesIncVert.insert(face);
		}
		void addAdjacentVert(int vert1,int vert2)
		{
			adjacentVert.insert(vert1);
			adjacentVert.insert(vert2);
		}
};


#endif
