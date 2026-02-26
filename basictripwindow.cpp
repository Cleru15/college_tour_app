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

BasicTripWindow::~BasicTripWindow()
{
    delete ui;
}

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

void BasicTripWindow::rebuildNumDropdown(int campusCount)
{
    ui->numCollegestoVisitDropdownBT->clear();

    if (campusCount <= 0)
        campusCount = 1;

    for (int i = 1; i <= campusCount; ++i)
        ui->numCollegestoVisitDropdownBT->addItem(QString::number(i));
}

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
