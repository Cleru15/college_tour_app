/**
 * @file basictripwindow.h
 * @brief Defines the BasicTripWindow class.
 *
 * This window allows the user to create a basic campus tour.
 * The user selects the number of campuses to visit and the
 * application calculates a route starting from Saddleback College.
 */

#ifndef BASICTRIPWINDOW_H
#define BASICTRIPWINDOW_H

#include <QMainWindow>
#include <vector>

// Forward declaration for the UI class generated from basictripwindow.ui
QT_BEGIN_NAMESPACE
namespace Ui { class BasicTripWindow; }
QT_END_NAMESPACE

/*
 * Class: BasicTripWindow
 * Purpose: Provides the interface for starting a predefined
 *          campus tour beginning at Saddleback College.
 *
 * The window loads available campuses from the database and
 * allows the user to choose how many campuses to visit.
 */
class BasicTripWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*
     * Function: BasicTripWindow constructor
     * Purpose : Initializes the window and loads campus
     *           information from the database.
     */
    explicit BasicTripWindow(QWidget *parent = nullptr);

    /*
     * Function: ~BasicTripWindow
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~BasicTripWindow();

private slots:

    /*
     * Function: on_startTripButtonBT_clicked
     * Purpose : Starts the basic campus tour using the
     *           selected number of campuses.
     */
    void on_startTripButtonBT_clicked();

    /*
     * Function: on_backButtonBT_clicked
     * Purpose : Returns the user to the previous window
     *           without starting the trip.
     */
    void on_backButtonBT_clicked();

private:

    // Pointer to UI elements generated from basictripwindow.ui
    Ui::BasicTripWindow *ui;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection
     *           is open before performing queries.
     */
    bool ensureDbOpen();

    /*
     * Function: loadCampusesFromDb
     * Purpose : Retrieves the list of available campuses
     *           from the database.
     */
    void loadCampusesFromDb();

    /*
     * Function: rebuildNumDropdown
     * Purpose : Updates the dropdown menu that allows the
     *           user to choose how many campuses to visit.
     */
    void rebuildNumDropdown(int campusCount);
};

#endif // BASICTRIPWINDOW_H
