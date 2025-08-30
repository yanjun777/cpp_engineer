#include "MainWindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    button = new QPushButton("Hello Qt5", this);
    button->setGeometry(50, 50, 120, 40);
}

MainWindow::~MainWindow() {}
