#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStateMachine>
#include <QPropertyAnimation>
#include <QEventTransition>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStateMachine *machine = new QStateMachine(this);

    QState *s1 = new QState();
    s1->assignProperty(ui->stateLabel, "text", "Current state: 1");
    s1->assignProperty(ui->pushButton, "geometry", QRect(50, 200, 100, 50));

    QState *s2 = new QState();
    s2->assignProperty(ui->stateLabel, "text", "Current state: 2");
    s2->assignProperty(ui->pushButton, "geometry", QRect(200, 50, 140, 100));

    QPropertyAnimation *animation = new QPropertyAnimation(ui->pushButton, "geometry");
    animation->setEasingCurve(QEasingCurve::OutBounce);

    QEventTransition *t1 = new QEventTransition(ui->changeState, QEvent::MouseButtonPress);
    t1->setTargetState(s2);
    t1->addAnimation(animation);
    s1->addTransition(t1);

    QEventTransition *t2 = new QEventTransition(ui->changeState, QEvent::MouseButtonPress);
    t2->setTargetState(s1);
    t2->addAnimation(animation);
    s2->addTransition(t2);

    machine->addState(s1);
    machine->addState(s2);

    machine->setInitialState(s1);
    machine->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
