/**
 * @file mainwindow.cpp
 * @brief Implements the MainWindow class.
 *
 * This window serves as the main hub of the application.
 * It allows users to preview souvenirs, view campus distances,
 * start trips, or return to the login screen.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"

#include "basictripwindow.h"
#include "customtripwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QHeaderView>
#include <QComboBox>
#include <QTableView>
#include <QRect>
#include <QAbstractItemView>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

/*
 * Function: MainWindow constructor
 * Purpose : Initializes the main window, opens the database,
 *           and prepares the campus/souvenir display UI.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ensureDbOpen();
    setupDistanceUi();
    populateSouvenirCampusCombo();
    populateStartingCampusCombo();

    m_distanceModel->setQuery("SELECT '' AS col1, '' AS col2 WHERE 1=0", m_db);
    m_distanceModel->setHeaderData(0, Qt::Horizontal, "");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "");

    resizeDistanceTableColumns();
}

/*
 * Function: ~MainWindow
 * Purpose : Closes the database connection and cleans up
 *           UI resources when the main window closes.
 */
MainWindow::~MainWindow()
{
    const QString connName = m_db.connectionName();

    if (m_db.isValid() && m_db.isOpen())
        m_db.close();

    m_db = QSqlDatabase();
    QSqlDatabase::removeDatabase(connName);

    delete ui;
}

/*
 * Function: resizeDistanceTableColumns
 * Purpose : Adjusts the distance table column widths when
 *           the window size changes.
 */
void MainWindow::resizeDistanceTableColumns()
{
    if (!m_distanceTable || !m_distanceTable->model())
        return;

    const int totalWidth = m_distanceTable->viewport()->width();
    if (totalWidth <= 0)
        return;

    const int firstColWidth  = static_cast<int>(totalWidth * 0.75);
    const int secondColWidth = totalWidth - firstColWidth;

    m_distanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_distanceTable->setColumnWidth(0, firstColWidth);
    m_distanceTable->setColumnWidth(1, secondColWidth);
}

/*
 * Function: resizeEvent
 * Purpose : Handles window resize events and updates
 *           the distance table layout.
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    resizeDistanceTableColumns();
}

/*
 * Function: ensureDbOpen
 * Purpose : Opens the SQLite database connection used by
 *           the main window if it is not already open.
 */
bool MainWindow::ensureDbOpen()
{
    const QString connName = QString("main_conn_%1")
    .arg(reinterpret_cast<quintptr>(this));

    if (QSqlDatabase::contains(connName))
        m_db = QSqlDatabase::database(connName);
    else
        m_db = QSqlDatabase::addDatabase("QSQLITE", connName);

    if (!m_db.isValid())
    {
        QMessageBox::critical(this, "Database Error",
                              "QSQLITE driver is not available.");
        return false;
    }

    if (m_db.isOpen())
        return true;

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
            dbPath = QFileInfo(p).absoluteFilePath();
            break;
        }
    }

    if (dbPath.isEmpty())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not find college_tour.sqlite.");
        return false;
    }

    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open SQLite database:\n" +
                                  m_db.lastError().text() +
                                  "\n\nPath tried:\n" + dbPath);
        return false;
    }

    return true;
}

/*
 * Function: setupDistanceUi
 * Purpose : Creates and configures the distance display table
 *           used to show campus-to-campus distances.
 */
void MainWindow::setupDistanceUi()
{
    if (!ensureDbOpen())
        return;

    m_fromCampusCombo = ui->dropdownDistances;

    QRect tableRect(260, 30, 541, 410);
    if (ui->distanceListView)
    {
        tableRect = ui->distanceListView->geometry();
        ui->distanceListView->hide();
    }

    m_distanceTable = new QTableView(ui->centralwidget);
    m_distanceTable->setGeometry(tableRect);
    m_distanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_distanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_distanceTable->horizontalHeader()->setStretchLastSection(false);

    m_distanceModel = new QSqlQueryModel(m_distanceTable);
    m_distanceTable->setModel(m_distanceModel);

    populateStartingCampusCombo();

    if (m_fromCampusCombo && m_fromCampusCombo->count() > 1)
    {
        QString start = m_fromCampusCombo->currentData().toString();
        if (start.isEmpty())
            start = m_fromCampusCombo->currentText();

        if (!start.isEmpty() &&
            start.compare("Select a College", Qt::CaseInsensitive) != 0)
        {
            loadDistancesForCampus(start);
        }
    }
}

/*
 * Function: populateStartingCampusCombo
 * Purpose : Loads enabled campuses into the distance
 *           selection dropdown.
 */
void MainWindow::populateStartingCampusCombo()
{
    if (!m_fromCampusCombo)
        return;

    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    m_fromCampusCombo->clear();
    m_fromCampusCombo->addItem("Select a College");

    QSqlQuery q(m_db);

    const QString sql = R"(
        SELECT TRIM(campus)
        FROM campus_access
        WHERE enabled = 1
          AND TRIM(campus) <> ''
        ORDER BY TRIM(campus) ASC
    )";

    if (!q.exec(sql))
    {
        QMessageBox::critical(this, "Query Error",
                              "Campus list query failed:\n" +
                                  q.lastError().text() +
                                  "\n\nDB: " + m_db.databaseName() +
                                  "\nSQL:\n" + sql);
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
 * Purpose : Loads and displays the distances from the
 *           selected campus to other enabled campuses.
 */
void MainWindow::loadDistancesForCampus(const QString &fromCampus)
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    const QString from = fromCampus.trimmed();
    if (from.isEmpty() ||
        from.compare("Select a College", Qt::CaseInsensitive) == 0)
        return;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT
            TRIM(d.to_campus) AS campus,
            MIN(d.miles)      AS miles
        FROM distances d
        JOIN campus_access ca
          ON TRIM(ca.campus) = TRIM(d.to_campus)
        WHERE TRIM(d.from_campus) = :from
          AND TRIM(d.to_campus) <> :from
          AND TRIM(d.to_campus) <> ''
          AND ca.enabled = 1
        GROUP BY TRIM(d.to_campus)
        ORDER BY miles ASC, campus ASC
    )");
    query.bindValue(":from", from);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Distance query failed:\n" + query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              "Distance model error:\n" + m_distanceModel->lastError().text());
        return;
    }

    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Campus");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Distance");

    resizeDistanceTableColumns();
}

