/**
 * @file admindeletesouvenir.h
 * @brief Defines the AdminDeleteSouvenir class.
 *
 * This window allows administrators to remove existing
 * souvenir items from a selected campus in the database.
 */

#ifndef ADMINDELETESOUVENIR_H
#define ADMINDELETESOUVENIR_H

#include <QWidget>

// Forward declaration for the UI class generated from admindeletesouvenir.ui
namespace Ui {
class AdminDeleteSouvenir;
}

/*
 * Class: AdminDeleteSouvenir
 * Purpose: Provides an interface for administrators to delete
 *          souvenir items associated with a specific campus.
 *
 * Inherits from QWidget so it functions as a separate
 * administrative modification window.
 */
class AdminDeleteSouvenir : public QWidget
{
    Q_OBJECT

public:

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
    void loadSouvenirDropdown();
};

#endif // ADMINDELETESOUVENIR_H
