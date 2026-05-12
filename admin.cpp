<<<<<<< HEAD
#include "admin.h"
#include "ui_admin.h"
#include "modstadiums.h"
#include "modsouvenirs.h"
#include "login.h"

=======
/**
 * @file admin.cpp
 * @brief Implements the Admin class.
 *
 * This window serves as the administrator menu. From here,
 * the user can open the college modification window, the
 * souvenir modification window, or return to the login screen.
 */

#include "admin.h"
#include "ui_admin.h"
#include "modcolleges.h"
#include "modsouvenirs.h"
#include "login.h"

/*
 * Function: Admin constructor
 * Purpose : Initializes the admin menu window and loads
 *           the interface from the UI file.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
Admin::Admin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Admin)
{
    ui->setupUi(this);
}

<<<<<<< HEAD
=======
/*
 * Function: ~Admin
 * Purpose : Cleans up UI resources when the admin window closes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
Admin::~Admin()
{
    delete ui;
}

<<<<<<< HEAD
void Admin::on_StadMod_clicked()
{
    auto *win = new ModStadiums(this);
=======
/*
 * Function: on_ColMod_clicked
 * Purpose : Opens the college modification window and hides
 *           the current admin menu.
 */
void Admin::on_ColMod_clicked()
{
    auto *win = new ModColleges(this);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}

<<<<<<< HEAD


=======
/*
 * Function: on_SuvMod_clicked
 * Purpose : Opens the souvenir modification window and hides
 *           the current admin menu.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void Admin::on_SuvMod_clicked()
{
    auto *win = new ModSouvenirs(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}

<<<<<<< HEAD




void Admin::on_BackButt_clicked()
{

        auto *win = new Login(this);
        win->setAttribute(Qt::WA_DeleteOnClose);
        win->show();
        this->hide();
}

=======
/*
 * Function: on_BackButt_clicked
 * Purpose : Returns the user to the login window and hides
 *           the current admin menu.
 */
void Admin::on_BackButt_clicked()
{
    auto *win = new Login(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
