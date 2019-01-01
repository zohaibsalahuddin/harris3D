/*
	Date Created: 24th November
	FILENAME : Faces.h
------------------------------------------------
 *
 *	This file contains definitions for functions 
 *	to manipulate the information of the Faces in
 *	in the OFF file.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#ifndef __FACES__
#define __FACES__

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

class Faces 
{
	public:
// Since our implementation is only for triangular faces
		static int numFaces;
		int memberVerts[3];
		Faces()
		{
			numFaces++;
		}
		void addFaceInfo(int vertno1, int vertno2, int vertno3)
		{
	
			memberVerts[0] = vertno1;
			memberVerts[1] = vertno2;
			memberVerts[2] = vertno3;

		} 
	
};


#endif
