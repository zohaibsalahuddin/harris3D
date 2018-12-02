// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>

#include "myofffile.h"
#include "myglwidget.h"


MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    zRot = 0;
    MyOffFile off_file;
    off_file.readOffFile("C:\\Users\\Ahmed\\Documents\\CPP\\lab6\\MyOpenGL5\\octahedron.off");
    read_Faces_Buffer = off_file.get_Faces_Buffer();
    read_Vertices_Buffer = off_file.get_Vertices_Buffer();
    read_number_of_faces = off_file.get_number_of_faces();
    read_number_of_vertices = off_file.get_number_of_vertices();
}

MyGLWidget::~MyGLWidget()
{
}

//minimumSizeHint and sizeHint functions are needed when resizing the window
QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

// from the source code available at https://www.bogotobogo.com/
// the angle is increase or decreased every 16 pixels move, to allow for more convenient rotation
// this is why we see this hard-coded 16 value here and there
// a trackball approach using quaternion would be more elegant...

static void qNormalizeAngle(int &angle)
{
    // urgl, it kind of hurts, but it works ;)
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

//implementation of the 3 slots to catch rotation values from sliders and update the openglwidget
void MyGLWidget::setXRotation(int angle)
{
    //we assert the rotation value makes sense
    qNormalizeAngle(angle);

    if (angle != xRot) {//if rotation has changed
        xRot = angle; // update value
        emit xRotationChanged(angle); // emit a signal that carries the angular value
        updateGL(); // draw the scene
    }
}

// same goes for the Y and Z axis
void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::initializeGL()
{
    //init background color
    qglClearColor(Qt::black);

    //enable depth test
    glEnable(GL_DEPTH_TEST);

    //enable face culling, see https://learnopengl.com/Advanced-OpenGL/Face-culling
    glEnable(GL_CULL_FACE);

    //enable smooth (linear color interpolation)
    glShadeModel(GL_SMOOTH);

    //we need at least 1 light to see something
    glEnable(GL_LIGHTING);
    //light0 is the default one
    glEnable(GL_LIGHT0);

    //we now set the light position
    static GLfloat lightPosition[4] = { 0, 0, 10, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

//paintGL is called whenever the GLwidget needs to be drawn.
//it first inits the transforms then calls the draw() member function
void MyGLWidget::paintGL()
{
    //first, clear everything
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //reset any transform
    glLoadIdentity();

    //translate to see the object. Comment that ligne to see the changes
    glTranslatef(0.0, 0.0, -10.0);

    //apply rotations along x, y, then z axis
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    draw();
}

//resizeGL is used when the GLwidget is resized
void MyGLWidget::resizeGL(int width, int height)
{

    //first, we need to adjust the viewport,
    //see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glViewport.xml
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    //glMatrixMode is used to activate the world in which some transforms will be applied
    //https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml

    glMatrixMode(GL_PROJECTION); //basically, we are working in the rendered image, in pixels
    glLoadIdentity();

    //Hmmm, QT might use various opengl emulators, so glOrtho might be replaced by glOrthof
#ifdef QT_OPENGL_ES_1
    glOrthof(-2, +2, -2, +2, 1.0, 15.0);
#else
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
#endif

    //we are done with the positionning and stretching of the rendered image,
    //we then reset the matrix mode to model view,
    //so that the rendered objet is rotated whenever we move the rotation cursors
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void MyGLWidget::draw()
{
    //qglColor(Qt::red);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0;i<read_number_of_faces;i++){
        //
               glColor3f(0.5f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);

            glVertex3f(read_Vertices_Buffer[read_Faces_Buffer[i][0]][0],read_Vertices_Buffer[read_Faces_Buffer[i][0]][1],read_Vertices_Buffer[read_Faces_Buffer[i][0]][2]);
            glVertex3f(read_Vertices_Buffer[read_Faces_Buffer[i][1]][0],read_Vertices_Buffer[read_Faces_Buffer[i][1]][1],read_Vertices_Buffer[read_Faces_Buffer[i][1]][2]);
            glVertex3f(read_Vertices_Buffer[read_Faces_Buffer[i][2]][0],read_Vertices_Buffer[read_Faces_Buffer[i][2]][1],read_Vertices_Buffer[read_Faces_Buffer[i][2]][2]);
        glEnd();
    }


    /*
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);

    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    */
}
