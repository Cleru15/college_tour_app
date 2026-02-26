#include "mainwindow.h"
#include "newwindow.h"

#include <QApplication>

// I GUESS BRO - BRANDON HO

// Creates the Qt app, shows the main window, and starts the event loop.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
