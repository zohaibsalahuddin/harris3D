#ifndef VERTICES_H
#define VERTICES_H

#include <iostream>
#include <vector>
#include <set>
#include "Faces.h"

using namespace std;
class Vertices
{
    public:
        static int numVertices;
        Vertices()
        {
            numVertices++;
        }
        int marked=0;
        int index=-1;
        double harris_response;
        set <int> facesIncVert;
        set <int> adjacentVert;
        double vertx,verty,vertz;
        void setValues ( double x , double y , double z)
        {
            vertx = x;
            verty = y;
            vertz = z;
        }
        void addFace(int face)
        {
            facesIncVert.insert(face);
        }
        void addAdjacentVert(int vert1,int vert2)
        {
            adjacentVert.insert(vert1);
            adjacentVert.insert(vert2);
        }

        void getRingNeighborhood (int level_depth, Vertices * & vertex_in , set <int> & neighborhood)
        {
            set<int>  temp_vert;
            set<int>  temp_vert2;
            //cout << "In Neighborhood Function" << endl;
            for (int i =0 ; i <= level_depth; i++)
            {
                if ( i ==0)
                {
                    neighborhood.insert(index);
                    marked = 1;
                    temp_vert.insert(adjacentVert.begin(),adjacentVert.end());
                    neighborhood.insert(adjacentVert.begin(),adjacentVert.end());

                }
                else
                {	//cout << "Point" << endl;
                    set<int> :: iterator it;
                    for(it = temp_vert.begin(); it!=temp_vert.end();it++ )
                    {	//cout << "Investigating Vertex No. "  << *it << endl;


                        //cout << "Marked Status" << vertex_in[*current].marked  << endl;
                        if (vertex_in[*it].marked == 0)
                        {
                            temp_vert2.insert(vertex_in[*it].adjacentVert.begin(),
                                     vertex_in[*it].adjacentVert.end());
                            neighborhood.insert(vertex_in[*it].adjacentVert.begin(),
                                     vertex_in[*it].adjacentVert.end());
                                vertex_in[*it].marked = 1;
                        }
                    }
                        temp_vert.clear();
                        temp_vert.insert(temp_vert2.begin(),temp_vert2.end());
                        temp_vert2.clear();
                }
            }

            for (int i =0 ; i < Vertices::numVertices; i++)
            {
                vertex_in[i].marked =0;
            }

        }


        int isMaximum(Vertices * & vertex_in)
        {
            set<int> :: iterator it;
            for(it = adjacentVert.begin(); it!=adjacentVert.end();it++ )
            {
                //cout << harris_response << " " << vertex_in[*it].harris_response << endl;
                if (harris_response < vertex_in[*it].harris_response)
                {
                    //cout << "Response False" << endl;
                    return -1;
                }
            }
            //cout << "Response True" << endl;
            return 0;
        }
};
#endif // VERTICES_H
