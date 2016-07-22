#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTimer>
#include <QOpenGLWindow>

#include <QSurfaceFormat>
#include <QOpenGLFunctions>

#include <QtOpenGL>
#include <GL/glu.h>

#include <QGLWidget>

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QOpenGLContext* context;
    QOpenGLFunctions* openGLFunctions;

    float rotation;

public slots:
    void updateAnimation();

};

#endif // MAINWINDOW_H
