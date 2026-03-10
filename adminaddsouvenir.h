/**
 * @file adminaddsouvenir.h
 * @brief Defines the adminAddSouvenir class.
 *
 * This window allows administrators to add new souvenir items
 * for a selected campus into the application database.
 */

#ifndef ADMINADDSOUVENIR_H
#define ADMINADDSOUVENIR_H

#include <QWidget>
#include <QString>

// Forward declaration for the UI class generated from adminaddsouvenir.ui
namespace Ui {
class adminAddSouvenir;
}

/*
 * Class: adminAddSouvenir
 * Purpose: Provides an interface for administrators to add
 *          new souvenir items to a selected campus.
 *
 * Inherits from QWidget so it functions as a standalone
 * window within the admin modification workflow.
 */
class adminAddSouvenir : public QWidget
{
    Q_OBJECT

public:

    /*
     * Function: adminAddSouvenir constructor
     * Purpose : Initializes the window and stores the campus name
     *           where the new souvenir will be added.
     */
    explicit adminAddSouvenir(const QString &campus, QWidget *parent = nullptr);

    /*
     * Function: ~adminAddSouvenir
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~adminAddSouvenir();

private slots:

    /*
     * Function: on_addSouvConfirm_clicked
     * Purpose : Inserts the new souvenir item and price
     *           into the database for the selected campus.
     */
    void on_addSouvConfirm_clicked();

    /*
     * Function: on_addSouvBack_clicked
     * Purpose : Returns the user to the previous admin menu
     *           without adding a new souvenir.
     */
    void on_addSouvBack_clicked();

private:

    // Pointer to the UI elements generated from adminaddsouvenir.ui
    Ui::adminAddSouvenir *ui;

    // Stores the campus name that the souvenir will be added to
    QString m_campus;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection is open
     *           before performing any database operations.
     */
    bool ensureDbOpen();
};

#endif // ADMINADDSOUVENIR_H
