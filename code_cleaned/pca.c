/*
	Date Created: 24thNovember
	FILENAME : pca.h
------------------------------------------------
 *
 *	This file contains the declarations for dimensionality reduction
 *	and harris response calculation
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#include "Vertices.h"s
#include "pca.h"

using namespace std;

void calculate_center (vector <Vertices> & nVert, int total_vertices,double& centerx,double& centery,double &centerz)
{
	centerx = 0; 
	centery = 0;
	centerz = 0;
	for (int i =0 ; i < total_vertices ; i++)
	{	
		centerx = nVert(i).vertx + centerx;
		centery = nVert(i).verty + centery;
		centerz = nVert(i).vertz + centerz;
	}
	centerx = centerx/total_vertices;
	centery = centery/total_vertices;
	centerz = centerz/total_vertices;
}

void shift_center_to_zero ( vector <Vertices> & nVert, int total_vertices,double& centerx,double& centery,double &centerz )
{
	calculate_center (nVert,centerx,centery,centerz);
	for (int i =0; i < total_vertices; i++)
	{
		nVert(i).setValues(nVert(i).vertx - centerx, nVert(i).verty - centery , nVert(i).vertz - centerz);
	}
}

MatrixXd * pca_calculate(vector <Vertices> & nVert, int total_vertices )
{

	MatrixXd data = MatrixXd::Zero(total_vertices,3);

	for(int i =0; i < total_vertices; i++)
	{
		data(i,1) = nVert(i).vertx;
		data(i,2) = nVert(i).verty;
		data(i,3) = nVert(i).vertz;
	}

  	MatrixXd covariance = MatrixXd::Zero(3, 3);
  	covariance = (1 / (double) 9) * data * data.transpose();

  	EigenSolver<MatrixXd> solver(covariance);
  	VectorXd eigen_values = VectorXd::Zero(3);
  	eigen_values = solver.eigenvalues().real();
  
  	MatrixXd * eigen_vectors = new MatrixXd::Zero(total_vertices, 3);
  	*eigen_vectors = solver.eigenvectors().real();

	//Sorting the eigen values
	VectorXd temp= VectorXd::Zero(3);
	if (eigen_values(0) < eigen_values(1))
	{
		temp = (*eigen_vectors).col(0);
		(*eigen_vectors).col(0) = (*eigen_vectors).col(1);
		(*eigen_vectors).col(1) = temp;
	}

	if (eigen_values(1) < eigen_values(2))
	{
		temp = (*eigen_vectors).col(1);
		(*eigen_vectors).col(1) = (*eigen_vectors).col(2);
		(*eigen_vectors).col(2) = temp;
	}

	if (eigen_values(0) < eigen_values(1))
	{
		temp = (*eigen_vectors).col(0);
		(*eigen_vectors).col(0) = (*eigen_vectors).col(1);
		(*eigen_vectors).col(1) = temp;
	}


	return eigen_vectors;

}

void pca_rotate (vector <Vertices> & nVert, MatrixXd * eigen_vectors, int total_vertices)
{


	double x1,x2,x3;
	double y1,y2,y3;
	double z1,z2,z3;

	x1 = (*eigen_vectors)(0, 0);
	x2 = (*eigen_vectors)(1, 0);
	x3 = (*eigen_vectors)(2, 0);

	y1 = (*eigen_vectors)(0, 1);
	y2 = (*eigen_vectors)(1, 1);
	y3 = (*eigen_vectors)(2, 1);
	
	z1 = (*eigen_vectors)(0, 2);
	z2 = (*eigen_vectors)(1, 2);
	z3 = (*eigen_vectors)(2, 2);

	for (int i =0 ; i < total_vertices; i++)
	{
		//nVert[i].setValues(x1*nVert[i].vertx + y1*nVert[i].verty + z1*nVert[i].vertz ,x2*nVert[i].vertx + y2*nVert[i].verty + z2*nVert[i].vertz,x3*nVert[i].vertx + y3*nVert[i].verty + z3*nVert[i].vertz);

	}


}















