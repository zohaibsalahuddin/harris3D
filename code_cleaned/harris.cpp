/*
	Date Created: 27th December
	FILENAME : harris.cpp
------------------------------------------------
 *
 *	This file contains declarations for functions
 *	that will be required by the open gl interface.
 * 	These function will be used to get 
 *	neighborhood, interest points and everything.
------------------------------------------------
 *	Created By:
 *		Zohaib Salahuddin
 *		Abdullah Thabit
 *		Ahmed Mustafa Ahmed AbdulMajid Gouda
*/

#include "harris.h"

int Faces::numFaces =0;
int Vertices::numVertices =0;

bool double_equals (double a, double b, double epsilon = 0.001)
{

	return (abs(a-b) < epsilon);
}

int cal_interest_points(double ** & result, int & size_result, string filename, double harris_parameter, double fraction, double radius_param, string selection_type,string n_type)
{

	Faces * ptrfaces;
	Vertices * ptrvertices;
	vector <Vertices> nVert;
	int totalfaces =0;
	int totalVertices =0;
	double k = harris_parameter;
	int flag_selection = 0;

	if (selection_type == "fraction")
	{
		flag_selection =0;
		cout << "Selecting Points by Fraction" << endl;
	}
	else if (selection_type == "clustering")
	{
		flag_selection =1;
		cout << "Selecting points by Clustering" << endl;
	}
		

	cout << "Calling the Function to read the Vertex and Faces" << endl;
	int ret = readVertFace(filename,ptrfaces,ptrvertices) ;
	if (ret != 0)
	{
		cout << "File Not Supported. Aborting" << endl;
		return -1;
	}

	totalfaces = Faces::numFaces;
	totalVertices =Vertices::numVertices ;

	set <int> neighbor;
	for (int i =0 ; i < totalVertices; i++)
	{
		//cout << "Vertex: "<< ptrvertices[i].vertx << " " << ptrvertices[i].verty << " " << ptrvertices[i].vertz<< endl;

		if ( n_type == "ring")
		{
			cout << "here" << endl;
			ptrvertices[i].getRingNeighborhood(radius_param,ptrvertices,neighbor);
		}
		else if (n_type == "adaptive")
		{
			ptrvertices[i].getAdaptiveNeighborhood ((radius_param * (get_cluster_diag ( ptrvertices ,totalVertices))), ptrvertices , neighbor, totalVertices);
		}

		set<int> :: iterator it;
		int i1 =0;
		int vertex_index =0;
		for (it = neighbor.begin(); it!= neighbor.end(); it++)
		{	
			if ((*it) == ptrvertices[i].index)
			{
				vertex_index = i1;
			}
			i1++;
			cout << ptrvertices[*it].vertx << " " << ptrvertices[*it].verty << " " << ptrvertices[*it].vertz<< endl;
			nVert.push_back(ptrvertices[*it]);
		}


		
		double centerx,centery,centerz;
		double  p1,p2,p3,p4,p5,p6;
		double response =0;
		if (nVert.size() > 6)
		{
			calculate_center (nVert,centerx,centery,centerz);
			shift_center_to_zero (nVert, centerx, centery,centerz );
	
			MatrixXd * eigen_vectors = pca_calculate(nVert);

			direction_check_shift (nVert, eigen_vectors, vertex_index, centerx,centery,centerz);
 			pca_rotate (nVert, eigen_vectors);

			shift_to_vertex_centerxy(nVert, vertex_index);
		
			quadratic_fit (nVert, p1, p2, p3, p4 , p5 ,p6);
			response = get_harris_response (p1, p2, p3, p4 , p5 ,p6,k);
			//cout << "RESPONSE: "<< response << endl;
			ptrvertices[i].harris_response = response;
		}
		else
		{
			ptrvertices[i].harris_response = DBL_MIN;
		}
		
		nVert.clear();	
		neighbor.clear();
		
		}
		vector <Vertices> interest_points_all;
		vector <Vertices> interest_points_cluster;
		int flag;
		for (int i =0 ; i < totalVertices; i++)
		{
			int flag = (ptrvertices[i].isMaximum(ptrvertices));

			if (flag == 0)
			{
				interest_points_all.push_back(ptrvertices[i]);
			}
		
		}

		sort(interest_points_all.begin(), interest_points_all.end(), response_compare);

		vector<Vertices> :: iterator itv;
		for(itv = interest_points_all.begin(); itv!=interest_points_all.end(); itv++)
		{
				//cout << (*itv).vertx << "  "<< (*itv).verty  << "  "<< (*itv).vertz << " Response: " << (*itv).harris_response  << endl;
				
				
		}
		int points_fraction = fraction * totalVertices;
		
		cout << "size of points: " << interest_points_all.size() << ", Fraction: " << points_fraction << endl;
		//cout << "Interest Points: " << endl;

		if ( flag_selection == 0)
		{
			if ( points_fraction < interest_points_all.size())
			{
				size_result = points_fraction;
			}
			else 
			{
				size_result = interest_points_all.size();
			}		
		}
		else if (flag_selection == 1)
		{
			size_result = interest_points_all.size();
		}
		
		result = new double*[size_result];
		for (int i =0 ; i < size_result ; i++)
		{
			result[i] = new double[3];
						
		}
		float diag = (get_cluster_diag ( ptrvertices ,totalVertices));
		float diag_thresh = (fraction * (get_cluster_diag ( ptrvertices ,totalVertices)));
		cout << flag_selection << endl;
		if (flag_selection == 0)
		{
			for (int i = 0 ; ((i < points_fraction) & (i < interest_points_all.size()));i++)
			{
			
				result[i][0] = interest_points_all[i].vertx;
				result[i][1] = interest_points_all[i].verty;
				result[i][2] = interest_points_all[i].vertz;
			}
		}
		else if (flag_selection == 1)
		{	size_result = 0;
			for(int i = 0; i < (interest_points_all.size() ); i++)
			{
				int flag_temp =0;
				for(int j = 0; j < interest_points_cluster.size(); j++)
				{
					double x_dist = interest_points_cluster[j].vertx - interest_points_all[i].vertx;
					double y_dist = interest_points_cluster[j].verty - interest_points_all[i].verty;
					double z_dist = interest_points_cluster[j].vertz - interest_points_all[i].vertz;

				if(sqrt(x_dist*x_dist + y_dist*y_dist + z_dist*z_dist) < diag_thresh)
				{
					flag_temp = 1;
					break;
				}
				}

				if (flag_temp == 0)
				{
					interest_points_cluster.push_back(interest_points_all[i]);
					result[size_result][0] = interest_points_all[i].vertx;
					result[size_result][1] = interest_points_all[i].verty;
					result[size_result][2] = interest_points_all[i].vertz;
					size_result++;
				}
				
			}
		}
/*
		set<int> :: iterator itn;
		set <int> nice;
		ptrvertices[685].getRingNeighborhood (2, ptrvertices , nice);
		cout << "Here" << endl;
		for (itn = nice.begin(); itn!= nice.end(); itn++)
		{	
		
			cout << ptrvertices[*itn].vertx << endl;

		}	
*/
		neighbor.clear();
		interest_points_cluster.clear();
		interest_points_all.clear();
		delete[] ptrfaces;
		delete[] ptrvertices;
		nVert.clear();
 		Faces::numFaces =0;
 		Vertices::numVertices =0;
		return 0;


}





