<<<<<<< HEAD
#include "basictripwindow.h"
#include "ui_basictripwindow.h"

=======
/**
 * @file basictripwindow.cpp
 * @brief Implements the BasicTripWindow class.
 *
 * This window allows the user to start a basic campus tour
 * by selecting a starting campus and the number of additional
 * campuses to visit.
 */

#include "basictripwindow.h"
#include "ui_basictripwindow.h"

#include "tripwindow.h"
#include "mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
 * Function: BasicTripWindow constructor
 * Purpose : Initializes the basic trip window, loads campuses,
 *           and prepares the trip selection controls.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
BasicTripWindow::BasicTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BasicTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Basic Trip");

    if (ensureDbOpen())
<<<<<<< HEAD
        loadStadiumsFromDb();

    // Number of visitable stadiums after choosing a starting stadium
    rebuildNumDropdown(ui->selectStartingStadiumDropdownBT->count() - 1);

    // No default stadium selected
    ui->selectStartingStadiumDropdownBT->setCurrentIndex(-1);
}

=======
        loadCampusesFromDb();

    rebuildNumDropdown(ui->selectStartingCollegeDropdownBT->count() - 1);
    ui->selectStartingCollegeDropdownBT->setCurrentIndex(-1);
}

/*
 * Function: ~BasicTripWindow
 * Purpose : Cleans up UI resources when the window closes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
BasicTripWindow::~BasicTripWindow()
{
    delete ui;
}

<<<<<<< HEAD
=======
/*
 * Function: ensureDbOpen
 * Purpose : Ensures the SQLite database connection is open
 *           before loading campus information.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
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

<<<<<<< HEAD
    // Look in exe dir and walk upward a few levels (helps when you keep the DB in the project root)
    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("stadium_tour.sqlite");
=======
    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("college_tour.sqlite");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        if (!d.cdUp())
            break;
    }

<<<<<<< HEAD
    // Also try current working directory (Qt Creator often sets this to the build folder)
    candidates << QDir::current().filePath("stadium_tour.sqlite");

QString dbPath;
=======
    candidates << QDir::current().filePath("college_tour.sqlite");

    QString dbPath;
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    for (const QString &p : candidates)
    {
        if (QFileInfo::exists(p))
        {
            dbPath = p;
            break;
        }
    }
<<<<<<< HEAD
=======

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    if (dbPath.isEmpty())
        return false;

    db.setDatabaseName(dbPath);
    return db.open();
}

<<<<<<< HEAD
void BasicTripWindow::loadStadiumsFromDb()
{
    ui->selectStartingStadiumDropdownBT->clear();
=======
/*
 * Function: loadCampusesFromDb
 * Purpose : Loads all enabled campuses from the database
 *           into the starting campus dropdown.
 */
void BasicTripWindow::loadCampusesFromDb()
{
    ui->selectStartingCollegeDropdownBT->clear();
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT campus
        FROM campus_access
        WHERE enabled = 1
            AND TRIM(campus) <> ''
        ORDER BY campus ASC
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
<<<<<<< HEAD
            ui->selectStartingStadiumDropdownBT->addItem(c);
    }
}

void BasicTripWindow::rebuildNumDropdown(int campusCount)
{
    ui->numStadiumstoVisitDropdownBT->clear();

    // campusCount = how many OTHER stadiums can still be visited
    if (campusCount < 0)
        campusCount = 0;

    ui->numStadiumstoVisitDropdownBT->setEnabled(true);

    // Allow visiting zero additional stadiums
    for (int i = 0; i <= campusCount; ++i)
        ui->numStadiumstoVisitDropdownBT->addItem(QString::number(i));
}

void BasicTripWindow::on_startTripButtonBT_clicked()
{
    const QString start = ui->selectStartingStadiumDropdownBT->currentText().trimmed();
    const int maxStops = ui->numStadiumstoVisitDropdownBT->currentText().toInt();
=======
            ui->selectStartingCollegeDropdownBT->addItem(c);
    }
}

/*
 * Function: rebuildNumDropdown
 * Purpose : Builds the dropdown for how many additional
 *           campuses the user wants to visit.
 */
void BasicTripWindow::rebuildNumDropdown(int campusCount)
{
    ui->numCollegestoVisitDropdownBT->clear();

    if (campusCount < 0)
        campusCount = 0;

    ui->numCollegestoVisitDropdownBT->setEnabled(true);

    for (int i = 0; i <= campusCount; ++i)
        ui->numCollegestoVisitDropdownBT->addItem(QString::number(i));
}

/*
 * Function: on_startTripButtonBT_clicked
 * Purpose : Starts the trip using the selected starting campus
 *           and the number of campuses to visit.
 */
void BasicTripWindow::on_startTripButtonBT_clicked()
{
    const QString start = ui->selectStartingCollegeDropdownBT->currentText().trimmed();
    const int maxStops = ui->numCollegestoVisitDropdownBT->currentText().toInt();
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    if (start.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
<<<<<<< HEAD
                                 "Please select a starting stadium.");
        return;
    }

    QStringList stadiums;
    for (int i = 0; i < ui->selectStartingStadiumDropdownBT->count(); ++i)
        stadiums << ui->selectStartingStadiumDropdownBT->itemText(i);

    this->close();

    tripWindow dlg(start, stadiums, maxStops, /*forceExact=*/false, nullptr);
=======
                                 "Please select a starting college.");
        return;
    }

    QStringList campuses;
    for (int i = 0; i < ui->selectStartingCollegeDropdownBT->count(); ++i)
        campuses << ui->selectStartingCollegeDropdownBT->itemText(i);

    this->close();

    tripWindow dlg(start, campuses, maxStops, /*forceExact=*/false, nullptr);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    dlg.setModal(true);
    dlg.exec();

    if (parentWidget())
        parentWidget()->show();
}

<<<<<<< HEAD
=======
/*
 * Function: on_backButtonBT_clicked
 * Purpose : Returns the user to the main window without
 *           starting a trip.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void BasicTripWindow::on_backButtonBT_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}
