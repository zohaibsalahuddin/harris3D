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

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include "pca.h"

void calculate_center (vector <Vertices> & nVert,double& centerx,double& centery,double &centerz)
{
	centerx = 0; 
	centery = 0;
	centerz = 0;
	for (int i =0 ; i < nVert.size() ; i++)
	{	
		centerx = nVert[i].vertx + centerx;
		centery = nVert[i].verty + centery;
		centerz = nVert[i].vertz + centerz;
	}
	centerx = centerx/nVert.size();
	centery = centery/nVert.size();
	centerz = centerz/nVert.size();
}

void shift_center_to_zero ( vector <Vertices> & nVert,double& centerx,double& centery,double &centerz )
{
	calculate_center (nVert,centerx,centery,centerz);
	for (int i =0; i < nVert.size(); i++)
	{
		nVert[i].setValues(nVert.vertx - centerx, nVert.verty - centery , nVert.vertz - centerz);
	}
}

gsl_matrix * pca_calculate(vector <Vertices> & nVert )
{
	gsl_matrix * m = gsl_matrix_alloc (nVert.size(),3);

	for(int i =0; i < nVert.size(); i++)
	{
		gsl_matrix_set(m,i,1,nVert.vertx);
		gsl_matrix_set(m,i,2,nVert.verty);
		gsl_matrix_set(m,i,3,nVert.vertz);
	}
	
	gsl_matrix * Comatrix = gsl_matrix_alloc(3,3);
	gsl_blas_dgemm(CblasTrans, CblasTrans, (1.0/(double)(nVert.size() - 1)),m,m,0.0,Comatrix);
	gsl_matrix_free(m);

	gsl_vector* evalues = gsl_vector_alloc(3);
	gsl_matrix* evectors = gsl_matrix_alloc(3,3);
	gsl_eigen_symmv_workspace* workspace = gsl_eigen_symmv_alloc(3);
	gsl_eigen_symmv(Comatrix,evalues,evectors,workspace);
	gsl_eigen_symmv_free(workspace);
	gsl_matrix_free(Comatrix);
	gsl_eigen_symmv_sort(evalues,evectors,GSL_EIGEN_SORT_ABS_DESC);
	gsl_vector_free(evalues);

	return evectors;

}

void pca_rotate (vector <Vertices> & nVert, gsl_matrix * evectors)
{
	double x1,x2,x3;
	double y1,y2,y3;
	double z1,z2,z3;

	x1 = gsl_matrix_get(evectors, 0, 0);
	x2 = gsl_matrix_get(evectors, 1, 0);
	x3 = gsl_matrix_get(evectors, 2, 0);

	y1 = gsl_matrix_get(evectors, 0, 1);
	y2 = gsl_matrix_get(evectors, 1, 1);
	y3 = gsl_matrix_get(evectors, 2, 1);
	
	z1 = gsl_matrix_get(evectors, 0, 2);
	z2 = gsl_matrix_get(evectors, 1, 2);
	z3 = gsl_matrix_get(evectors, 2, 2);

	for (int i =0 ; i < nVert.size(); i++)
	{
		nVert[i].setValues(x1*nVert[i].vertx + y1*nVert[i].verty + z1*nVert[i].vertz ,x2*nVert[i].vertx + y2*nVert[i].verty + z2*nVert[i].vertz,x3*nVert[i].vertx + y3*nVert[i].verty + z3*nVert[i].vertz);

	}
	gsl_matrix_free(evectors);
}















