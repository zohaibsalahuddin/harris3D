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
#include <cfloat>
#include <vector>
#include "Faces.h"
#include "Vertices.h"
#include "fileread.h"
#include "pca.h"

using namespace std;
using namespace Eigen;

int Faces::numFaces =0;
int Vertices::numVertices =0;

int main(void)
{
	// Parameters to be edited
	double k = 0.04;
	double fraction  = 0.01;
	int radius_param =2;

	Faces * ptrfaces;
	Vertices * ptrvertices;
	vector <Vertices> nVert;
	int totalfaces =0;
	int totalVertices =0;
	string filename;

	cout << "Calling the Function to read the Vertex and Faces" << endl;
	readVertFace(filename,ptrfaces,ptrvertices);
	totalfaces = Faces::numFaces;
	totalVertices =Vertices::numVertices ;
	set <int> neighbor;

	for (int i =0 ; i < totalVertices; i++)
	{

		ptrvertices[i].getRingNeighborhood(radius_param,ptrvertices,neighbor);
		set<int> :: iterator it;
		int i1 =0;
		int vertex_index =0;
		for (it = neighbor.begin(); it!= neighbor.end(); it++)
		{	
			if ((*it) == ptrvertices[i].index)
			{
				vertex_index = i1;
			}
			i1++;
			nVert.push_back(ptrvertices[*it]);
		}


		
		double centerx,centery,centerz;
		double  p1,p2,p3,p4,p5,p6;
		double response =0;
		if (nVert.size() > 6)
		{
			calculate_center (nVert,centerx,centery,centerz);
			shift_center_to_zero (nVert, centerx, centery,centerz );
	
			MatrixXd * eigen_vectors = pca_calculate(nVert);

			direction_check_shift (nVert, eigen_vectors, vertex_index, centerx,centery,centerz);
 			pca_rotate (nVert, eigen_vectors);

			shift_to_vertex_centerxy(nVert, vertex_index);
		
			quadratic_fit (nVert, p1, p2, p3, p4 , p5 ,p6);
			response = get_harris_response (p1, p2, p3, p4 , p5 ,p6,k);
			ptrvertices[i].harris_response = response;
		}
		else
		{
			ptrvertices[i].harris_response = DBL_MIN;
		}
		
		nVert.clear();	
		neighbor.clear();
		
	}
		vector <Vertices> interest_points_all;
		int flag;
		for (int i =0 ; i < totalVertices; i++)
		{
			int flag = (ptrvertices[i].isMaximum(ptrvertices));

			if (flag == 0)
			{
				interest_points_all.push_back(ptrvertices[i]);
			}
		
		}

		sort(interest_points_all.begin(), interest_points_all.end(), response_compare);

		int points_fraction = fraction * totalVertices;
		
		cout << "size of points: " << interest_points_all.size() << ", Fraction: " << points_fraction << endl;
		cout << "Interest Points: " << endl;

		for (int i = 0 ; ((i < points_fraction) & (i < interest_points_all.size()));i++)
		{
			cout << interest_points_all[i].vertx << " "<< interest_points_all[i].verty << " "<< interest_points_all[i].vertz << endl;
		}
		
		
	
	
	return 0;


}
