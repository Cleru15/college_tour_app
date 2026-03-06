/**
 * @file mainwindow.h
 * @brief Defines the MainWindow class.
 *
 * MainWindow acts as the main hub of the application where the user
 * can preview souvenirs, view campus distances, or start a campus trip.
 */

// mainwindow.h defines the MainWindow class.
// This window acts as the central hub of the application where the user can:
// preview souvenirs, view campus distances, or start a Basic or Custom trip.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Forward declarations for Qt classes used as pointers
class QComboBox;
class QTableView;
class QSqlQueryModel;

/*
 * Class: MainWindow
 * Purpose: Provides the main interface of the application.
 *          It allows users to preview souvenirs, view campus
 *          distances, and launch trip planning windows.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*
     * Constructor
     * Purpose: Initializes the main application window
     *          and builds the primary UI components.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /*
     * Destructor
     * Purpose: Cleans up UI resources when the window closes.
     */
    ~MainWindow();

private slots:

    /*
     * Slot: previewSouvenirButtonClick
     * Purpose: Opens the souvenir preview window for the
     *          selected campus.
     */
    void previewSouvenirButtonClick();

    /*
     * Slot: cancelButtonClick
     * Purpose: Exits the application when the cancel button
     *          is pressed.
     */
    void cancelButtonClick();

    /*
     * Slot: on_submitButtonSouvenirPreview_2_clicked
     * Purpose: Displays campus distances from the selected
     *          starting campus.
     */
    void on_submitButtonSouvenirPreview_2_clicked(); // Show distances

    /*
     * Slot: on_submitButtonSouvenirPreview_3_clicked
     * Purpose: Opens the Basic Trip setup window.
     */
    void on_submitButtonSouvenirPreview_3_clicked(); // Basic Trip

    /*
     * Slot: on_submitButtonSouvenirPreview_4_clicked
     * Purpose: Opens the Custom Trip setup window.
     */
    void on_submitButtonSouvenirPreview_4_clicked(); // Custom Trip

private:

    // Pointer to the UI generated from mainwindow.ui
    Ui::MainWindow *ui;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection exists
     *           and is open before executing queries.
     */
    bool ensureDbOpen();

    /*
     * Function: setupDistanceUi
     * Purpose : Creates the campus distance table view
     *           and connects it to the database model.
     */
    void setupDistanceUi();

    /*
     * Function: populateStartingCampusCombo
     * Purpose : Loads campus names from the database
     *           into the starting campus dropdown.
     */
    void populateStartingCampusCombo();

    /*
     * Function: loadDistancesForCampus
     * Purpose : Queries and displays distances from the
     *           selected campus in the table view.
     */
    void loadDistancesForCampus(const QString &fromCampus);

private:

    // Dropdown used to select the campus for distance queries
    QComboBox *m_fromCampusCombo = nullptr;

    // Table used to display campus distance results
    QTableView *m_distanceTable = nullptr;

    // Model that stores SQL query results for the table view
    QSqlQueryModel *m_distanceModel = nullptr;
};

#endif // MAINWINDOW_H
