#include "myofffile.h"
#include <QtWidgets>
#include <QtDebug>
#include <fstream>




MyOffFile::MyOffFile()
{

}

void MyOffFile::readOffFile(string filename)
{
    string line;
    ifstream objFile(filename);
    //ifstream objFile("C:\\Users\\Ahmed\\Documents\\CPP\\lab6\\MyOpenGL5\\octahedron.off");
    if(!objFile.is_open()){
        QMessageBox::information(0,"info","Cannot open the file");
    }
    else{
        qDebug()<<"Load Successfully\n";
        while (!objFile.eof()){
            getline (objFile,line);
            //qDebug()<< line.c_str();
            if ((line.c_str()[0] == 'O')&&(line.c_str()[1] == 'F')&&(line.c_str()[2] == 'F')){
                flag = 1;
            }
            else if(flag == 1){
                sscanf(line.c_str(),"%d %d %d",&number_of_vertices,&number_of_faces,&number_of_edges);
                //qDebug()<< number_of_vertices <<endl;
                //qDebug()<< number_of_faces <<endl;
                //qDebug()<< number_of_edges <<endl;
                Vertices_Buffer = new float*[number_of_vertices];
                for(int i=0;i<number_of_vertices;i++){
                    Vertices_Buffer[i] = new float[3];
                }
                Faces_Buffer = new int*[number_of_faces];
                for(int i=0;i<number_of_faces;i++){
                    Faces_Buffer[i] = new int[3];
                }
                flag = 2;
            }
            else if(flag == 2){

                if(vertices_index < number_of_vertices){
                    sscanf(line.c_str(),"%f %f %f",&Vertices_Buffer[vertices_index][0],&Vertices_Buffer[vertices_index][1],&Vertices_Buffer[vertices_index][2]);
                    vertices_index++;
                }
                else
                    flag = 3;
            }
            if(flag == 3){

                if(faces_index < number_of_faces){
                    line[0] = ' ';
                    sscanf(line.c_str(),"%d %d %d",&Faces_Buffer[faces_index][0],&Faces_Buffer[faces_index][1],&Faces_Buffer[faces_index][2]);
                    faces_index++;
                }
                else{
                    flag = 4;
                }
            }

        }
    }
}

int MyOffFile::get_number_of_vertices()
{
    return number_of_vertices;
}

int MyOffFile::get_number_of_faces()
{
    return number_of_faces;
}

float **MyOffFile::get_Vertices_Buffer()
{
    return Vertices_Buffer;
}

int **MyOffFile::get_Faces_Buffer()
{
    return Faces_Buffer;
}
