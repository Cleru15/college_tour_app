/**
 * @file customtripwindow.h
 * @brief Defines the CustomTripWindow class.
 *
 * Allows the user to select a starting campus and manually choose
 * which campuses to include in a custom trip.
 */

// customtripwindow.h defines the CustomTripWindow class.
// This window allows the user to create a custom campus tour
// by selecting a starting campus and choosing specific campuses
// they want to visit during the trip.

#ifndef CUSTOMTRIPWINDOW_H
#define CUSTOMTRIPWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CustomTripWindow; }
QT_END_NAMESPACE

/*
 * Class: CustomTripWindow
 * Purpose: Provides a UI for building a custom campus trip.
 *          The user selects a starting campus and chooses
 *          which campuses they want to visit.
 */
class CustomTripWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*
     * Constructor
     * Purpose: Initializes the Custom Trip setup window.
     */
    explicit CustomTripWindow(QWidget *parent = nullptr);

    /*
     * Destructor
     * Purpose: Cleans up UI resources when the window closes.
     */
    ~CustomTripWindow();

private slots:

    /*
     * Slot: on_confirmButtonCT_clicked
     * Purpose: Locks in the starting campus and populates
     *          the checklist of campuses the user can select.
     */
    void on_confirmButtonCT_clicked();

    /*
     * Slot: on_startTripButtonCT_clicked
     * Purpose: Starts the trip using the campuses selected
     *          by the user in the checklist table.
     */
    void on_startTripButtonCT_clicked();

private:

    // Pointer to the UI generated from customtripwindow.ui
    Ui::CustomTripWindow *ui;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection exists
     *           and is open before querying campus data.
     */
    bool ensureDbOpen();

    /*
     * Function: loadCampuses
     * Purpose : Retrieves a list of campus names from the
     *           database to populate the dropdown menu.
     */
    QStringList loadCampuses();

    // Stores the starting campus once the user confirms their selection
    QString m_startLocked;

    // Stores the full list of available campuses loaded from the database
    QStringList m_allCampuses;
};

#endif // CUSTOMTRIPWINDOW_H
