<<<<<<< HEAD
=======
/**
 * @file login.cpp
 * @brief Implements the Login class.
 *
 * This file controls the login window of the application.
 * It allows users to clear login fields, check entered passwords,
 * and open either the Admin window or the MainWindow.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#include "login.h"
#include "admin.h"
#include "ui_login.h"
#include "mainwindow.h"
<<<<<<< HEAD

=======
#include <string>

/*
 * Function: Login constructor
 * Purpose : Initializes the Login dialog and loads
 *           the interface created in Qt Designer.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

<<<<<<< HEAD
Login::~Login()
{
    delete ui;
}

void Login::on_CancelButt_clicked()
{
    ui->logUser->clear();
    ui->logPass->clear();
}

void Login::on_LogButton_clicked()
{
    const QString username = ui->logUser->text().trimmed();
    const QString passwordText = ui->logPass->text().trimmed();

    // Admin login data
    const QString adminUsernames[5] =
    {
        "Christian",
        "Ryan",
        "Lila",
        "Dean",
        "Kevin"
    };

    const int adminPasswords[5] =
    {
        111,
        222,
        333,
        444,
        555
    };

    bool passwordIsNumber = false;
    const int password = passwordText.toInt(&passwordIsNumber);

    bool adminLogin = false;

    if (passwordIsNumber)
    {
        for (int i = 0; i < 5; i++)
        {
            if (username == adminUsernames[i] &&
                password == adminPasswords[i])
            {
                adminLogin = true;
                break;
            }
        }
    }

    if (adminLogin)
    {
        auto *win = new Admin(nullptr);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->show();

        this->close();
        return;
    }

    auto *win = new MainWindow(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
=======
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
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
}
