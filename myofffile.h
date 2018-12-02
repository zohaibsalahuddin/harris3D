#ifndef MYOFFFILE_H
#define MYOFFFILE_H

#include <string>
using namespace std;
class MyOffFile
{
private:
    float **Vertices_Buffer;
    int **Faces_Buffer;
    int POINTS_PER_VERTEX = 3;
    int flag = 0, vertices_index = 0,faces_index = 0;
    int number_of_vertices=0, number_of_faces=0,  number_of_edges=0;
public:
    MyOffFile();
    void readOffFile(string file_name);
    int get_number_of_vertices();
    int get_number_of_faces();
    float** get_Vertices_Buffer();
    int** get_Faces_Buffer();
};

#endif // MYOFFFILE_H