/*
 * Function: loadSouvenirsForCampus
 * Purpose : Loads and displays the souvenirs available
 *           for the selected campus.
 */
void MainWindow::loadSouvenirsForCampus(const QString &campus)
{
    if (!m_distanceTable || !m_distanceModel)
        return;
    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    const QString selectedCampus = campus.trimmed();
    if (selectedCampus.isEmpty() ||
        selectedCampus.compare("Select a College", Qt::CaseInsensitive) == 0)
        return;

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT
            TRIM(s.item) AS souvenir,
            s.price      AS price
        FROM souvenirs s
        LEFT JOIN souvenir_access sa
          ON TRIM(sa.campus) = TRIM(s.campus)
         AND TRIM(sa.item)   = TRIM(s.item)
        WHERE TRIM(s.campus) = :campus
          AND (sa.enabled = 1 OR sa.enabled IS NULL)
          AND TRIM(s.item) <> ''
        ORDER BY TRIM(s.item) ASC
    )");
    query.bindValue(":campus", selectedCampus);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Souvenir query failed:\n" + query.lastError().text());
        return;
    }

    m_distanceModel->setQuery(query);

    if (m_distanceModel->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error",
                              "Souvenir model error:\n" + m_distanceModel->lastError().text());
        return;
    }

    m_distanceModel->setHeaderData(0, Qt::Horizontal, "Souvenir");
    m_distanceModel->setHeaderData(1, Qt::Horizontal, "Price");

    resizeDistanceTableColumns();
}

/*
 * Function: previewSouvenirButtonClick
 * Purpose : Displays the souvenirs for the selected campus
 *           in the table view.
 */
void MainWindow::previewSouvenirButtonClick()
{
    const QString campus = ui->dropdownSouvenirPreview->currentText().trimmed();

    if (campus.isEmpty() ||
        campus.compare("Select a College", Qt::CaseInsensitive) == 0)
    {
        m_distanceModel->setQuery("SELECT '' AS col1, '' AS col2 WHERE 1=0", m_db);
        m_distanceModel->setHeaderData(0, Qt::Horizontal, "");
        m_distanceModel->setHeaderData(1, Qt::Horizontal, "");
        return;
    }

    loadSouvenirsForCampus(campus);
}

/*
 * Function: cancelButtonClick
 * Purpose : Closes the application.
 */
void MainWindow::cancelButtonClick()
{
    QCoreApplication::quit();
}

/*
 * Function: on_buttonDistancesSubmit_clicked
 * Purpose : Displays distances for the campus selected
 *           in the distances dropdown.
 */
void MainWindow::on_buttonDistancesSubmit_clicked()
{
    const QString campus = ui->dropdownDistances->currentText().trimmed();

    if (campus.isEmpty() ||
        campus.compare("Select a College", Qt::CaseInsensitive) == 0)
    {
        m_distanceModel->setQuery("SELECT '' AS col1, '' AS col2 WHERE 1=0", m_db);
        m_distanceModel->setHeaderData(0, Qt::Horizontal, "");
        m_distanceModel->setHeaderData(1, Qt::Horizontal, "");
        return;
    }

    loadDistancesForCampus(campus);
}

/*
 * Function: on_buttonBasicTrip_clicked
 * Purpose : Opens the basic trip planning window.
 */
void MainWindow::on_buttonBasicTrip_clicked()
{
    auto *win = new BasicTripWindow(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}

/*
 * Function: on_buttonCustomTrip_clicked
 * Purpose : Opens the custom trip planning window.
 */
void MainWindow::on_buttonCustomTrip_clicked()
{
    auto *win = new CustomTripWindow(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}

/*
 * Function: on_pushButton_clicked
 * Purpose : Returns the user to the login window.
 */
void MainWindow::on_pushButton_clicked()
{
    auto *win = new Login(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}

/*
 * Function: populateSouvenirCampusCombo
 * Purpose : Loads enabled campuses into the souvenir
 *           preview dropdown menu.
 */
void MainWindow::populateSouvenirCampusCombo()
{
    if (!ui->dropdownSouvenirPreview)
        return;

    ui->dropdownSouvenirPreview->clear();
    ui->dropdownSouvenirPreview->addItem("Select a College");

    if (!m_db.isOpen() && !ensureDbOpen())
        return;

    QSqlQuery q(m_db);

    const QString sql = R"(
        SELECT TRIM(campus)
        FROM campus_access
        WHERE enabled = 1
          AND TRIM(campus) <> ''
        ORDER BY TRIM(campus) ASC
    )";

    if (!q.exec(sql))
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load enabled colleges:\n" +
                                 q.lastError().text() +
                                 "\n\nDB: " + m_db.databaseName() +
                                 "\nSQL:\n" + sql);
        return;
    }

    while (q.next())
    {
        const QString campus = q.value(0).toString().trimmed();
        if (!campus.isEmpty())
            ui->dropdownSouvenirPreview->addItem(campus);
    }
}
