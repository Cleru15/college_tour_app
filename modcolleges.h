/**
 * @file modcolleges.h
 * @brief Defines the ModColleges class.
 *
 * This window allows administrators to modify which campuses
 * are available in the system. Administrators can enable or
 * disable campuses using a checklist interface.
 */

#ifndef MODCOLLEGES_H
#define MODCOLLEGES_H

#include <QDialog>

// Forward declaration for the UI class generated from modcolleges.ui
namespace Ui {
class ModColleges;
}

/*
 * Class: ModColleges
 * Purpose: Provides the administrative interface for modifying
 *          the list of active campuses in the database.
 *
 * Administrators can toggle campuses on or off and save the
 * updated configuration.
 */
class ModColleges : public QDialog
{
    Q_OBJECT

public:

    /*
     * Function: ModColleges constructor
     * Purpose : Initializes the window and loads the list
     *           of campuses into the checklist interface.
     */
    explicit ModColleges(QWidget *parent = nullptr);

    /*
     * Function: ~ModColleges
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~ModColleges();

private slots:

    /*
     * Function: on_pushButton_clicked
     * Purpose : Returns the user to the previous admin window.
     */
    void on_pushButton_clicked();          // Back

    /*
     * Function: on_ColModifybutton_clicked
     * Purpose : Saves the current checkbox states and updates
     *           the campus access table in the database.
     */
    void on_ColModifybutton_clicked();     // Save checkbox states

private:

    // Pointer to UI elements generated from modcolleges.ui
    Ui::ModColleges *ui;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection is open
     *           before performing database operations.
     */
    bool ensureDbOpen();

    /*
     * Function: ensureCampusAccessTable
     * Purpose : Ensures the campus access table exists in the
     *           database for storing campus availability states.
     */
    void ensureCampusAccessTable();

    /*
     * Function: syncCampusesIntoAccessTable
     * Purpose : Synchronizes campuses from the main campus list
     *           into the access table used for admin control.
     */
    void syncCampusesIntoAccessTable();

    /*
     * Function: loadCampusChecklist
     * Purpose : Loads the list of campuses into the checklist
     *           so administrators can enable or disable them.
     */
    void loadCampusChecklist();
};

#endif // MODCOLLEGES_H
