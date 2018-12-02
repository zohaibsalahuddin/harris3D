// main.cpp

#include <QApplication>
#include <QDesktopWidget>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    // When you handle several widgets with stretch factors (potentially with layouts)
    // you have to provide a sizeHint function to your components.

    // see https://www.ics.com/designpatterns/book/sizepolicy.html

    window.resize(window.sizeHint());

    //compute areas to determine if the application should be drawn maximized or not
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    window.setWindowTitle("Harris 3D");


    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    return app.exec();
}
