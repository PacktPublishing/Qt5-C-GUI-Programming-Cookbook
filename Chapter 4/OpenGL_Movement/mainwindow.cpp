#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{
    setSurfaceType(QWindow::OpenGLSurface);

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2, 1); // OpenGL 2.1
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);

    openGLFunctions = context->functions();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    timer->start(100);

    rotation = 0;

    moveX = 0;
    moveZ = 0;
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
    {
        moveZ -= 0.2;
    }

    if (event->key() == Qt::Key_S)
    {
        moveZ += 0.2;
    }

    if (event->key() == Qt::Key_A)
    {
        moveX -= 0.2;
    }

    if (event->key() == Qt::Key_D)
    {
        moveX += 0.2;
    }
}

void MainWindow::initializeGL()
{
    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    QImage image("bricks");
    QImage texture = QGLWidget::convertToGLFormat(image);

    glGenTextures(1, &texID[0]);
    glBindTexture(GL_TEXTURE_2D, texID[0]);

    // Point sampling of nearest neightbour
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Bilinear interpolation
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Trilinear interpolation
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

    // Enable smooth shading
    glShadeModel(GL_SMOOTH);

    // Lighting
    glEnable(GL_LIGHT1);
    GLfloat lightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat lightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[]= { 3.0f, 3.0f, -5.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

    // Make sure render at the correct aspect ratio
    resizeGL(this->width(), this->height());
}

void MainWindow::resizeGL(int w, int h)
{
    // Set the viewport
    glViewport(0, 0, w, h);

    qreal aspectRatio = qreal(w) / qreal(h);

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(75, aspectRatio, 0.1, 400000000);

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::paintGL()
{
    // Initialize clear color (cornflower blue)
    glClearColor(0.39f, 0.58f, 0.93f, 1.f);

    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Transformations
    glTranslatef(0.0, 0.0, -3.0);

    // Keyboard movement
    glTranslatef(moveX, 0.0, moveZ);

    // Texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texID[0]);

    glEnable(GL_LIGHTING);

    // FRONT
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, -0.5, -0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, 0.5, -0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    // BACK
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 0.0f,-1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, 0.5);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    // RIGHT
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, -0.5, -0.5);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, -0.5, 0.5);
    glEnd();

    // LEFT
    glBegin(GL_POLYGON);
        glNormal3f(0.0f,-1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, -0.5, 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, 0.5);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 0.5, -0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    // TOP
    glBegin(GL_POLYGON);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, -0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, -0.5);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.5, 0.5, 0.5);
    glEnd();

    // Red side - BOTTOM
    glBegin(GL_POLYGON);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, -0.5, -0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, -0.5, 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, -0.5, -0.5);
    glEnd();

    glDisable(GL_LIGHTING);

    // Disable texturing once you're done
    glDisable(GL_TEXTURE_2D);

    glFlush();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paintGL();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(), this->height());
    this->update();
}

void MainWindow::updateAnimation()
{
    rotation += 10;
    this->update();
}
