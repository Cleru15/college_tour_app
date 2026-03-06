/**
 * @file basictripwindow.h
 * @brief Defines the BasicTripWindow class.
 *
 * This window allows the user to configure a basic campus trip by
 * selecting a starting campus and the number of campuses to visit.
 */

// basictripwindow.h defines the BasicTripWindow class.
// This window allows the user to configure a basic campus trip by
// selecting a starting campus and the number of campuses to visit.
// After setup, it launches the TripWindow to simulate the trip.

#ifndef BASICTRIPWINDOW_H
#define BASICTRIPWINDOW_H

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class BasicTripWindow; }
QT_END_NAMESPACE

/*
 * Class: BasicTripWindow
 * Purpose: Provides a UI for creating a basic campus trip.
 *          The user selects a starting campus and how many
 *          campuses to visit before launching the trip window.
 */
class BasicTripWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*
     * Constructor
     * Purpose: Initializes the Basic Trip setup window.
     */
    explicit BasicTripWindow(QWidget *parent = nullptr);

    /*
     * Destructor
     * Purpose: Cleans up UI resources when the window closes.
     */
    ~BasicTripWindow();

private slots:

    /*
     * Slot: on_startTripButtonBT_clicked
     * Purpose: Triggered when the user clicks the Start Trip button.
     *          Collects selected options and launches the trip window.
     */
    void on_startTripButtonBT_clicked();

private:

    // Pointer to the UI generated from basictripwindow.ui
    Ui::BasicTripWindow *ui;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection exists
     *           and is open before querying campus data.
     */
    bool ensureDbOpen();

    /*
     * Function: loadCampusesFromDb
     * Purpose : Queries the database and loads campus names
     *           into the starting campus dropdown menu.
     */
    void loadCampusesFromDb();

    /*
     * Function: rebuildNumDropdown
     * Purpose : Rebuilds the "number of campuses to visit" dropdown
     *           based on how many campuses exist in the database.
     */
    void rebuildNumDropdown(int campusCount);
};

#endif // BASICTRIPWINDOW_H
