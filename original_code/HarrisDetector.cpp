#include <cassert>
#include <cstdlib>
#include <cfloat>
#include <algorithm>
#include <vector>
#include <set>
#include <string.h>

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

#include "Mesh.h"
#include "HarrisDetector.h"
#include "Clock.h"


using namespace std;

HarrisDetector :: HarrisDetector(){
	typeNeighborhood = ADAPTIVE;
	fractionDiagonal = 0.01;
	numberRingNeighbor = 0;
	
	k = 0.04;
	
	numberRingsDetection = 1;
	
	typeSelection = FRACTION;
	paramSelection = 0.01;
	
	object = NULL;
	prop = NULL;
}

HarrisDetector :: HarrisDetector(Mesh* obj, Properties* pr){
	object = obj;
	prop = pr;
	
	processOptions();
}

void HarrisDetector :: processOptions(){
	/*We evaluate the properties*/
	//typeNeighborhood
	string val = prop->getProperty("type-neighborhood");
	if(val.empty()){
		cout << "Option type-neighborhood was not set. ADAPTIVE is assumed." << endl;
		typeNeighborhood = ADAPTIVE;
	}
	else{
		if(!val.compare("spatial"))
			typeNeighborhood = SPATIAL;
		else if(!val.compare("rings"))
			typeNeighborhood = RINGS;
		else if(!val.compare("adaptive"))
			typeNeighborhood = ADAPTIVE;
		else{
			cout << "Option type-neighborhood is not recognized. ADAPTIVE is assumed." << endl;
			typeNeighborhood = ADAPTIVE;
		}
	}
	
	//typeSelection
	val = prop->getProperty("interest-points-selection");
	if(val.empty()){
		cout << "Option interest-points-detection was not set. FRACTION is assumed." << endl;
		typeSelection = FRACTION;
	}else{
		if(!val.compare("clustering"))
			typeSelection = CLUSTERING;
		else if(!val.compare("fraction"))
			typeSelection = FRACTION;
		else{
			cout << "Option interest-points-detection is not recognized. FRACTION is assumed." << endl;
		}
	}
	
	//parameter-neighborhood
	val = prop->getProperty("parameter-neighborhood");
	if(typeNeighborhood == SPATIAL || typeNeighborhood == ADAPTIVE){
		if(val.empty()){
			cout << "Using SPATIAL or ADAPTIVE: Option parameter-neighborhood was not set. Using 0.01 by default" << endl; 
			fractionDiagonal = 0.01;
		}
		else
			fractionDiagonal = atof(val.c_str());
	}else if(typeNeighborhood == RINGS){
		if(val.empty()){
			cout << "Using RINGS: Option parameter-neighborhood was not set. Using 2 by default" << endl;
			numberRingNeighbor = 2;
		}
		else
			numberRingNeighbor = atoi(val.c_str());
	}
	
	//k
	val = prop->getProperty("K");
	if(val.empty()){
		cout << "Parameter K was not set. Using 0.04 by default" << endl;
		k = 0.04;
	}
	else{
		k = atof(val.c_str());
	}
	
	//numberRingsDetection
	val = prop->getProperty("ring-maxima-detection");
	if(val.empty()){
		cout << "Parameter ring-maxima-detection was not set. Using 1 by default" << endl;
		numberRingsDetection = 1;
	}else{
		numberRingsDetection = atoi(val.c_str());
	}
	
	//parameter-selection
	val = prop->getProperty("parameter-selection");
	if(val.empty()){
		cout << "Parameter parameter-selection was not set. Using 0.01 by default." << endl;
		paramSelection = 0.01;
	}else{
		paramSelection = atof(val.c_str());
	}
}

