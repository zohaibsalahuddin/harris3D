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

#include "Vertices.h"
#include "pca.h"

using namespace std;

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
	cout << "Before Dividing: " << centerx << " " << centery<< " "  << centerz<< " "  << endl;
	cout << "Size : " << nVert.size() << endl;
	centerx = centerx/nVert.size();
	centery = centery/nVert.size();
	centerz = centerz/nVert.size();
}

void shift_center_to_zero ( vector <Vertices> & nVert,double& centerx,double& centery,double &centerz )
{

	for (int i =0; i < nVert.size(); i++)
	{
		nVert[i].setValues(nVert[i].vertx - centerx, nVert[i].verty - centery , nVert[i].vertz - centerz);
	}
}

MatrixXd * pca_calculate(vector <Vertices> & nVert)
{

	MatrixXd data = MatrixXd::Zero(3,nVert.size());

	for(int i =0; i < nVert.size(); i++)
	{
		data(0,i) = nVert[i].vertx;
		data(1,i) = nVert[i].verty;
		data(2,i) = nVert[i].vertz;
	}

  	MatrixXd covariance = MatrixXd::Zero(3, 3);
  	covariance = (1 / (double) (nVert.size()-1)) * data * data.transpose();

  	SelfAdjointEigenSolver<MatrixXd> solver(covariance);
  	VectorXd eigen_values = VectorXd::Zero(3);
  	eigen_values = solver.eigenvalues().real();
  
  	MatrixXd * eigen_vectors = new MatrixXd(MatrixXd::Zero(nVert.size(), 3));
  	*eigen_vectors = solver.eigenvectors().real();
	double temp_eigen_val = 0;
	//Sorting the eigen values
	VectorXd temp= VectorXd::Zero(3);
	cout << "Eigen Values"  << endl;
	cout << eigen_values << endl;
	cout << "MATRIX BEFORRRRRREEEE"<< endl;
	cout << *eigen_vectors << endl;
	cout << "MATRIX After"<< endl;
	

	if (eigen_values(0) < eigen_values(1))
	{	

		temp_eigen_val = eigen_values(0);
		eigen_values(0) = eigen_values(1);
		eigen_values(1) = temp_eigen_val;
		temp = (*eigen_vectors).col(0);
		(*eigen_vectors).col(0) = (*eigen_vectors).col(1);
		(*eigen_vectors).col(1) = temp;
	}

	if (eigen_values(1) < eigen_values(2))
	{
		temp_eigen_val = eigen_values(1);
		eigen_values(2) = eigen_values(1);
		eigen_values(1) = temp_eigen_val;
		temp = (*eigen_vectors).col(1);
		(*eigen_vectors).col(1) = (*eigen_vectors).col(2);
		(*eigen_vectors).col(2) = temp;
	}

	if (eigen_values(0) < eigen_values(1))
	{
		temp_eigen_val = eigen_values(0);
		eigen_values(0) = eigen_values(1);
		eigen_values(1) = temp_eigen_val;
		temp = (*eigen_vectors).col(0);
		(*eigen_vectors).col(0) = (*eigen_vectors).col(1);
		(*eigen_vectors).col(1) = temp;
	}

	cout << "Eigen Values"  << endl;
	cout << eigen_values << endl;
	return eigen_vectors;

}

