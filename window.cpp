// window.cpp

#include  <QMessageBox>
#include <QtWidgets>
#include "window.h"
#include "ui_window.h"

#include "myglwidget.h"

#include <iostream>
#include <cfloat>
#include <vector>
#include "faces.h"
#include "vertices.h"
#include "fileread.h"
#include "pca.h"

using namespace std;
using namespace Eigen;

int Faces::numFaces =0;
int Vertices::numVertices =0;
QString filename;

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    ui->NTcomboBox->addItem("rings");
    ui->NTcomboBox->addItem("spatial");
    ui->NTcomboBox->addItem("adaptive");
    ui->IPScomboBox->addItem("fraction");
    ui->IPScomboBox->addItem("clustering");

    // manual slot/signal connection example.
    // http://doc.qt.io/archives/qt-4.8/signalsandslots.html



    connect(
                ui->myGLWidget,                 // sender object
                SIGNAL(xRotationChanged(int)),  // sent signal carrying an int
                ui->rotXSlider,                 // receiver
                SLOT(setValue(int)));           // slot function executed


    //same applies for the other 2 sliders
    connect(ui->myGLWidget, SIGNAL(yRotationChanged(int)), ui->rotYSlider, SLOT(setValue(int)));
    connect(ui->myGLWidget, SIGNAL(zRotationChanged(int)), ui->rotZSlider, SLOT(setValue(int)));

}

Window::~Window()
{
    delete ui;
}

//overload of the keyPressEvent function for the window, as seen in first QT Labs
void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::on_OpenpushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open File"),"C://", "All files (*.*);;Text File (*.off)");
    QMessageBox::information(this, tr("File Name"), filename);

}

void Window::on_DetectpushButton_clicked()
{
    /*
    QString parameters;
    parameters = QString("Neighborhood type: %1\nInterest points selection: %2\n"
                         "Diagonal fraction: %3\nRings number: %4\nHarris parameter: %5\n"
                         "Ring maxima detection: %6\nSelection paramter: %7").arg(ui->NTcomboBox->currentText())
            .arg(ui->IPScomboBox->currentText()).arg(ui->DFlineEdit->text()).arg(ui->RNlineEdit->text())
            .arg(ui->HPlineEdit->text()).arg(ui->RMDlineEdit->text()).arg(ui->SPlineEdit->text());
    QMessageBox::information(this,"Detection parameters",parameters);
    */

        double k = (ui->HPlineEdit->text()).toDouble();

        double fraction  = (ui->SPlineEdit->text()).toDouble();
        int radius_param =(ui->RNlineEdit->text()).toDouble();
        string filepath = filename.toUtf8().constData();

        Faces * ptrfaces;
        Vertices * ptrvertices;
        vector <Vertices> nVert;
        int totalfaces =0;
        int totalVertices =0;


        cout << "Calling the Function to read the Vertex and Faces" << endl;
        readVertFace(filepath,ptrfaces,ptrvertices);
        totalfaces = Faces::numFaces;
        totalVertices =Vertices::numVertices ;/*
        cout << "Printing the Face Information:" << endl;
        cout << "Total Number of Faces:" << totalfaces << endl;
        for (int i =0 ; i < totalfaces; i++)
        {
            cout << ptrfaces[i].memberVerts[0] << " " << ptrfaces[i].memberVerts[1] << " " << ptrfaces[i].memberVerts[2]<< endl;;
        }

        cout << "Printing the Face Information:" << endl;
        cout << "Total Number of Vertices:" << totalVertices << endl;
        for (int i =0 ; i < totalVertices; i++)
        {
            cout << ptrvertices[i].vertx << " " << ptrvertices[i].verty << " " << ptrvertices[i].vertz<< endl;

            cout << "Adjacent Vertices for Vertex no. " << i << endl;
            set<int> :: iterator it;
            for(it = ptrvertices[i].adjacentVert.begin(); it!=ptrvertices[i].adjacentVert.end(); it++)
            {
                    cout << *it << "  ";

            }
            cout << endl;

            cout << " Faces part of Vertex no. " << i << endl;

            for(it = ptrvertices[i].facesIncVert.begin(); it!=ptrvertices[i].facesIncVert.end(); it++)
            {
                    cout << *it << "  ";

            }

            cout << endl;
        }*/
        set <int> neighbor;
        //cout << "The Neighborhood of all the vertices are:" << endl;
        for (int i =0 ; i < totalVertices; i++)
        {

            ptrvertices[i].getRingNeighborhood(radius_param,ptrvertices,neighbor);
            set<int> :: iterator it;
            int i1 =0;
            int vertex_index =0;
            //cout << "Vertex Neighborhood: "<< ptrvertices[i].vertx << " " << ptrvertices[i].verty << " " << ptrvertices[i].vertz<< endl;
            for (it = neighbor.begin(); it!= neighbor.end(); it++)
            {
                if ((*it) == ptrvertices[i].index)
                {
                    vertex_index = i1;
                }
                i1++;
                nVert.push_back(ptrvertices[*it]);
                //cout << ptrvertices[*it].vertx << " " << ptrvertices[*it].verty << " " << ptrvertices[*it].vertz<< endl;
            }



            double centerx,centery,centerz;
            double  p1,p2,p3,p4,p5,p6;
            double response =0;
            if (nVert.size() > 6)
            {
                calculate_center (nVert,centerx,centery,centerz);
                shift_center_to_zero (nVert, centerx, centery,centerz );
                //cout << centerx << "," << centery << "," << centerz<<endl;
                //cout << "MATRIX STARTS HERE" << endl;

                MatrixXd * eigen_vectors = pca_calculate(nVert);
                //(*eigen_vectors).transposeInPlace();
                //cout << (*eigen_vectors) << endl;
                //cout << "MATRIX ENDS HERE" << endl;
                direction_check_shift (nVert, eigen_vectors, vertex_index, centerx,centery,centerz);
                pca_rotate (nVert, eigen_vectors);

            /*cout << "Before VERTEX: " << vertex_index << endl;
            vector<Vertices> :: iterator itv;
            for(itv = nVert.begin(); itv!=nVert.end(); itv++)
            {
                    cout << (*itv).vertx << "  "<< (*itv).verty  << "  "<< (*itv).vertz  << endl;

            }*/
                //cout << "Vertex Number" << vertex_index << endl;
                shift_to_vertex_centerxy(nVert, vertex_index);

            /*cout << "VERTEX: " << vertex_index << endl;
            for(itv = nVert.begin(); itv!=nVert.end(); itv++)
            {
                    cout << (*itv).vertx << "  "<< (*itv).verty  << "  "<< (*itv).vertz  << endl;

            }
            */
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

            int points_fraction = fraction * totalVertices;

            cout << "size of points: " << interest_points_all.size() << ", Fraction: " << points_fraction << endl;
            cout << "Interest Points: " << endl;

            for (int i = 0 ; ((i < points_fraction) & (i < interest_points_all.size()));i++)
            {
                cout << interest_points_all[i].vertx << " "<< interest_points_all[i].verty << " "<< interest_points_all[i].vertz << endl;
            }


}