void HarrisDetector :: showOptions(){
	cout << "type-neighborhood = ";
	switch(typeNeighborhood){
		case SPATIAL: cout << "SPATIAL" << endl;
					  break;
		case ADAPTIVE: cout << "ADAPTIVE" << endl;
						break;
		case RINGS:	cout << "RINGS" << endl;
					break;
	}
	
	cout << "parameter-neighborhood = ";
	if(typeNeighborhood == SPATIAL || typeNeighborhood == ADAPTIVE)
		cout << fractionDiagonal << endl;
	else
		cout << numberRingNeighbor << endl;
	
	cout <<"K = "<< k << endl;
	cout <<"ring-maxima-detection = " << numberRingsDetection << endl;
	
	cout << "interest-points-selection = ";
	switch(typeSelection){
		case FRACTION: cout << "FRACTION" << endl;
						break;
		case CLUSTERING: cout << "CLUSTERING" << endl;
						break;
	}
	
	cout << "parameter-selection = " << paramSelection << endl;
}

bool comp(Vertex i, Vertex j){
	return i.getResponse() > j.getResponse();
}

void HarrisDetector :: detectInterestPoints(vector<Vertex>& interestPoints){
	assert(object);
	
	double max = 0.0;
	int rad;
	int cont = 0;
	double diag = object->getDiagonal();
	
	Clock r;
	//double tiempo = 0.0;
	
	Vertex* vertices = object->getVertices();
	Face* faces = object->getFaces();
	
	//Tree tree;
	
	if(typeNeighborhood == SPATIAL){
/*		vector<Point_3> points;
		for(register int i = 0; i < object->getNumberVertices(); i++){
			points.push_back(Point_3(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ()));
		}
		tree.insert(points.begin(), points.end());
*/	}
		
	//Process each vertex
	cout << "Begin responses calculation..." << endl;
	for(register int i = 0; i< object->getNumberVertices(); i++){
		vector<Vertex*> set;
		
		//r.tick();
		//Calcular radio adaptativo
		if(typeNeighborhood == ADAPTIVE){
			rad = vertices[i].getRadius(vertices, diag * fractionDiagonal, set);
		}
		else if(typeNeighborhood == SPATIAL){
/*
			Point_3 query(vertices[i].getX(), vertices[i].getY(), vertices[i].getZ());
			Fuzzy_sphere fs(query, diag * fractionDiagonal, 0.0);
			
			vector<Point_3> puntos;
			tree.search(back_inserter(puntos), fs);
			
			for(int i = 0; i < puntos.size(); i++){
				Vertex* p = new Vertex(puntos[i].x(), puntos[i].y(), puntos[i].z());
				set.push_back(p);
*/
			
		}else if(typeNeighborhood == RINGS){
			vertices[i].getNeighborhood(numberRingNeighbor, set, vertices);
		}
		//r.tick();
		//tiempo = tiempo + r.obtenerTiempo();
		
		if(set.size() < 6){
			//cout << i << endl;
			cont++;
			vertices[i].setResponse(DBL_MIN);
			continue;
		}
		//Process "set", first, calculate the centroid
		double xc = 0, yc = 0, zc = 0;
		for(register int j = 0; j< set.size();j++){
			xc += set[j]->getX();
			yc += set[j]->getY();
			zc += set[j]->getZ();
		}
		
		xc /= set.size();
		yc /= set.size();
		zc /= set.size();
		
		//Translate the vertex, in order the centroid is in [0 0 0]
		for(register int j = 0; j< set.size();j++){
			set[j]->setX(set[j]->getX() - xc);
			set[j]->setY(set[j]->getY() - yc);
			set[j]->setZ(set[j]->getZ() - zc);
		}

		//Aplicar PCA para encontrar una pose de la nube de puntos de manera que la mayor dispersión de los puntos esté en el plano XY
		//La media de las 3 coordenadas ya es (0, 0, 0), así que en realidad no es necesario calcularla, directamente calculamos la 
		//matriz de covarianza
		double A[9];
		memset(A, 0, sizeof(double)*9);
		
		for(register int j = 0; j < set.size(); j++){
			double x = set[j]->getX();
			double y = set[j]->getY();
			double z = set[j]->getZ();
			
			A[0] += x*x;	A[1] += x*y;	A[2] += x*z;
							A[4] += y*y;	A[5] += y*z;
											A[8] += z*z;
		}
		A[3] = A[1];	A[6] = A[2];	A[7] = A[5];
		for(int j = 0; j < 9; j++)
			A[j] /= (set.size()-1);
		
		//Con la matriz de covarianza, calculamos PCA
		gsl_matrix_view m = gsl_matrix_view_array(A, 3, 3);
		gsl_vector* eval = gsl_vector_alloc(3);
		gsl_matrix* evec = gsl_matrix_alloc(3, 3);
		
		gsl_eigen_symmv_workspace* w = gsl_eigen_symmv_alloc(3);
		gsl_eigen_symmv(&m.matrix, eval, evec, w);
		gsl_eigen_symmv_free(w);
		
		//Ordenar autovectores decrecientemente por autovalor
		gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_DESC);
		
		//Sacamos las componentes del nuevo sistema de coordenadas
		double x_1 = gsl_matrix_get(evec, 0, 0);	double x_2 = gsl_matrix_get(evec, 1, 0); double x_3 = gsl_matrix_get(evec, 2, 0);
		double y_1 = gsl_matrix_get(evec, 0, 1);	double y_2 = gsl_matrix_get(evec, 1, 1); double y_3 = gsl_matrix_get(evec, 2, 1);
		double z_1 = gsl_matrix_get(evec, 0, 2);	double z_2 = gsl_matrix_get(evec, 1, 2); double z_3 = gsl_matrix_get(evec, 2, 2);
		
		
		double x2 = set[0]->getX() - xc;
		double y2 = set[0]->getY() - yc;
		double z2 = set[0]->getZ() - zc;
		
		if((z_1*x2 + z_2*y2 + z_3*z2) < 0){
			z_1 = -z_1;	
			z_2 = -z_2;
			z_3 = -z_3;
			
			double aux_x1 = x_1;
			double aux_x2 = x_2;
			double aux_x3 = x_3;
			
			x_1 = y_1;
			x_2 = y_2;
			x_3 = y_3;
			y_1 = aux_x1;
			y_2 = aux_x2;
			y_3 = aux_x3;
			
		}
		//Realizamos la rotacion, con el nuevo sistema de coordenadas
		for(register int j = 0; j < set.size(); j++){
			double x = set[j]->getX();
			double y = set[j]->getY();
			double z = set[j]->getZ();
			
			set[j]->setX(x_1*x + x_2*y + x_3*z);
			set[j]->setY(y_1*x + y_2*y + y_3*z);
			set[j]->setZ(z_1*x + z_2*y + z_3*z);
		}
		 
		//Movemos todos los puntos para que el punto de analisis se encuentre en el origen del plano XY. Solo movemos en las coordenadas X e Y
		double x = set[0]->getX();
		double y = set[0]->getY();
		
		for(register int j = 0; j < set.size(); j++){
			set[j]->setX(set[j]->getX() - x);
			set[j]->setY(set[j]->getY() - y);
		}
		
		//Fit a quadratic surface
		double C[36];
		double D[6];
		
		memset(C, 0, sizeof(double)*36);
		memset(D, 0, sizeof(double)*6);
		
		for(register int j = 0; j < set.size(); j++){
			double x = set[j]->getX();
			double y = set[j]->getY();
			double z = set[j]->getZ();
			
			double x2 = x*x;
			double y2 = y*y;
			double x3 = x2*x;
			double y3 = y2*y;
			
			C[0] += x*x3;	C[1] += x3*y;	C[2] += x2*y2;	C[3] += x3;		C[4] += x2*y;	C[5] += x2;
							C[7] += x2*y2;	C[8] += x*y3;	C[9] += x2*y;	C[10] += x*y2;	C[11] += x*y;
											C[14] += y*y3;	C[15] += x*y2;	C[16] += y3;	C[17] += y2;
															C[21] += x2;	C[22] += x*y;	C[23] += x;
																			C[28] += y2;	C[29] += y;
			D[0] += z*x2;	D[1] += z*x*y;	D[2] += z*y2;	D[3] += z*x;	D[4] += z*y;	D[5] += z; 
		}
		
		C[6] = C[1];
		C[12] = C[2];	C[13] = C[8];
		C[18] = C[3];	C[19] = C[9];	C[20] = C[15];
		C[24] = C[4];	C[25] = C[10];	C[26] = C[16];	C[27] = C[22];
		C[30] = C[5];	C[31] = C[11];	C[32] = C[17];	C[33] = C[23];	C[34] = C[29];
		
		C[35] = (double)set.size();
		
		//Using GSL for solve linear system
		gsl_matrix_view m1 = gsl_matrix_view_array(C, 6, 6);
		gsl_vector_view b1 = gsl_vector_view_array(D, 6);
		gsl_vector *x1 = gsl_vector_alloc(6);
		
		int s1;
		
		gsl_permutation *p11 = gsl_permutation_alloc(6);
		gsl_linalg_LU_decomp(&m1.matrix, p11, &s1);
		gsl_linalg_LU_solve(&m1.matrix, p11, &b1.vector, x1);
		
		//Extract solution of quadratic surface
		double c20_2 = gsl_vector_get(x1, 0);
		double c11 = gsl_vector_get(x1, 1);
		double c02_2 = gsl_vector_get(x1, 2);
		double c10 = gsl_vector_get(x1, 3);
		double c01 = gsl_vector_get(x1, 4);
		double c0 = gsl_vector_get(x1, 5);
		
		double c20 = c20_2*2;
		double c02 = c02_2*2;
		
		double fx2 = c10*c10 + 2*c20*c20 + 2*c11*c11; //A
		double fy2 = c10*c10 + 2*c11*c11 + 2*c02*c02; //B
		double fxfy = c10*c01 + 2*c20*c11 + 2*c11*c02; //C
		
		//double k = 0.04;
		double resp = fx2*fy2 - fxfy*fxfy - k*(fx2 + fy2)*(fx2 + fy2);
		
		vertices[i].setResponse(resp);
		
		if(resp > max)
			max = resp;
			
		gsl_vector_free(x1);
		gsl_vector_free(eval);
		gsl_matrix_free(evec);
		gsl_permutation_free(p11);
		
		for(register int j = 0; j < set.size(); j++)
			delete set[j];
		set.clear();
	}
	
	cout << cont << " - " << object->getNumberVertices() << endl;
	cout << "Responses calculated..." << endl;
	
	vector<Vertex> candidatePoints;
	
	
	//Search for local maximum 
	for(register int i = 0; i< object->getNumberVertices();i++){
		vertices[i].processMaximum(vertices, numberRingsDetection);
		if(vertices[i].getInterest()){
			candidatePoints.push_back(vertices[i]);
		}
	}
	
	cout << "Candidates calculated ..." << endl;
	
	sort(candidatePoints.begin(), candidatePoints.end(), comp);
	
	if(typeSelection == FRACTION){
		//Seleccionar los puntos de mayor respuesta
		int numPoints = paramSelection * object->getNumberVertices();
		//int numPoints = candidatePoints.size();
		for(register int i = 0; i < numPoints; i++)
			interestPoints.push_back(candidatePoints[i]);
	}else if(typeSelection == CLUSTERING){
		//Aplicar proceso de Clustering
		for(int i = 0; i < candidatePoints.size(); i++){
			bool found = false;
			int j = 0;
			while(j < interestPoints.size() && !found){
				double distX = interestPoints[j].getX() - candidatePoints[i].getX();
				double distY = interestPoints[j].getY() - candidatePoints[i].getY();
				double distZ = interestPoints[j].getZ() - candidatePoints[i].getZ();
			
				if(sqrt(distX*distX + distY*distY + distZ*distZ) < (paramSelection * diag))
					found = true;
				j++;
			}
			if(!found)
				interestPoints.push_back(candidatePoints[i]);
		}
	}
}
