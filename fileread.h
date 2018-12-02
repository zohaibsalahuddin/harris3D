#ifndef FILEREAD_H
#define FILEREAD_H

#include "Faces.h"
#include "Vertices.h"

using namespace std;

void skipline(istream &in);
int readVertFace (const string filePath , Faces *& ptrFaces , Vertices *& ptrVertices);

#endif // FILEREAD_H
