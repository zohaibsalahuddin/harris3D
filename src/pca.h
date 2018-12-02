/*
	Date Created: 24thNovember
	FILENAME : pca.h
------------------------------------------------
 *
 *	This file contains the definitions for dimensionality reduction
 *	and harris response calculation
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/



#ifndef __PCA__
#define __PCA__

#include <iostream>
#include "Eigen/Core"
#include "Eigen/Eigen"
#include "Vertices.h"
#include "Faces.h"
#include <vector>


using namespace std;
using namespace Eigen;


void calculate_center (vector <Vertices> & nVert,double& centerx,double& centery,double &centerz);
void shift_center_to_zero ( vector <Vertices> & nVert,double& centerx,double& centery,double &centerz );
MatrixXd * pca_calculate(vector <Vertices> & nVert);
void pca_rotate (vector <Vertices> & nVert, MatrixXd * eigen_vectors);
void calculate_center (vector <Vertices> & nVert,double& centerx,double& centery,double &centerz );
void direction_check_shift (vector <Vertices> & nVert, MatrixXd * eigen_vectors, int index_vertex,double& centerx,double& centery,double &centerz);
void shift_to_vertex_centerxy(vector <Vertices> & nVert, int index_vertex);
void quadratic_fit (vector <Vertices> & nVert, double & p1, double &p2, double & p3, double &p4 , double & p5 , double &p6);
double get_harris_response (double & p1, double &p2, double & p3, double &p4 , double & p5 , double &p6, double &k);

#endif
































