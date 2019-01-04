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
#include "mesh.h"
#include "pca.h"
#include "harris.h"

using namespace std;
using namespace Eigen;



int main(void)
{
	// Parameters to be edited
	double harris_parameter = 0.04;
	double fraction  = 0.01;
	double radius_param = 1;
	string neighborhood_type = "ring";
	string filename1 = "/home/zohaib123/Desktop/Harris3D/harris3D/src/sphere.off";	
	string filename2 = "/home/zohaib123/Desktop/Harris3D/harris3D/src/bunny.off";	
	string selection_type;
	double ** result;
	int size_result;
	selection_type= "fraction";

	int ret = cal_interest_points(result, size_result, filename1,harris_parameter,fraction,radius_param,selection_type,neighborhood_type);

	if (ret == 0)
	{
		cout << "Interest Points Calculation Successful" << endl;
	}
	else
	{
		cout << "Interest Points Calculation Failed" << endl;
	}

		cout << "Iterating through the interest Points:" << endl;

		for (int i =0; i < size_result; i++)
		{
			cout << " POINT " << i << " : " << " x " <<result[i][0] << " y " <<result[i][1] << " z " <<result[i][2]  <<endl;
		}
	
	for (int i =0; i < size_result ; i++)
	{
		delete[] result[i];
	}
	delete [] result;


	int ** result_n;
	int size_result_n;
	int * face;
	int ret2 = get_faces(result_n,face, size_result_n, filename1,radius_param, 0.426 , 0.64 , -0.64,neighborhood_type);

	cout << "neighborhood faces are" << endl;
	for (int i =0 ; i < size_result_n ; i++)
	{
		cout << face[i] << endl;
	}

	for (int i =0; i < size_result_n ; i++)
	{
		delete[] result_n[i];
	}
	delete [] result_n;
	delete[] face;
	ret2 = get_faces(result_n,face, size_result_n, filename1,radius_param,   0.426 , 0.64, 0.64,neighborhood_type);

	cout << "neighborhood faces are" << endl;
	for (int i =0 ; i < size_result_n ; i++)
	{
		cout << face[i] << endl;
	}

	for (int i =0; i < size_result_n ; i++)
	{
		delete[] result_n[i];
	}
	delete [] result_n;
	delete[] face;
 /*	ret2 = get_faces(result_n,face, size_result_n, filename1,radius_param,  0.069 , -1.045 , -0.069,neighborhood_type);

	cout << "neighborhood faces are" << endl;
	for (int i =0 ; i < size_result_n ; i++)
	{
		cout << face[i] << endl;
	}

	for (int i =0; i < size_result_n ; i++)
	{
		delete[] result_n[i];
	}
	delete [] result_n;
	delete[] face;


	int ret3 = get_faces(result_n,face, size_result_n, filename1,radius_param,-0.606 ,-0.606 ,-0.606 ,neighborhood_type);

	cout << "neighborhood faces are" << endl;
	for (int i =0 ; i < size_result_n ; i++)
	{
		cout << face[i] << endl;
	}

	for (int i =0; i < size_result_n ; i++)
	{
		delete[] result_n[i];
	}
	delete [] result_n;
	delete[] face;

	ret3 = get_faces(result_n,face, size_result_n, filename1,radius_param, -0.606 ,-0.606 ,0.606,neighborhood_type );

	cout << "neighborhood faces are" << endl;
	for (int i =0 ; i < size_result_n ; i++)
	{
		cout << face[i] << endl;
	}

	for (int i =0; i < size_result_n ; i++)
	{
		delete[] result_n[i];
	}
	delete [] result_n;
	delete[] face;

ret3 = get_faces(result_n,face, size_result_n, filename1,radius_param, -0.606 ,-0.606 ,0.606,neighborhood_type );

	cout << "neighborhood faces are" << endl;
	for (int i =0 ; i < size_result_n ; i++)
	{
		cout << face[i] << endl;
	}

	for (int i =0; i < size_result_n ; i++)
	{
		delete[] result_n[i];
	}
	delete [] result_n;
	delete[] face;
*/
}
