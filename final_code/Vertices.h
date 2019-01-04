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
#include <math.h>
#include <map>




class Vertices
{
	public:
		Vertices();
		int marked;
		int index;
		double harris_response;
		set <int> facesIncVert;
		set <int> adjacentVert;
		double vertx,verty,vertz;

		void setValues ( double x , double y , double z);
		void addFace(int face);
		void addAdjacentVert(int vert1,int vert2);


		void getRingNeighborhood (int level_depth, Vertices * & vertex_in , set <int> & neighborhood,int size);
		void getAdaptiveNeighborhood (double radius, Vertices * & vertex_in , set <int> & neighborhood, int size);
		int isMaximum(Vertices * & vertex_in);
		~Vertices();
};
 
float get_cluster_diag ( Vertices * & vertex_in , int total_num);

		


#endif
