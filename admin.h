/**
 * @file admin.h
 * @brief Defines the Admin class.
 *
 * The Admin window allows administrators to modify colleges
 * and souvenirs within the application database.
 */

#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>

// Forward declaration for the UI class generated from admin.ui
namespace Ui {
class Admin;
}

/*
 * Class: Admin
 * Purpose: Provides the administrator control panel where
 *          database modification tools can be accessed.
 *
 * Inherits from QDialog so it can function as a separate
 * window that appears on top of the main application.
 */
class Admin : public QDialog
{
    Q_OBJECT

public:
    /*
     * Function: Admin constructor
     * Purpose : Initializes the administrator window and loads
     *           the interface designed in admin.ui.
     */
    explicit Admin(QWidget *parent = nullptr);

    /*
     * Function: ~Admin
     * Purpose : Cleans up UI resources when the admin window closes.
     */
    ~Admin();

private slots:

    /*
     * Function: on_ColMod_clicked
     * Purpose : Opens the college modification window where
     *           administrators can add or edit college information.
     */
    void on_ColMod_clicked();

    /*
     * Function: on_SuvMod_clicked
     * Purpose : Opens the souvenir modification window where
     *           administrators can modify souvenir items or prices.
     */
    void on_SuvMod_clicked();

    /*
     * Function: on_BackButt_clicked
     * Purpose : Returns the user to the previous window or main menu.
     */
    void on_BackButt_clicked();

private:

    // Pointer to the UI elements generated from admin.ui
    Ui::Admin *ui;
};

#endif // ADMIN_H
