<<<<<<< HEAD
=======
/**
 * @file adminaddsouvenir.h
 * @brief Defines the adminAddSouvenir class.
 *
 * This window allows administrators to add new souvenir items
 * for a selected campus into the application database.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#ifndef ADMINADDSOUVENIR_H
#define ADMINADDSOUVENIR_H

#include <QWidget>
#include <QString>

<<<<<<< HEAD
=======
// Forward declaration for the UI class generated from adminaddsouvenir.ui
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
namespace Ui {
class adminAddSouvenir;
}

<<<<<<< HEAD
=======
/*
 * Class: adminAddSouvenir
 * Purpose: Provides an interface for administrators to add
 *          new souvenir items to a selected campus.
 *
 * Inherits from QWidget so it functions as a standalone
 * window within the admin modification workflow.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
class adminAddSouvenir : public QWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit adminAddSouvenir(const QString &stadium, QWidget *parent = nullptr);
    ~adminAddSouvenir();

private slots:
    void on_addSouvConfirm_clicked();
    void on_addSouvBack_clicked();

private:
    Ui::adminAddSouvenir *ui;

    QString m_stadium;

=======

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
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    bool ensureDbOpen();
};

#endif // ADMINADDSOUVENIR_H