int get_faces(int ** & result, int * & face, int & size_result, string filename,double radius_param, double x, double y , double z, string n_type)
{
	Faces * ptrfaces;
	Vertices * ptrvertices;
	vector <Vertices> nVert;
	int totalfaces =0;
	int totalVertices =0;
	set <int> neighbor;
	int index_l;
	vector<int> x_temp;
	vector<int> y_temp;
	vector<int> z_temp;
	vector<int> face_no;

	cout << "Calling the Function to read the Vertex and Faces" << endl;
	int ret = readVertFace(filename,ptrfaces,ptrvertices) ;
	if (ret != 0)
	{
		cout << "File Not Supported. Aborting" << endl;
		return -1;
	}

	totalfaces = Faces::numFaces;
	totalVertices =Vertices::numVertices ;
	for (int i =0 ; i < totalVertices ; i++)
	{

			cout << ptrvertices[i].vertx << " " <<  x<< endl;
			cout << ptrvertices[i].verty << " " <<  y<< endl;
			cout << ptrvertices[i].vertz << " " <<  z<< endl;
		if ((double_equals(ptrvertices[i].vertx , x)) && (double_equals(ptrvertices[i].verty , y)) && (double_equals(ptrvertices[i].vertz , z)))
		{
			
			index_l = i;
			break;
		}
	
	}
	cout << "index_l " << index_l << endl;
	if ( n_type == "ring")
	{
		ptrvertices[index_l].getRingNeighborhood(radius_param,ptrvertices,neighbor);
	}
	else if (n_type == "adaptive")
	{
		ptrvertices[index_l].getAdaptiveNeighborhood ((radius_param * (get_cluster_diag ( ptrvertices ,totalVertices))), ptrvertices , neighbor, totalVertices);
	}

	for (int i = 0; i < totalfaces ; i++)
	{
		if ( (neighbor.find(ptrfaces[i].memberVerts[0]) != neighbor.end()) && (neighbor.find(ptrfaces[i].memberVerts[1]) != neighbor.end()) &&(neighbor.find(ptrfaces[i].memberVerts[2]) != neighbor.end()))
		{
			x_temp.push_back(ptrfaces[i].memberVerts[0]);
			y_temp.push_back(ptrfaces[i].memberVerts[1]);
			z_temp.push_back(ptrfaces[i].memberVerts[2]);
			face_no.push_back(i);
		}

	}

	size_result = x_temp.size();
	result = new int*[size_result];
	face = new int[size_result];
	for (int i =0 ; i < size_result ; i++)
	{
		result[i] = new int[3];
						
	}
	
	for ( int i = 0; i < size_result; i++)
	{
		result[i][0] = x_temp[i];
		result[i][1] = y_temp[i];
		result[i][2] = z_temp[i];
		face[i] = face_no[i];		
	}

 	Faces::numFaces =0;
 	Vertices::numVertices =0;

	x_temp.clear();
	y_temp.clear();
	z_temp.clear();
	face_no.clear();
	neighbor.clear();
	delete[] ptrfaces;
	delete[] ptrvertices;

	return 0;
}

