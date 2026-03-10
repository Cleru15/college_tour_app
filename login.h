/**
 * @file login.h
 * @brief Defines the Login class.
 *
 * The Login window allows users to enter credentials
 * before accessing the application. If the password
 * matches an administrator password, the Admin panel
 * is opened. Otherwise the regular MainWindow opens.
 */

#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

// Forward declaration of UI class generated from login.ui
namespace Ui { class Login; }

// Forward declaration of the main application window
class MainWindow;

/*
 * Class: Login
 * Purpose: Provides the login interface for the application.
 *
 * This dialog checks the entered password and determines
 * whether the user should access the Admin interface or
 * the regular application window.
 */
class Login : public QDialog
{
    Q_OBJECT

public:

    /*
     * Function: Login constructor
     * Purpose : Creates the login window and initializes
     *           the user interface.
     */
    explicit Login(QWidget *parent = nullptr);

    /*
     * Function: ~Login
     * Purpose : Cleans up UI resources when the login
     *           window is destroyed.
     */
    ~Login();

private slots:

    /*
     * Function: on_CancelButt_clicked
     * Purpose : Clears the username and password fields
     *           in the login form.
     */
    void on_CancelButt_clicked();

    /*
     * Function: on_LogButton_clicked
     * Purpose : Validates the entered password and
     *           opens either the Admin window or MainWindow.
     */
    void on_LogButton_clicked();

private:

    // Pointer to UI elements generated from login.ui
    Ui::Login *ui = nullptr;

    // Pointer to the main application window for regular users
    MainWindow *m_mainWindow = nullptr;
};

#endif // LOGIN_H
