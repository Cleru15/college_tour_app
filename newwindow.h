/**
 * @file newwindow.h
 * @brief Defines the NewWindow dialog class.
 *
 * Displays souvenir items and prices for a selected campus using
 * a read-only table connected to the SQLite database.
 */

// newwindow.h defines the NewWindow dialog class.
// This dialog displays the souvenirs and prices for a selected campus.
// It queries the SQLite database and shows the results in a read-only table.

#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class NewWindow; }
QT_END_NAMESPACE

/*
 * Class: NewWindow
 * Purpose: Displays souvenir items and prices for a selected campus.
 *          This window opens its own database connection and uses
 *          a model/view table to show the results.
 */
class NewWindow : public QDialog
{
    Q_OBJECT

public:

    /*
     * Constructor
     * Purpose: Creates the souvenir dialog for a specific campus
     *          and loads its souvenir items from the database.
     */
    explicit NewWindow(const QString &collegeName, QWidget *parent = nullptr);

    /*
     * Destructor
     * Purpose: Cleans up the UI and ensures the database
     *          connection is properly closed and removed.
     */
    ~NewWindow();

private:

    // Pointer to the UI generated from newwindow.ui
    Ui::NewWindow *ui;

    // Stores the campus name used to filter souvenir results
    QString m_collegeName;

    // SQLite database connection used by this dialog
    QSqlDatabase m_db;

    // Unique connection name so multiple dialogs don't conflict
    QString m_connName;

    /*
     * Function: openDb
     * Purpose : Opens a unique SQLite database connection
     *           for this dialog instance.
     */
    bool openDb();

    /*
     * Function: closeDb
     * Purpose : Closes the dialog's database connection
     *           and removes it from Qt's connection registry.
     */
    void closeDb();

    /*
     * Function: loadSouvenirs
     * Purpose : Queries the database and loads souvenir
     *           items and prices for the selected campus.
     */
    void loadSouvenirs();
};

#endif // NEWWINDOW_H
