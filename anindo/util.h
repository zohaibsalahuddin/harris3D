#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>
#include "Vertex.h"
using namespace std;

void myPause();
void myPause(const char* message);
int sgn(double num);
void skipline(istream &in);
double distanceL2(Vertex* v1, Vertex* v2);
#endif
