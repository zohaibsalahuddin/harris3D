/*
	Date Created: 27th December
	FILENAME : Edges.h
------------------------------------------------
 *
 *	This file contains definitions for functions 
 *	to manipulate the information of the Edges in
 *	in the OFF file.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#ifndef __Edges__
#define __Edges__

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

class Edges 
{
	public:
// Since our implementation is only for triangular faces

		int memberEdges[2];
		Edges()
		{

		}
		void addEdges(int vertno1, int vertno2)
		{
	
			memberEdges[0] = vertno1;
			memberEdges[1] = vertno2;

		} 

};


#endif
