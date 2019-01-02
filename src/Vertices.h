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
		static int numVertices;
		Vertices()
		{
			numVertices++;
		}
		int marked=0;
		int index=-1;
		double harris_response;
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
			if (level_depth < 1)
				return;
			else
				level_depth -= 1;
			set<int>  temp_vert;
			set<int>  temp_vert2;	
			//cout << "In Neighborhood Function" << endl;	
			for (int i =0 ; i <= level_depth; i++)
			{	
				if ( i ==0)
				{
					neighborhood.insert(index);
					marked = 1;
					temp_vert.insert(adjacentVert.begin(),adjacentVert.end());
					neighborhood.insert(adjacentVert.begin(),adjacentVert.end());

				}
				else
				{	//cout << "Point" << endl;
					set<int> :: iterator it;
					for(it = temp_vert.begin(); it!=temp_vert.end();it++ )
					{	//cout << "Investigating Vertex No. "  << *it << endl;

						
						//cout << "Marked Status" << vertex_in[*current].marked  << endl;
						if (vertex_in[*it].marked == 0)
						{
							temp_vert2.insert(vertex_in[*it].adjacentVert.begin(),
									 vertex_in[*it].adjacentVert.end());
							neighborhood.insert(vertex_in[*it].adjacentVert.begin(),
									 vertex_in[*it].adjacentVert.end());
						    	vertex_in[*it].marked = 1;
						}
					}
						temp_vert.clear();
						temp_vert.insert(temp_vert2.begin(),temp_vert2.end());					
						temp_vert2.clear();			
				}
			}
			
			for (int i =0 ; i < Vertices::numVertices; i++)
			{
				vertex_in[i].marked =0;			
			}
					      
		}



void getAdaptiveNeighborhood (double radius, Vertices * & vertex_in , set <int> & neighborhood, int size)
		{
			double x ,y ,z ;
			double level_depth = radius;
			set<int>  temp_vert;
			set<int>  temp_vert2;
			map<int, double> distances;
			double max_dist = 0;
			double temp_dist =0;
			distances[index] = 0;
			int flag = 0;
			cout << "RADIUS  : " << radius << endl;
		
			//cout << "In Neighborhood Function" << endl;	
			for (int i =0 ; flag == 0 ; i++)
			{	
				
				if ( i ==0)
				{
					neighborhood.insert(index);
					temp_vert.insert(index);

				}
				else
				{	//cout << "Point" << endl;
					set<int> :: iterator it;
					for(it = temp_vert.begin(); it!=temp_vert.end();it++ )
					{	//cout << "Investigating Vertex No. "  << *it << endl;

						
						//cout << "Marked Status" << vertex_in[*current].marked  << endl;
						if (vertex_in[*it].marked == 0)
						{
							temp_vert2.insert(vertex_in[*it].adjacentVert.begin(),
									 vertex_in[*it].adjacentVert.end());
							neighborhood.insert(vertex_in[*it].adjacentVert.begin(),
									 vertex_in[*it].adjacentVert.end());
						    	vertex_in[*it].marked = 1;
							temp_dist = distances[vertex_in[*it].index];

							
							
							
							set<int> :: iterator itn;

							for (itn = vertex_in[*it].adjacentVert.begin(); itn!= vertex_in[*it].adjacentVert.end(); itn++)
							{	
								cout << vertex_in[*itn].vertx << " " << vertex_in[*itn].verty << " " << vertex_in[*itn].vertz << endl;
								x = vertex_in[*it].vertx - vertex_in[*itn].vertx;
								y =vertex_in[*it].verty - vertex_in[*itn].verty;
								z = vertex_in[*it].vertz - vertex_in[*itn].vertz;
								cout << "one : "<< distances[vertex_in[*it].index] << endl;
								cout << "two : " << (sqrt(x*x + y*y +z*z))<< endl;
								double new_dist = distances[vertex_in[*it].index] + (sqrt(x*x + y*y +z*z));
								if ( i > 0)
								{

									//distances[vertex_in[*itn].index] = new_dist;
									if ( distances[vertex_in[*itn].index] == 0)
									{
										distances[vertex_in[*itn].index] = new_dist;
									}
									else if ( new_dist < distances[vertex_in[*itn].index])
									{
										distances[vertex_in[*itn].index] = new_dist;
									}
								}
								else 
								{
									distances[vertex_in[*itn].index] =0;
								}								
								temp_dist = distances[vertex_in[*itn].index];

								cout << " CHECK " << i <<  " : " << temp_dist << endl;
								if (temp_dist > max_dist)
									max_dist = temp_dist;		
							}
							
							cout << "WTF" << endl;
							
						}
						cout << "HI" << endl;
					}
						cout << " Hmmm " << endl;
						temp_vert.clear();
						temp_vert.insert(temp_vert2.begin(),temp_vert2.end());					
						temp_vert2.clear();	
						if (max_dist > level_depth || temp_vert.size() == 0)
						{
							cout << "WTF : "<<max_dist  << endl;
							cout << level_depth << endl;		
							cout << temp_vert.size() << "THIS" << endl;
							flag =1;
							break;
						}		
					cout << "sup " << endl;
				}
				cout << " TADAAAAA" << endl;
			}
			
			for (int i =0 ; i < Vertices::numVertices; i++)
			{
				vertex_in[i].marked =0;			
			}
			cout << "Size " <<  size << endl;
					      
		}

		
		int isMaximum(Vertices * & vertex_in)
		{
			set<int> :: iterator it;
			for(it = adjacentVert.begin(); it!=adjacentVert.end();it++ )
			{	
				//cout << harris_response << " " << vertex_in[*it].harris_response << endl;
				if (harris_response < vertex_in[*it].harris_response)
				{
					//cout << "Response False" << endl;
					return -1; 	
				}
			}
			//cout << "Response True" << endl;
			return 0;
		}

};
 

		


#endif
