/**
 * @file main.cpp
 * @brief Entry point of the Campus Tour application.
 *
 * Initializes the Qt application, creates the main window,
 * and starts the Qt event loop.
 */

// main.cpp is the entry point of the application.
// It initializes the Qt framework, creates the main window,
// and starts the Qt event loop so the program can respond to user actions.

#include "mainwindow.h"
#include "newwindow.h"

#include <QApplication>

/*
 * Function: main
 * Purpose : Entry point of the program. Initializes the QApplication,
 *           creates the MainWindow, displays it, and starts the Qt
 *           event loop which keeps the GUI running.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
