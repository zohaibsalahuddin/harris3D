#include <iostream>
#include <cmath>
#include "util.h"
#include <stdio.h>

using namespace std;

void myPause(){
	cout<<"Press any key to continue...";
	getchar();
}

void myPause(const char* message){
	cout<<message<<endl;
	myPause();
}

int sgn(double num){
	if(num >= 0.0)
		return 1;
	else
		return -1;
}

void skipline(istream &in){
	char c;
	while(in>>noskipws>>c && c!='\n');
	in>>skipws;
}

double distanceL2(Vertex* v1, Vertex* v2){
	double diffx = v1->getX() - v2->getX();
	double diffy = v1->getY() - v2->getY();
	double diffz = v1->getZ() - v2->getZ();
	return sqrt(diffx*diffx + diffy*diffy + diffz*diffz);
}
