/**
 * @file mainwindow.h
 * @brief Defines the MainWindow class.
 *
 * The MainWindow acts as the main hub of the application.
 * From this window the user can preview souvenirs, view
 * campus distances, start a basic trip, or create a custom trip.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QResizeEvent>

// Forward declaration for the UI class generated from mainwindow.ui
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Forward declarations to reduce compile dependencies
class QComboBox;
class QTableView;
class QSqlQueryModel;

/*
 * Class: MainWindow
 * Purpose: Serves as the central interface of the application.
 *
 * Users can navigate to different features such as viewing
 * campus distances, previewing souvenirs, or launching
 * trip planning windows.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*
     * Function: MainWindow constructor
     * Purpose : Initializes the main window and prepares
     *           all UI components and database connections.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /*
     * Function: ~MainWindow
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~MainWindow();

private slots:

    /*
     * Function: previewSouvenirButtonClick
     * Purpose : Opens the souvenir preview window for
     *           the selected campus.
     */
    void previewSouvenirButtonClick();

    /*
     * Function: cancelButtonClick
     * Purpose : Closes the application when the cancel
     *           button is pressed.
     */
    void cancelButtonClick();

    /*
     * Function: on_buttonDistancesSubmit_clicked
     * Purpose : Displays the distances from the selected
     *           campus to other campuses.
     */
    void on_buttonDistancesSubmit_clicked();

    /*
     * Function: on_buttonBasicTrip_clicked
     * Purpose : Opens the Basic Trip planning window.
     */
    void on_buttonBasicTrip_clicked();

    /*
     * Function: on_buttonCustomTrip_clicked
     * Purpose : Opens the Custom Trip planning window.
     */
    void on_buttonCustomTrip_clicked();

    /*
     * Function: on_pushButton_clicked
     * Purpose : Handles additional button interactions
     *           defined in the UI.
     */
    void on_pushButton_clicked();

protected:

    /*
     * Function: resizeEvent
     * Purpose : Handles window resize events to adjust
     *           table column sizes dynamically.
     */
    void resizeEvent(QResizeEvent *event) override;

private:

    // Pointer to UI elements generated from mainwindow.ui
    Ui::MainWindow *ui = nullptr;

    // SQLite database connection used by the application
    QSqlDatabase m_db;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection
     *           is open before running queries.
     */
    bool ensureDbOpen();

    /*
     * Function: populateSouvenirCampusCombo
     * Purpose : Loads the campus list into the souvenir
     *           preview dropdown menu.
     */
    void populateSouvenirCampusCombo();

    /*
     * Function: setupDistanceUi
     * Purpose : Initializes the distance display table
     *           used for showing campus-to-campus distances.
     */
    void setupDistanceUi();

    /*
     * Function: populateStartingCampusCombo
     * Purpose : Loads the campus dropdown used for
     *           selecting a starting campus.
     */
    void populateStartingCampusCombo();

    /*
     * Function: loadDistancesForCampus
     * Purpose : Queries the database and displays distances
     *           from the selected campus to other campuses.
     */
    void loadDistancesForCampus(const QString &fromCampus);

    /*
     * Function: loadSouvenirsForCampus
     * Purpose : Retrieves and displays souvenirs for
     *           the selected campus.
     */
    void loadSouvenirsForCampus(const QString &campus);

    /*
     * Function: resizeDistanceTableColumns
     * Purpose : Adjusts the distance table columns when
     *           the window is resized.
     */
    void resizeDistanceTableColumns();

private:

    // Dropdown used to select the starting campus for distance queries
    QComboBox *m_fromCampusCombo = nullptr;

    // Table view used to display campus distances
    QTableView *m_distanceTable = nullptr;

    // Model storing query results for the distance table
    QSqlQueryModel *m_distanceModel = nullptr;
};

#endif // MAINWINDOW_H
