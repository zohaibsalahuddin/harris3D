/*
	Date Created: 23rd November
	FILENAME : fileread.cpp
------------------------------------------------
 *
 *	This file contains declarations for functions 
 *	to open the input file containing the vertices
 *	and faces information
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#ifndef __FILE_READ__
#define __FILE_READ__

#include "Faces.h"
#include "Vertices.h"
#include "pca.h"
#include <iostream>
#include <cfloat>
#include <vector>
#include "Faces.h"
#include "Vertices.h"


using namespace std;

void skipline(istream &in);
int readVertFace (const string filePath , Faces *& ptrFaces , Vertices *& ptrVertices);
int cal_interest_points(double ** & result, int & size_result, string filename, double harris_parameter, double fraction, int radius_param, string selection_type);


#endif
