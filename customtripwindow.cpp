/**
 * @file customtripwindow.cpp
 * @brief Implements the CustomTripWindow class.
 *
 * Loads campus data and builds a checklist so the user can select
 * specific campuses to include in their trip.
 */

// CustomTripWindow allows the user to create a custom college tour.
// The user selects:
// - a starting campus
// - specific campuses they want to visit
//
// After confirming the starting campus, a checklist of remaining campuses
// is displayed. The user selects which ones to include, and the window
// launches tripWindow using the selected list with the optimal route option.

#include "customtripwindow.h"
#include "ui_customtripwindow.h"

#include "tripwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QTableWidgetItem>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
 * Function: CustomTripWindow constructor
 * Purpose : Initializes the Custom Trip window.
 *           Loads campuses from the database, fills the starting dropdown,
 *           and prepares the checklist table for campus selection.
 */
CustomTripWindow::CustomTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustomTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Custom Trip");

    if (ensureDbOpen())
        m_allCampuses = loadCampuses();
    else
    {
        // Fall back to whatever is already in the dropdown from the .ui
        for (int i = 0; i < ui->selectStartingCollegeDropdownCT->count(); ++i)
            m_allCampuses << ui->selectStartingCollegeDropdownCT->itemText(i);
    }

    // Populate the starting dropdown
    ui->selectStartingCollegeDropdownCT->clear();
    for (const QString &c : m_allCampuses)
        ui->selectStartingCollegeDropdownCT->addItem(c);

    const int idx = ui->selectStartingCollegeDropdownCT->findText("Saddleback College");
    if (idx >= 0)
        ui->selectStartingCollegeDropdownCT->setCurrentIndex(idx);

    // Prep table
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Campuses");
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setRowCount(0);
}

/*
 * Function: ~CustomTripWindow
 * Purpose : Releases memory used by the UI when the window is destroyed.
 */
CustomTripWindow::~CustomTripWindow()
{
    delete ui;
}

/*
 * Function: ensureDbOpen
 * Purpose : Ensures that the SQLite database connection exists and is open.
 *           Searches common directories for college_tour.sqlite if necessary.
 *
 * Returns : true if the database is successfully opened
 *           false if the database cannot be found or opened
 */
bool CustomTripWindow::ensureDbOpen()
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

    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("college_tour.sqlite");
        if (!d.cdUp())
            break;
    }

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
 * Function: loadCampuses
 * Purpose : Retrieves a list of campus names from the database.
 *           Combines campuses appearing as both starting and destination nodes.
 *
 * Returns : A QStringList containing all campus names.
 */
QStringList CustomTripWindow::loadCampuses()
{
    QStringList campuses;

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
        return campuses;

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        if (!c.isEmpty())
            campuses << c;
    }
    return campuses;
}

/*
 * Function: on_confirmButtonCT_clicked
 * Purpose : Locks in the starting campus selected by the user.
 *           Populates the checklist table with the remaining campuses.
 */
void CustomTripWindow::on_confirmButtonCT_clicked()
{
    m_startLocked = ui->selectStartingCollegeDropdownCT->currentText().trimmed();
    if (m_startLocked.isEmpty())
        return;

    ui->selectStartingCollegeDropdownCT->setEnabled(false);
    ui->confirmButtonCT->setEnabled(false);

    QStringList remaining;
    for (const QString &c : m_allCampuses)
    {
        if (c.compare(m_startLocked, Qt::CaseInsensitive) != 0)
            remaining << c;
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(remaining.size());
    ui->tableWidget->setColumnCount(1);

    for (int r = 0; r < remaining.size(); ++r)
    {
        auto *item = new QTableWidgetItem(remaining[r]);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->tableWidget->setItem(r, 0, item);
    }
}

/*
 * Function: on_startTripButtonCT_clicked
 * Purpose : Starts the custom trip when the Start Trip button is clicked.
 *           Collects selected campuses and launches the TripWindow.
 */
void CustomTripWindow::on_startTripButtonCT_clicked()
{
    if (m_startLocked.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
                                 "Click Confirm to lock in your starting college first.");
        return;
    }

    QStringList selected;
    selected << m_startLocked;

    for (int r = 0; r < ui->tableWidget->rowCount(); ++r)
    {
        QTableWidgetItem *item = ui->tableWidget->item(r, 0);
        if (item && item->checkState() == Qt::Checked)
            selected << item->text().trimmed();
    }

    if (selected.size() <= 1)
    {
        QMessageBox::information(this, "No Campuses Selected",
                                 "Select at least one campus to visit in your custom trip.");
        return;
    }

    this->close();

    tripWindow dlg(m_startLocked, selected, selected.size(), /*forceExact=*/true, nullptr);
    dlg.setModal(true);
    dlg.exec();

    if (parentWidget())
        parentWidget()->show();
}
