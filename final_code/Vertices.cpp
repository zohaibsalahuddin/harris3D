/*
	Date Created: 24th November
	FILENAME : Vertices.cpp
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

/*
 *	Constructor to Vertices Class
-------------------------------------------------
 *	This is a constructor to Vertices
 *	Class
 * 
 *	Parameters:
 *	None
 *	Return  : None
*/


Vertices::Vertices()
{
	marked = 0;
	index = -1;
}

/* FUNCTION: Vertices::setValues
 * --------------------------
 * Sets the (x,y,z) coordinate values of the vertix
 * Inputs:
 * - double: value of the vertix x-coordinate
 * - double: value of the vertix y-coordinate
 * - double: value of the vertix z-coordinate
 *
 * Output: returns nothing
 */

void Vertices::setValues ( double x , double y , double z)
{
	vertx = x;
	verty = y;
	vertz = z;
}

/* FUNCTION: Vertices::addFace
 * --------------------------
 * Adds a face to the set of faces of a vertix
 * Inputs:
 * - int: index of the face to be added to the set
 *
 * Output: returns nothing
 */

void Vertices::addFace(int face)
{
	facesIncVert.insert(face);
}

/* FUNCTION: Vertices::addAdjacentVert
 * --------------------------
 * Adds the two adjacent verticies of a vertix to a set that includes all adjacent vertices for that vertix
 * Inputs:
 * - int: index of the 1st adajcent vertix
 * - int: index of the 2nd adajcent vertix
 *
 * Output: returns nothing
 */

void Vertices::addAdjacentVert(int vert1,int vert2)
{
	adjacentVert.insert(vert1);
	adjacentVert.insert(vert2);
}

/* FUNCTION: Vertices::getRingNeighborhood
 * --------------------------
 * Finds the neighboring vertices of a given vertix that are within the area spanned of a spicific number of rings
 * Inputs:
 * - int: number of rings to be included in calculating the neighborhood
 * - Vertices * &: a pointer to the object Verticies that include all the vertices in the file
 * - set <int> &: a set of integers that include the indexes fo the vertices of the neighborhood (passed by reference)
 *
 * Output: returns nothing
 */

void Vertices::getRingNeighborhood (int level_depth, Vertices * & vertex_in , set <int> & neighborhood,int size)
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
			
	for (int i =0 ; i < size; i++)
	{
		vertex_in[i].marked =0;			
	}
					      
}


/* FUNCTION: Vertices::getAdaptiveNeighborhood
 * --------------------------
 * Finds the neighboring vertices of a given vertix that are within the area spanned by a spicific radius
 * Inputs:
 * - double: radius of the area to be covered for the calcultion, it is determined from multiplying the object
 *           diagonal by a chosen parameter
 * - Vertices * &: a pointer to the object Verticies that include all the vertices in the file
 * - set <int> &: a set of integers that include the indexes fo the vertices of the neighborhood (passed by reference)
 * - int: the number of total vertices
 *
 * Output: returns nothing
 */

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
			
	for (int i =0 ; i < size; i++)
	{
		vertex_in[i].marked =0;			
	}	
			      
}


/* FUNCTION: Vertices::isMaximum
 * --------------------------
 * Checks a vertix is a local maximum by comparing its harris responce to its neighborhood vertices' harris responce
 * Inputs:
 * - Vertices * &: a pointer to the object Verticies that include all the vertices in the file
 *
 * Output: returns int: 0 if the vertix is a local maximum or -1 if it is not
 */


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


/*
 *	Destructor to Vertices Class
-------------------------------------------------
 *	This is a Destructor to Vertices
 *	Class
 * 
 *	Parameters:
 *	None
 *	Return  : None
*/


Vertices::~Vertices()
{
	
}



