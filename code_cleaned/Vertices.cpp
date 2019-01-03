/*
	Date Created: 24th November
	FILENAME : Vertices.h
------------------------------------------------
 *
 *	This file contains definitions for various 
 *	functions available in the Vertices.h file.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#include "Vertices.h"


Vertices::Vertices()
{
	numVertices++;
}


void Vertices::setValues ( double x , double y , double z)
{
			vertx = x;
			verty = y;
			vertz = z;
}


void Vertices::addFace(int face)
{
	facesIncVert.insert(face);
}


void Vertices::addAdjacentVert(int vert1,int vert2)
{
	adjacentVert.insert(vert1);
	adjacentVert.insert(vert2);
}


void Vertices::getRingNeighborhood (int level_depth, Vertices * & vertex_in , set <int> & neighborhood)
{
	if (level_depth < 1)
		return;
	else
		level_depth -= 1;
			
	set<int>  temp_vert;
	set<int>  temp_vert2;	
	
	
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
		{	
			set<int> :: iterator it;
			for(it = temp_vert.begin(); it!=temp_vert.end();it++ )
			{	
						
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



void Vertices::getAdaptiveNeighborhood (double radius, Vertices * & vertex_in , set <int> & neighborhood, int size)
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
			
			
	for (int i =0 ; flag == 0 ; i++)
	{		
		if ( i ==0)
		{
			neighborhood.insert(index);
			temp_vert.insert(index);

		}
		else
		{	
			set<int> :: iterator it;
			for(it = temp_vert.begin(); it!=temp_vert.end();it++ )
			{	
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


						double new_dist = distances[vertex_in[*it].index] + (sqrt(x*x + y*y +z*z));
						if ( i > 0)
						{

							distances[vertex_in[*itn].index] = new_dist;
							//if ( distances[vertex_in[*itn].index] == 0)
							//{
							//	distances[vertex_in[*itn].index] = new_dist;
							//}
							//else if ( new_dist < distances[vertex_in[*itn].index])
							//{
							//	distances[vertex_in[*itn].index] = new_dist;
							//}
						}
						else 
						{
							distances[vertex_in[*itn].index] =0;
						}								
						temp_dist = distances[vertex_in[*itn].index];

						if (temp_dist > max_dist)
							max_dist = temp_dist;		
						}
							

							
					}

				}

				temp_vert.clear();
				temp_vert.insert(temp_vert2.begin(),temp_vert2.end());					
				temp_vert2.clear();
	
				if (max_dist > level_depth || temp_vert.size() == 0)
				{
					cout << level_depth << endl;		
					cout << temp_vert.size() << "THIS" << endl;
					flag =1;
					break;
				}		

			}

		}
			
	for (int i =0 ; i < Vertices::numVertices; i++)
	{
		vertex_in[i].marked =0;			
	}	
			      
}

int Vertices::isMaximum(Vertices * & vertex_in)
{
	set<int> :: iterator it;
	for(it = adjacentVert.begin(); it!=adjacentVert.end();it++ )
	{	
		if (harris_response < vertex_in[*it].harris_response)
		{
			return -1; 	
		}
	}

	return 0;
}
