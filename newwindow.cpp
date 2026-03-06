/**
 * @file newwindow.cpp
 * @brief Implements the NewWindow dialog.
 *
 * Opens a database connection and loads souvenir data for the
 * selected campus into a Qt table view.
 */

// newwindow.cpp displays the souvenir list for a selected campus.
// This dialog queries the SQLite database and shows souvenir items
// and prices in a read-only table view using Qt's model/view system.
//
// The window creates its own database connection so multiple dialogs
// can safely access the database without interfering with each other.

#include "newwindow.h"
#include "ui_newwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

/*
 * Function: NewWindow constructor
 * Purpose : Creates the souvenir dialog for the selected campus.
 *           Configures the UI, opens the database, and loads souvenir data.
 */
NewWindow::NewWindow(const QString &collegeName, QWidget *parent)
    : QDialog(parent), ui(new Ui::NewWindow), m_collegeName(collegeName)
{
    ui->setupUi(this);

    this->setWindowTitle("Souvenirs - " + m_collegeName);
    this->resize(900, 600);

    if (ui->tableView)
    {
        ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    if (!openDb()) {
        return;
    }

    loadSouvenirs();
}

/*
 * Function: ~NewWindow
 * Purpose : Closes the database connection and releases
 *           the dynamically allocated UI resources.
 */
NewWindow::~NewWindow()
{
    closeDb();
    delete ui;
}

/*
 * Function: openDb
 * Purpose : Creates and opens a unique SQLite connection
 *           for this dialog instance.
 *
 * Returns : true if the database is successfully opened
 *           false if the database cannot be found or opened
 */
bool NewWindow::openDb()
{
    m_connName = QString("campus_conn_%1").arg(reinterpret_cast<quintptr>(this));

    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connName);

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
                              "Could not find college_tour.sqlite."
                              "Put it next to your .exe (build/debug folder),"
                              "or in the project folder.");
        return false;
    }

    m_db.setDatabaseName(dbPath);

    if (!m_db.open())
    {
        QMessageBox::critical(this, "Database Error",
                              "Failed to open SQLite database:\n" + m_db.lastError().text());
        return false;
    }

    return true;
}

/*
 * Function: closeDb
 * Purpose : Safely closes the database connection used by this dialog
 *           and removes it from Qt's connection registry.
 */
void NewWindow::closeDb()
{
    if (m_db.isValid())
    {
        if (m_db.isOpen())
            m_db.close();
    }

    QSqlDatabase::removeDatabase(m_connName);
}

/*
 * Function: loadSouvenirs
 * Purpose : Retrieves souvenir items and prices for the selected campus
 *           and displays them in the table view.
 */
void NewWindow::loadSouvenirs()
{
    qDebug() << "loadSouvenirs campus =" << m_collegeName;

    auto *model = new QSqlQueryModel(this);

    QSqlQuery query(m_db);
    query.prepare
        (R"(
        SELECT item, price
        FROM souvenirs
        WHERE TRIM(campus) = :campus
        ORDER BY item
    )");

    query.bindValue(":campus", m_collegeName);

    qDebug() << "Prepared =" << query.lastQuery();

    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Souvenir query failed:\n" + query.lastError().text());
        return;
    }

    model->setQuery(std::move(query));

    if (model->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error","Model error:\n" + model->lastError().text());
        return;
    }

    model->setHeaderData(0, Qt::Horizontal, "Item");
    model->setHeaderData(1, Qt::Horizontal, "Price");

    if (ui->tableView)
    {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    }
}
