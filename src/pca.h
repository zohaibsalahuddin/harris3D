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


#endif
































