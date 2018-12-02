// window.h

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QSlider>
#include <QtDebug>

namespace Ui {
class Window;
}

//instead of building an application with menus, status bar, etc.,
//the main window simply inherits from QWidget

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();



protected:
    //overload for keyboard handling to quit when ESC key is pressed
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_OpenpushButton_clicked();

    void on_DetectpushButton_clicked();

private:
    Ui::Window *ui;
};

#endif // WINDOW_H
