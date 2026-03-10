/**
 * @file modsouvenirs.h
 * @brief Defines the ModSouvenirs class.
 *
 * This window allows administrators to manage souvenirs for
 * each campus. Admins can enable/disable souvenirs, add new
 * souvenirs, change prices, or delete existing souvenirs.
 */

#ifndef MODSOUVENIRS_H
#define MODSOUVENIRS_H

#include <QDialog>
#include <QString>

// Forward declaration for the UI class generated from modsouvenirs.ui
namespace Ui {
class ModSouvenirs;
}

/*
 * Class: ModSouvenirs
 * Purpose: Provides the administrative interface for managing
 *          souvenir items associated with campuses.
 *
 * Administrators can modify which souvenirs are active,
 * as well as add, delete, or update souvenir prices.
 */
class ModSouvenirs : public QDialog
{
    Q_OBJECT

public:

    /*
     * Function: ModSouvenirs constructor
     * Purpose : Initializes the souvenir modification window
     *           and prepares the user interface.
     */
    explicit ModSouvenirs(QWidget *parent = nullptr);

    /*
     * Function: ~ModSouvenirs
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~ModSouvenirs();

    /*
     * Function: setSelectedCampus
     * Purpose : Sets the campus currently being managed and
     *           loads its associated souvenirs.
     */
    void setSelectedCampus(const QString &campus);

private slots:

    /*
     * Function: on_pushButton_clicked
     * Purpose : Returns the user to the previous admin window.
     */
    void on_pushButton_clicked();              // Back

    /*
     * Function: on_cancelButt_clicked
     * Purpose : Resets the UI to its default state without
     *           saving any modifications.
     */
    void on_cancelButt_clicked();              // Reset to default state

    /*
     * Function: on_SubmitButt_clicked
     * Purpose : Confirms the selected college and loads
     *           its souvenirs for modification.
     */
    void on_SubmitButt_clicked();              // Confirm selected college

    /*
     * Function: on_SuvModifybutton_clicked
     * Purpose : Saves the current souvenir checkbox states
     *           to the database.
     */
    void on_SuvModifybutton_clicked();         // Save checkbox states

    /*
     * Function: on_comboBox_currentIndexChanged
     * Purpose : Clears pending UI selections when the
     *           selected campus changes.
     */
    void on_comboBox_currentIndexChanged(int); // Clear pending UI when selection changes

    /*
     * Function: on_addSouvButt_clicked
     * Purpose : Opens the window for adding a new souvenir
     *           to the selected campus.
     */
    void on_addSouvButt_clicked();             // Open add souvenir window

    /*
     * Function: on_changePrButt_clicked
     * Purpose : Opens the window for changing the price
     *           of an existing souvenir.
     */
    void on_changePrButt_clicked();            // Open change price window

    /*
     * Function: on_delSouvButt_clicked
     * Purpose : Opens the window for deleting a souvenir
     *           from the selected campus.
     */
    void on_delSouvButt_clicked();             // Open delete souvenir window

private:

    // Pointer to UI elements generated from modsouvenirs.ui
    Ui::ModSouvenirs *ui;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection is open
     *           before performing database operations.
     */
    bool ensureDbOpen();

    /*
     * Function: ensureSouvenirAccessTable
     * Purpose : Ensures the database table used for managing
     *           souvenir access exists.
     */
    void ensureSouvenirAccessTable();

    /*
     * Function: syncSouvenirsIntoAccessTable
     * Purpose : Synchronizes souvenirs from the main souvenir
     *           list into the admin access table.
     */
    void syncSouvenirsIntoAccessTable();

    /*
     * Function: loadCollegeDropdown
     * Purpose : Loads the campus dropdown menu used to
     *           select which campus souvenirs to modify.
     */
    void loadCollegeDropdown();

    /*
     * Function: clearSouvenirTable
     * Purpose : Clears the souvenir display table before
     *           loading new data.
     */
    void clearSouvenirTable();

    /*
     * Function: loadSouvenirsForCollege
     * Purpose : Loads all souvenirs for the selected campus
     *           into the UI for modification.
     */
    void loadSouvenirsForCollege(const QString &campus);

    /*
     * Function: resetToDefaultState
     * Purpose : Restores the UI to its initial state.
     */
    void resetToDefaultState();

    /*
     * Function: souvenirCountForCampus
     * Purpose : Returns the number of souvenirs associated
     *           with a given campus.
     */
    int souvenirCountForCampus(const QString &campus);

    // Stores the campus currently selected for modification
    QString m_selectedCampus;
};

#endif // MODSOUVENIRS_H