void pca_rotate (vector <Vertices> & nVert, MatrixXd * eigen_vectors)
{


	double x1,x2,x3;
	double y1,y2,y3;
	double z1,z2,z3;
	cout << "MATRIXXXX" << endl;
	cout << *eigen_vectors <<endl;
	x1 = (*eigen_vectors)(0, 0);
	x2 = (*eigen_vectors)(1, 0);
	x3 = (*eigen_vectors)(2, 0);

	y1 = (*eigen_vectors)(0, 1);
	y2 = (*eigen_vectors)(1, 1);
	y3 = (*eigen_vectors)(2, 1);
	
	z1 = (*eigen_vectors)(0, 2);
	z2 = (*eigen_vectors)(1, 2);
	z3 = (*eigen_vectors)(2, 2);


		cout << "MATRIX ##########" << endl;		
		cout << x1 << " " << x2 << " " << x3 << endl;
		cout << y1 << " " << y2 << " " << y3 << endl;
		cout << z1 << " " << z2 << " " << z3 << endl;		
		cout << "MATRIX ########" << endl;

	for (int i =0 ; i < nVert.size(); i++)
	{
		nVert[i].setValues(((x1*nVert[i].vertx) + (x2*nVert[i].verty) + (x3*nVert[i].vertz)) ,((y1*nVert[i].vertx) + (y2*nVert[i].verty) + (y3*nVert[i].vertz)),((z1*nVert[i].vertx) + (z2*nVert[i].verty) + (z3*nVert[i].vertz)));

	}


}

void direction_check_shift (vector <Vertices> & nVert, MatrixXd * eigen_vectors, int index_vertex, double& centerx,double& centery,double &centerz)
{
	
	double vshift_x,vshift_y,vshift_z;
	VectorXd temp = VectorXd::Zero(3);
	vshift_x = nVert[index_vertex].vertx - centerx;
	vshift_y = nVert[index_vertex].verty - centery;
	vshift_z = nVert[index_vertex].vertz - centerz;

		
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


		cout << "MATRIX 5##########" << endl;		
		cout << x1 << " " << x2 << " " << x3 << endl;
		cout << y1 << " " << y2 << " " << y3 << endl;
		cout << z1 << " " << z2 << " " << z3 << endl;		
		cout << "MATRIX ########" << endl;

	double threshold = z1*vshift_x + z2*vshift_y + z3*vshift_z;
	
	if(threshold < 0)
	{
		cout << "HERE?" << endl;
		(*eigen_vectors).col(2) = -1 * (*eigen_vectors).col(2);
		temp = (*eigen_vectors).col(0);
	 	(*eigen_vectors).col(0) = (*eigen_vectors).col(1);
		(*eigen_vectors).col(1) = temp;
			
	}


}

void shift_to_vertex_centerxy(vector <Vertices> & nVert, int index_vertex)
{
	double x = nVert[index_vertex].vertx;
	double y = nVert[index_vertex].verty;
	cout <<  "X " << x << "Y " << y << endl;	
	for(int i = 0; i < nVert.size(); i++)
	{
		nVert[i].setValues(nVert[i].vertx - x,nVert[i].verty - y,nVert[i].vertz);
	}
		
}


void quadratic_fit (vector <Vertices> & nVert, double & p1, double &p2, double & p3, double &p4 , double & p5 , double &p6)
{
	MatrixXd data = MatrixXd::Zero(nVert.size(),6);
	VectorXd B= VectorXd::Zero(nVert.size());
	VectorXd X= VectorXd::Zero(6);
	for(int i =0; i < nVert.size(); i++)
	{
		data(i,0) = (nVert[i].vertx* nVert[i].vertx)/2;
		data(i,1) = nVert[i].vertx * nVert[i].verty;
		data(i,2) = (nVert[i].verty * nVert[i].verty)/2;
		data(i,3) = nVert[i].vertx;
		data(i,4) = nVert[i].verty;
		data(i,5) = 1;
		B(i) =  nVert[i].vertz;
	}

	X =  data.colPivHouseholderQr().solve(B);
	p1 =X(0);
	p2 =X(1);
	p3 =X(2);
	p4 =X(3);
	p5 =X(4);
	p6 =X(5);
}

double get_harris_response (double & p1, double &p2, double & p3, double &p4 , double & p5 , double &p6, double & k)
{
	double response;
	double A,B,C;
	double tr,det;
	A = (p4*p4) + (2*p1*p1) + (2*p2*p2);
	B = (p5*p5) + (2*p2*p2) + (2*p3*p3);
	C = (p4*p5) + (2*p1*p2) + (2*p2*p3);

	det = (A*B) - (C*C);
	tr = A+B;
	response = det - (k*tr*tr);
	return response;
}






