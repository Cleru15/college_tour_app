/**
 * @file main.cpp
 * @brief Entry point of the Campus Tour application.
 *
 * Creates the Qt application object, opens the login window,
 * and starts the event loop.
 */

#include <QApplication>
#include "login.h"

/*
 * Function: main
 * Purpose : Initializes the application, displays the login
 *           window, and starts the Qt event loop.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
