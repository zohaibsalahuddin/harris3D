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



int main(void)
{
	// Parameters to be edited
	double harris_parameter = 0.04;
	double fraction  = 0.25;
	int radius_param =2;
	string filename1 = "/home/zohaib123/Desktop/Harris3D/harris3D/src/epcot.off";	
	string filename2 = "/home/zohaib123/Desktop/Harris3D/harris3D/src/bunny.off";	
	string selection_type;
	double ** result;
	int size_result;
	selection_type= "fraction";

	int ret = cal_interest_points(result, size_result, filename2,harris_parameter,fraction,radius_param,selection_type);

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
			cout << " POINT " << i << " : " << "x " <<result[i][0] << "y " <<result[i][1] << " z " <<result[i][2]  <<endl;
		}
	
	for (int i =0; i < size_result ; i++)
	{
		delete[] result[i];
	}
	delete [] result;

	int ** result_n;
	int size_result_n;
	int * face;
	int ret2 = get_faces(result_n,face, size_result_n, filename2,2, -0.579332, 0.564556 , -0.318829);

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

	int ret3 = get_faces(result_n,face, size_result_n, filename2,2, 0.301401, -0.235867 , -0.392926 );

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


}
