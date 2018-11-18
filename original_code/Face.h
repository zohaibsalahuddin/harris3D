#ifndef __FACE_H_
#define __FACE_H_

#include <vector>

using namespace std;

          class Face{
                private:
                        vector<int> vertices;
                public:
                       Face(){}
                       void addVertex(int vertex){ vertices.push_back(vertex);}
					   vector<int>& getVertices(){ return vertices;}
				};

#endif
