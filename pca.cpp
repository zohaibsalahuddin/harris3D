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

/* FUNCTION: calculate_center
 * --------------------------
 * Calculates a centroid of a vector of verticies
 * Inputs:
 * - vector <Vertices> &: a vector containing the neighborhood verticies
 * - double& : x aixs centroid coordinate passed by reference
 * - double& : y axis centroid coordinate passed by reference
 * - double& : z aixs centroid coordinate passed by reference
 *
 * Output: returns nothing
 */

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

/* FUNCTION: shift_center_to_zero
 * --------------------------
 * Moves the centroiod of a vector of vertiecies to the origin of the coordinate system
 * Inputs:
 * - vector <Vertices> &: a vector containing the neighborhood verticies
 * - double& : x aixs centroid coordinate passed by reference
 * - double& : y axis centroid coordinate passed by reference
 * - double& : z aixs centroid coordinate passed by reference
 *
 * Output: returns nothing
 */
void shift_center_to_zero ( vector <Vertices> & nVert,double& centerx,double& centery,double &centerz )
{

    for (int i =0; i < nVert.size(); i++)
    {
        nVert[i].setValues(nVert[i].vertx - centerx, nVert[i].verty - centery , nVert[i].vertz - centerz);
    }
}

/* FUNCTION: pca_calculate
 * --------------------------
 * Performs PCA by calculating the covarinance matrix and its eignevectors, and arranging them
 * in a decending order, of a given vector of verticies.
 * Inputs:
 * - vector <Vertices> &: a vector containing the neighborhood verticies
 *
 * Output: returns MatrixXd *: a pointer to an object of Matrix class Xd and that returns a matrix of eign
 *         vectors arranged in columns
 */
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
    VectorXd temp= VectorXd::Zero(3);


    if (eigen_values(0) <= eigen_values(1))
    {

        temp_eigen_val = eigen_values(0);
        eigen_values(0) = eigen_values(1);
        eigen_values(1) = temp_eigen_val;
        temp = (*eigen_vectors).col(0);
        (*eigen_vectors).col(0) = (*eigen_vectors).col(1);
        (*eigen_vectors).col(1) = temp;
    }

    if (eigen_values(1) <= eigen_values(2))
    {
        temp_eigen_val = eigen_values(1);
        eigen_values(1) = eigen_values(2);
        eigen_values(2) = temp_eigen_val;
        temp = (*eigen_vectors).col(1);
        (*eigen_vectors).col(1) = (*eigen_vectors).col(2);
        (*eigen_vectors).col(2) = temp;
    }

    if (eigen_values(0) <=eigen_values(1)  )
    {
        temp_eigen_val = eigen_values(0);
        eigen_values(0) = eigen_values(1);
        eigen_values(1) = temp_eigen_val;
        temp = (*eigen_vectors).col(0);
        (*eigen_vectors).col(0) = (*eigen_vectors).col(1);
        (*eigen_vectors).col(1) = temp;
    }
    return eigen_vectors;

}


/* FUNCTION: pca_rotate
 * --------------------------
 * Performs a dot product between the vector of verticies (passed by reference) and the matrix of eign vectors
 * Inputs:
 * - vector <Vertices> &: a vector containing the neighborhood verticies
 * - MatrixXd *: a pointer to a matrix of class Xd contining the eign vectors
 *
 * Output: returns nothing
 */
void pca_rotate (vector <Vertices> & nVert, MatrixXd * eigen_vectors)
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

    for (int i =0 ; i < nVert.size(); i++)
    {
        nVert[i].setValues(((x1*nVert[i].vertx) + (x2*nVert[i].verty) + (x3*nVert[i].vertz)) ,((y1*nVert[i].vertx) + (y2*nVert[i].verty) + (y3*nVert[i].vertz)),((z1*nVert[i].vertx) + (z2*nVert[i].verty) + (z3*nVert[i].vertz)));

    }


}

/* FUNCTION: direction_check_shift
 * --------------------------
 * Checks the direction of the norm of the rotated plane if it points in the +ve z-axis; if not, it
 * swaps the x and y axis and revers the z-axis direction.
 * Inputs:
 * - vector <Vertices> &: a vector containing the neighborhood verticies
 * - MatrixXd *: a pointer to a matrix of class Xd contining the eign vectors
 * - int: index of the targeted vertix for harris operator calculation
 * - double& : x axis of the verticies centroid passed by reference
 * - double& : y axis of the verticies centroid passed by reference
 * - double& : z axis of the verticies centroid passed by reference
 *
 * Output: returns nothing
 */
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

    double threshold = z1*vshift_x + z2*vshift_y + z3*vshift_z;

    if(threshold < 0)
    {
        (*eigen_vectors).col(2) = -1 * (*eigen_vectors).col(2);
        temp = (*eigen_vectors).col(0);
        (*eigen_vectors).col(0) = (*eigen_vectors).col(1);
        (*eigen_vectors).col(1) = temp;

    }


}

/* FUNCTION: shift_to_vertex_centerxy
 * --------------------------
 * Shifts the vector of verticies with respect to the targeted vertix to center it at the origing of the xy-plane
 * Inputs:
 * - vector <Vertices> &: a vector containing the neighborhood verticies
 * - int: index of the targeted vertix for harris operator calculation
 *
 * Output: returns nothing
 */
void shift_to_vertex_centerxy(vector <Vertices> & nVert, int index_vertex)
{
    double x = nVert[index_vertex].vertx;
    double y = nVert[index_vertex].verty;
    for(int i = 0; i < nVert.size(); i++)
    {
        nVert[i].setValues(nVert[i].vertx - x,nVert[i].verty - y,nVert[i].vertz);
    }

}

/* FUNCTION: quadratic_fit
 * --------------------------
 * solves the eqation: Ax = B; where A is the matrix of verticies(nx3), x is the quadratic plane coefficents(p1,..,p6)
 * and B is the normal of the quadratic plane.
 * Inputs:
 * - vector <Vertices> &: a vector containing the neighborhood verticies
 * - double &: coefficients of the quadratic function (p1,...,p6)
 *
 * Output: returns nothing
 */
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

/* FUNCTION: get_harris_response
 * --------------------------
 * Calculates harris response of the targeted vertix
 * Inputs:
 * - double &: coefficients of the fitted quadratic function (p1,...,p6)
 * - double &: harris constant
 *
 * Output: returns nothing
 */
double get_harris_response (double & p1, double &p2, double & p3, double &p4 , double & p5 , double &p6, double & k)
{
    double response;
    double A,B,C;
    double tr,det;
    A = (p4*p4) + (2*p1*p1) + (2*p2*p2);
    B = (p4*p4) + (2*p2*p2) + (2*p3*p3);
    C = (p4*p5) + (2*p1*p2) + (2*p2*p3);


    det = (A*B) - (C*C);
    tr = A+B;
    response = det - (k*tr*tr);
    return response;
}

/* FUNCTION: responce_compare
 * --------------------------
 * compare harris responce of two verticies to check the vertix for local maximum
 * Inputs:
 * - Vertices: 1st vertix
 * - Vertices: 2nd vertix
 *
 *
 * Output: returns nothing
 */
bool response_compare(Vertices a, Vertices b)
{
    return (a.harris_response > b.harris_response);
}




