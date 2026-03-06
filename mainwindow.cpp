/**
 * @file mainwindow.cpp
 * @brief Implements the MainWindow class.
 *
 * Handles the main UI logic including loading campus distances,
 * opening souvenir previews, and launching trip planning windows.
 */

// mainwindow.cpp controls the main interface of the Campus Tour application.
// This window acts as the central hub where users can preview souvenirs,
// view campus distances, or start either a Basic Trip or Custom Trip.
//
// It also manages the model/view system used to display campus distance
// data from the SQLite database.

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "newwindow.h"
#include "basictripwindow.h"
#include "customtripwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QHeaderView>
#include <QComboBox>
#include <QTableView>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>

/*
 * Function: MainWindow constructor
 * Purpose : Initializes the main application window.
 *           Builds the UI and sets up the campus distance table.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Campus Tour");

    setupDistanceUi();
}

/*
 * Function: ~MainWindow
 * Purpose : Cleans up dynamically allocated UI resources
 *           when the main window is destroyed.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Function: ensureDbOpen
 * Purpose : Ensures the SQLite database connection exists and is open.
 *           Searches common locations for college_tour.sqlite if needed.
 *
 * Returns : true if the database connection is available
 *           false if the database cannot be found or opened
 */
bool MainWindow::ensureDbOpen()
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
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not find college_tour.sqlite.\n\n"
                              "Put it next to your .exe (build/debug folder),\n"
                              "or in the project folder.");
        return false;
    }

    db.setDatabaseName(dbPath);

    if (!db.open())
    {
        QMessageBox::critical(this, "Database Error",
                              "Failed to open SQLite database:\n" + db.lastError().text());
        return false;
    }

    return true;
}

/*
 * Function: setupDistanceUi
 * Purpose : Builds the campus distance display using Qt's model/view system.
 *           Creates a table view connected to a SQL query model.
 */
void MainWindow::setupDistanceUi()
{
    if (!ensureDbOpen())
        return;

    m_fromCampusCombo = ui->dropdownSouvenirPreview_2;

    if (ui->distanceListView)
        ui->distanceListView->hide();

    const QRect tableRect = ui->distanceListView
                                ? ui->distanceListView->geometry()
                                : QRect(260, 30, 541, 410);

    m_distanceTable = new QTableView(ui->centralwidget);
    m_distanceTable->setGeometry(tableRect);
    m_distanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_distanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_distanceTable->horizontalHeader()->setStretchLastSection(true);

    m_distanceModel = new QSqlQueryModel(m_distanceTable);

    populateStartingCampusCombo();

    if (m_fromCampusCombo && m_fromCampusCombo->count() > 0)
    {
        QString start = m_fromCampusCombo->currentData().toString();
        if (start.isEmpty())
            start = m_fromCampusCombo->currentText();
        loadDistancesForCampus(start);
    }
}

/*
 * Function: populateStartingCampusCombo
 * Purpose : Loads campus names from the database into the dropdown menu.
 *           Ensures all campuses are included by combining both distance columns.
 */
void MainWindow::populateStartingCampusCombo()
{
    if (!m_fromCampusCombo)
        return;

    m_fromCampusCombo->clear();

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
        QMessageBox::critical(this, "Query Error",
                              "Campus list query failed:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        if (!c.isEmpty())
            m_fromCampusCombo->addItem(c, c);
    }

    const int idx = m_fromCampusCombo->findData("Saddleback College");
    if (idx >= 0)
        m_fromCampusCombo->setCurrentIndex(idx);
}

/*
 * Function: loadDistancesForCampus
 * Purpose : Queries the database for all campuses reachable from
 *           the selected starting campus and displays them in the table.
 */
void MainWindow::loadDistancesForCampus(const QString &fromCampus)
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!ensureDbOpen())
        return;

    const QString from = fromCampus.trimmed();
    if (from.isEmpty())
        return;

    QSqlQuery query(QSqlDatabase::database());
    query.prepare(R"(
        SELECT
            TRIM(to_campus) AS campus,
            MIN(miles)      AS miles
        FROM distances
        WHERE TRIM(from_campus) = :from
          AND TRIM(to_campus) <> :from
          AND TRIM(to_campus) <> ''
        GROUP BY TRIM(to_campus)
        ORDER BY miles ASC, campus ASC
    )");
    query.bindValue(":from", from);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Distance query failed:\n" + query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(std::move(query));
    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              "Distance model error:\n" + m_distanceModel->lastError().text());
        return;
    }

    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Campus");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, QString("Miles from %1").arg(from));
    m_distanceTable->setModel(m_distanceModel);
    m_distanceTable->resizeColumnsToContents();
}

/*
 * Function: previewSouvenirButtonClick
 * Purpose : Opens the souvenir preview dialog for the selected campus.
 */
void MainWindow::previewSouvenirButtonClick()
{
    const QString campus = ui->dropdownSouvenirPreview
                               ? ui->dropdownSouvenirPreview->currentText().trimmed()
                               : QString();

    if (campus.isEmpty() || campus.startsWith("Display Souvenirs", Qt::CaseInsensitive))
        return;

    NewWindow dlg(campus, this);
    dlg.setModal(true);
    dlg.exec();
}

/*
 * Function: cancelButtonClick
 * Purpose : Exits the application when the cancel button is pressed.
 */
void MainWindow::cancelButtonClick()
{
    QCoreApplication::quit();
}

/*
 * Function: on_submitButtonSouvenirPreview_2_clicked
 * Purpose : Loads and displays campus distances for the selected starting campus.
 */
void MainWindow::on_submitButtonSouvenirPreview_2_clicked()
{
    if (!m_fromCampusCombo)
        return;

    QString from = m_fromCampusCombo->currentData().toString();
    if (from.isEmpty())
        from = m_fromCampusCombo->currentText();

    if (from.startsWith("Distance", Qt::CaseInsensitive))
        return;

    loadDistancesForCampus(from);
}

/*
 * Function: on_submitButtonSouvenirPreview_3_clicked
 * Purpose : Opens the Basic Trip planning window.
 */
void MainWindow::on_submitButtonSouvenirPreview_3_clicked()
{
    auto *win = new BasicTripWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}

/*
 * Function: on_submitButtonSouvenirPreview_4_clicked
 * Purpose : Opens the Custom Trip planning window.
 */
void MainWindow::on_submitButtonSouvenirPreview_4_clicked()
{
    auto *win = new CustomTripWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}
