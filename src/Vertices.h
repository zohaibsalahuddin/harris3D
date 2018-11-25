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
		int marked=0;
		int index=-1;
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

		void getRingNeighborhood (int level_depth, Vertices * & vertex_in , set <int> & neighborhood)
		{
			set<int>  temp_vert;	
			cout << "In Neighborhood Function" << endl;	
			for (int i =0 ; i <= level_depth; i++)
			{	
				if ( i ==0)
				{
					temp_vert.insert(adjacentVert.begin(),adjacentVert.end());
					neighborhood.insert(adjacentVert.begin(),adjacentVert.end());
					marked = 1;
					neighborhood.insert(index);
				}
				else
				{	cout << "Point" << endl;
					set<int> :: iterator it;
					for(it = temp_vert.begin(); it!=temp_vert.end(); )
					{	cout << "Investigating Vertex No. "  << *it << endl;

						set<int> :: iterator current = it++;
						cout << "Marked Status" << vertex_in[*current].marked  << endl;
						if (vertex_in[*current].marked == 0)
						{
							temp_vert.insert(vertex_in[*current].adjacentVert.begin(),
									 vertex_in[*current].adjacentVert.end());
							neighborhood.insert(vertex_in[*current].adjacentVert.begin(),
									 vertex_in[*current].adjacentVert.end());
						    	vertex_in[*current].marked = 1;
						}
						temp_vert.erase(current);
					}					
				}
			}
			
			for (int i =0 ; i < Vertices::numVertices; i++)
			{
				vertex_in[i].marked =0;			
			}
					      
		}
};
 

#endif
