/*
	Date Created: 27th December
	FILENAME : harris.h
------------------------------------------------
 *
 *	This file contains definitions for functions
 *	that will be required by the open gl interface.
 * 	These function definitions will be used to get 
 *	neighborhood, interest points and everything.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/


#ifndef __HARRIS__
#define __HARRIS__

#include <iostream>
#include <string>
#include <fstream>
#include "mesh.h"
#include "Faces.h"
#include "Vertices.h"
#include <set>
#include "pca.h"
#include <math.h>


bool double_equals (double a, double b, double epsilon);
int cal_interest_points(double ** & result, int & size_result, string filename, double harris_parameter, double fraction, double radius_param, string selection_type,string n_type);
int get_faces(int ** & result, int * & face, int & size_result, string filename,double radius_param, double x, double y , double z,string n_type);



#endif
