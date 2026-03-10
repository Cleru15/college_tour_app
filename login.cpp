/**
 * @file login.cpp
 * @brief Implements the Login class.
 *
 * This file controls the login window of the application.
 * It allows users to clear login fields, check entered passwords,
 * and open either the Admin window or the MainWindow.
 */

#include "login.h"
#include "admin.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <string>

/*
 * Function: Login constructor
 * Purpose : Initializes the Login dialog and loads
 *           the interface created in Qt Designer.
 */
Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

/*
 * Function: ~Login
 * Purpose : Cleans up resources when the Login
 *           window is destroyed.
 */
Login::~Login(){}

/*
 * Function: on_CancelButt_clicked
 * Purpose : Clears the username and password fields
 *           so the user can re-enter credentials.
 */
void Login::on_CancelButt_clicked()
{
    ui->logUser->setText("");
    ui->logPass->setText("");
}

/*
 * Function: on_LogButton_clicked
 * Purpose : Checks the entered password and opens
 *           either the Admin window or MainWindow.
 */
void Login::on_LogButton_clicked()
{
    const QString password = ui->logPass->text();

    QVector<QString> validPasswords =
        {
            "brandonho123",
            "cmetoyer456",
            "Nissan247xs&",
            "jdoe101",
            "admin2024"
        };

    bool isAdminLogin = false;

    for (int i = 0; i < validPasswords.size(); ++i)
    {
        if (password == validPasswords[i])
        {
            isAdminLogin = true;
            break;
        }
    }

    if (isAdminLogin)
    {
        auto *win = new Admin(this);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->show();
        this->hide();
    }
    else
    {
        if (!m_mainWindow)
        {
            m_mainWindow = new MainWindow();
            m_mainWindow->setAttribute(Qt::WA_DeleteOnClose);

            connect(m_mainWindow, &QObject::destroyed, this, [this]() {
                this->close();
            });
        }

        m_mainWindow->show();
        this->close();
    }
}
