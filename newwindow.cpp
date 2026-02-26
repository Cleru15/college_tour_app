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

// Builds the souvenirs dialog UI, opens the DB, and loads souvenirs for the chosen college
NewWindow::NewWindow(const QString &collegeName, QWidget *parent)
    : QDialog(parent), ui(new Ui::NewWindow), m_collegeName(collegeName)
{
    ui->setupUi(this);

    // Configure the dialog title/size for a consistent user experience
    this->setWindowTitle("Souvenirs - " + m_collegeName);
    this->resize(900, 600);

    // If table view exists, configure it for read-only row-based display
    if (ui->tableView)
    {
        ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

    // If the database cannot be opened, stop before attempting to query souvenirs.
    if (!openDb()) {
        return;
    }

    loadSouvenirs();
}

// Closes and removes database connection, then deletes the generated UI object
NewWindow::~NewWindow()
{
    closeDb();
    delete ui;
}

// Opens a unique SQLite connection for this dialog so multiple dialogs don't conflict
bool NewWindow::openDb()
{
    // Generate a per-instance connection name to avoid Qt reusing the wrong connection
    m_connName = QString("campus_conn_%1").arg(reinterpret_cast<quintptr>(this));

    m_db = QSqlDatabase::addDatabase("QSQLITE", m_connName);

    // Try common locations (exe dir, parent dir, current dir)
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
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not find college_tour.sqlite."
                              "Put it next to your .exe (build/debug folder),"
                              "or in the project folder.");
        return false;
    }

    m_db.setDatabaseName(dbPath);

    // If SQLite fails to open, show error text and prevent querying
    if (!m_db.open())
    {
        QMessageBox::critical(this, "Database Error",
                              "Failed to open SQLite database:\n" + m_db.lastError().text());
        return false;
    }

    return true;
}

// Closes dialog's database connection and removes it from Qt's connection registry
void NewWindow::closeDb()
{
    // If the connection object is valid, close it safely if it is open
    if (m_db.isValid())
    {
        // If database is currently open, close it before removal
        if (m_db.isOpen())
            m_db.close();
    }

    // Always remove  named connection so Qt doesn't keep stale connections around
    QSqlDatabase::removeDatabase(m_connName);
}

// Loads souvenir items/prices for  selected campus and displays them in the table view
void NewWindow::loadSouvenirs()
{
    // Log campus name to help debug filtering/parameter binding problems
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

    // If souvenir query fails, show SQL error and stop
    if (!query.exec())
    {
        QMessageBox::critical(this, "Query Error",
                              "Souvenir query failed:\n" + query.lastError().text());
        return;
    }

    // Move executed query into the model so the view can display the results
    model->setQuery(std::move(query));

    // If  model reports an error, show it and stop.
    if (model->lastError().isValid())
    {
        QMessageBox::critical(this, "Model Error","Model error:\n" + model->lastError().text());
        return;
    }

    model->setHeaderData(0, Qt::Horizontal, "Item");
    model->setHeaderData(1, Qt::Horizontal, "Price");

    // If the UI table view exists, attach the model and autosize columns
    if (ui->tableView)
    {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    }
}
