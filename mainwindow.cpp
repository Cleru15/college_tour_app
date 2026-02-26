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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Campus Tour");

    // Build the distances table on top of the existing list view area.
    setupDistanceUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
            dbPath = QDir(p).absolutePath() == p ? p : p;
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

void MainWindow::setupDistanceUi()
{
    if (!ensureDbOpen())
        return;

    // Reuse existing dropdown + button from the .ui
    m_fromCampusCombo = ui->dropdownSouvenirPreview_2;

    // Replace the center listView with a real table (like your souvenir window)
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

    // Load default distances
    if (m_fromCampusCombo && m_fromCampusCombo->count() > 0)
    {
        QString start = m_fromCampusCombo->currentData().toString();
        if (start.isEmpty())
            start = m_fromCampusCombo->currentText();
        loadDistancesForCampus(start);
    }
}

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

    // Default to Saddleback if present
    const int idx = m_fromCampusCombo->findData("Saddleback College");
    if (idx >= 0)
        m_fromCampusCombo->setCurrentIndex(idx);
}

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

// ---- Slots required by mainwindow.ui connections ----

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

void MainWindow::cancelButtonClick()
{
    QCoreApplication::quit();
}

// ---- Auto-connected slots from object names ----

void MainWindow::on_submitButtonSouvenirPreview_2_clicked()
{
    if (!m_fromCampusCombo)
        return;

    QString from = m_fromCampusCombo->currentData().toString();
    if (from.isEmpty())
        from = m_fromCampusCombo->currentText();

    // Ignore placeholder if any
    if (from.startsWith("Distance", Qt::CaseInsensitive))
        return;

    loadDistancesForCampus(from);
}

void MainWindow::on_submitButtonSouvenirPreview_3_clicked()
{
    // Basic Trip
    auto *win = new BasicTripWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}

void MainWindow::on_submitButtonSouvenirPreview_4_clicked()
{
    // Custom Trip
    auto *win = new CustomTripWindow(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}
