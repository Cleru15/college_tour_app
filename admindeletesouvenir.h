<<<<<<< HEAD
=======
/**
 * @file admindeletesouvenir.h
 * @brief Defines the AdminDeleteSouvenir class.
 *
 * This window allows administrators to remove existing
 * souvenir items from a selected campus in the database.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#ifndef ADMINDELETESOUVENIR_H
#define ADMINDELETESOUVENIR_H

#include <QWidget>
<<<<<<< HEAD
#include <QString>

=======

// Forward declaration for the UI class generated from admindeletesouvenir.ui
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
namespace Ui {
class AdminDeleteSouvenir;
}

<<<<<<< HEAD
=======
/*
 * Class: AdminDeleteSouvenir
 * Purpose: Provides an interface for administrators to delete
 *          souvenir items associated with a specific campus.
 *
 * Inherits from QWidget so it functions as a separate
 * administrative modification window.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
class AdminDeleteSouvenir : public QWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit AdminDeleteSouvenir(const QString &stadium, QWidget *parent = nullptr);
    ~AdminDeleteSouvenir();

private slots:
    void on_delSouvConfirm_clicked();
    void on_delSouvBack_clicked();

private:
    Ui::AdminDeleteSouvenir *ui;

    QString m_stadium;

    bool ensureDbOpen();
=======

    /*
     * Function: AdminDeleteSouvenir constructor
     * Purpose : Initializes the delete souvenir window and stores
     *           the campus whose souvenirs may be removed.
     */
    explicit AdminDeleteSouvenir(const QString &campus, QWidget *parent = nullptr);

    /*
     * Function: ~AdminDeleteSouvenir
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~AdminDeleteSouvenir();

private slots:

    /*
     * Function: on_delSouvConfirm_clicked
     * Purpose : Deletes the selected souvenir item from the database
     *           for the chosen campus.
     */
    void on_delSouvConfirm_clicked();

    /*
     * Function: on_delSouvBack_clicked
     * Purpose : Returns the user to the previous admin window
     *           without deleting a souvenir.
     */
    void on_delSouvBack_clicked();

private:

    // Pointer to UI elements generated from admindeletesouvenir.ui
    Ui::AdminDeleteSouvenir *ui;

    // Stores the campus whose souvenirs are being managed
    QString m_campus;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection is open
     *           before performing any database operations.
     */
    bool ensureDbOpen();

    /*
     * Function: loadSouvenirDropdown
     * Purpose : Loads the list of available souvenirs for the
     *           selected campus into the dropdown menu.
     */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    void loadSouvenirDropdown();
};

#endif // ADMINDELETESOUVENIR_H
