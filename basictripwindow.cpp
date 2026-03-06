/**
 * @file basictripwindow.cpp
 * @brief Implements the BasicTripWindow class.
 *
 * Loads campus data from the database and launches the trip window
 * using a simple nearest-neighbor route strategy.
 */

// BasicTripWindow collects user input for a "basic trip":
// - pick a starting campus
// - pick how many campuses to visit
// Then it launches tripWindow with the campus list and stop limit.
// This mode uses a greedy nearest-neighbor style route (fast, simple, not always optimal).

#include "basictripwindow.h"
#include "ui_basictripwindow.h"

#include "tripwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
 * Function: BasicTripWindow constructor
 * Purpose : Initializes the Basic Trip window.
 *           Sets up the UI, opens the database, loads campus names,
 *           builds the stop-count dropdown, and sets a default campus.
 */
BasicTripWindow::BasicTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BasicTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Basic Trip");

    if (ensureDbOpen())
        loadCampusesFromDb();

    rebuildNumDropdown(ui->selectStartingCollegeDropdownBT->count());

    // Default selection
    const int idx = ui->selectStartingCollegeDropdownBT->findText("Saddleback College");
    if (idx >= 0)
        ui->selectStartingCollegeDropdownBT->setCurrentIndex(idx);
}

/*
 * Function: ~BasicTripWindow
 * Purpose : Cleans up the dynamically allocated UI object
 *           when the window is destroyed.
 */
BasicTripWindow::~BasicTripWindow()
{
    delete ui;
}

/*
 * Function: ensureDbOpen
 * Purpose : Ensures that the SQLite database connection exists and is open.
 *           Searches common directories for college_tour.sqlite if needed.
 *
 * Returns : true if the database is successfully opened
 *           false if the database file cannot be found or opened
 */
bool BasicTripWindow::ensureDbOpen()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains())
    {
        db = QSqlDatabase::database();
        if (db.isOpen())
            return true;
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    const QString exeDir = QCoreApplication::applicationDirPath();
    QStringList candidates;

    // Look in exe dir and walk upward a few levels (helps when you keep the DB in the project root)
    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("college_tour.sqlite");
        if (!d.cdUp())
            break;
    }

    // Also try current working directory (Qt Creator often sets this to the build folder)
    candidates << QDir::current().filePath("college_tour.sqlite");

    QString dbPath;
    for (const QString &p : candidates)
    {
        if (QFileInfo::exists(p))
        {
            dbPath = p;
            break;
        }
    }

    if (dbPath.isEmpty())
        return false;

    db.setDatabaseName(dbPath);
    return db.open();
}

/*
 * Function: loadCampusesFromDb
 * Purpose : Retrieves campus names from the database and fills
 *           the starting-campus dropdown menu.
 */
void BasicTripWindow::loadCampusesFromDb()
{
    ui->selectStartingCollegeDropdownBT->clear();

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT DISTINCT TRIM(name) AS campus
        FROM (
            SELECT from_campus AS name FROM distances
            UNION
            SELECT to_campus AS name FROM distances
        )
        WHERE TRIM(name) <> ''
        ORDER BY TRIM(name) ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load campus list:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        if (!c.isEmpty())
            ui->selectStartingCollegeDropdownBT->addItem(c);
    }
}

/*
 * Function: rebuildNumDropdown
 * Purpose : Populates the dropdown for the number of colleges to visit.
 *           Limits the choices based on how many campuses are available.
 */
void BasicTripWindow::rebuildNumDropdown(int campusCount)
{
    ui->numCollegestoVisitDropdownBT->clear();

    if (campusCount <= 0)
        campusCount = 1;

    for (int i = 1; i <= campusCount; ++i)
        ui->numCollegestoVisitDropdownBT->addItem(QString::number(i));
}

/*
 * Function: on_startTripButtonBT_clicked
 * Purpose : Starts the trip when the user clicks the Start Trip button.
 *           Collects the selected values and opens the trip window.
 */
void BasicTripWindow::on_startTripButtonBT_clicked()
{
    const QString start = ui->selectStartingCollegeDropdownBT->currentText().trimmed();
    const int maxStops = ui->numCollegestoVisitDropdownBT->currentText().toInt();

    if (start.isEmpty())
        return;

    QStringList campuses;
    for (int i = 0; i < ui->selectStartingCollegeDropdownBT->count(); ++i)
        campuses << ui->selectStartingCollegeDropdownBT->itemText(i);

    // Close this setup window so TripWindow is the only one open.
    this->close();

    tripWindow dlg(start, campuses, maxStops, /*forceExact=*/false, nullptr);
    dlg.setModal(true);
    dlg.exec();

    // Return to main window after trip finishes.
    if (parentWidget())
        parentWidget()->show();
}
