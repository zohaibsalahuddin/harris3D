#ifndef FACES_H
#define FACES_H

#include <iostream>
#include <vector>

using namespace std;

class Faces
{
    public:
// Since our implementation is only for triangular faces
        static int numFaces;
        int memberVerts[3];
        Faces()
        {
            numFaces++;
        }
        void addFaceInfo(int vertno1, int vertno2, int vertno3)
        {

            memberVerts[0] = vertno1;
            memberVerts[1] = vertno2;
            memberVerts[2] = vertno3;

        }

};

#endif // FACES_H
