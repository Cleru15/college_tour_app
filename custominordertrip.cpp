#include "custominordertrip.h"
#include "ui_custominordertrip.h"

CustomInOrderTrip::CustomInOrderTrip(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustomInOrderTrip)
{
    ui->setupUi(this);

    setWindowTitle("Custom In Order Trip");

    ui->selectStartingStadiumDropdownCOT->clear();
    ui->selectNextStadiumDropdownCOT->clear();

    if (ui->tripOrder)
        ui->tripOrder->hide();

    const QRect listRect = ui->tripOrder ? ui->tripOrder->geometry() : QRect(20, 210, 290, 200);

    m_tripList = new QListWidget(this);
    m_tripList->setGeometry(listRect);
    m_tripList->clear();

    // Load only stadiums where stadium_access.enabled = 1.
    if (ensureDbOpen())
        m_allStadiums = loadStadiums();

    for (const QString &stadium : m_allStadiums)
    {
        ui->selectStartingStadiumDropdownCOT->addItem(stadium);
    }

    // No default stadium selected.
    ui->selectStartingStadiumDropdownCOT->setCurrentIndex(-1);
    ui->selectNextStadiumDropdownCOT->setCurrentIndex(-1);

    // The user should not pick the next stadium until the start is locked in.
    ui->selectNextStadiumDropdownCOT->setEnabled(false);
    ui->confirmButtonCOT_2->setEnabled(false);
}

CustomInOrderTrip::~CustomInOrderTrip()
{
    delete ui;
}


bool CustomInOrderTrip::ensureDbOpen()
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
        candidates << d.filePath("BaseballDatabase.sqlite");
        if (!d.cdUp())
            break;
    }

    // Also try current working directory (Qt Creator often sets this to the build folder)
    candidates << QDir::current().filePath("BaseballDatabase.sqlite");

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

QStringList CustomInOrderTrip::loadStadiums()
{
    QStringList allStadiums;

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT DISTINCT TRIM(sa.stadium) AS stadium
        FROM stadium_access sa
        WHERE sa.enabled = 1
          AND TRIM(sa.stadium) <> ''
          AND EXISTS (
              SELECT 1
              FROM MLBDistances d
              WHERE TRIM(d."Beginning Stadium") = TRIM(sa.stadium)
                 OR TRIM(d."Ending Stadium") = TRIM(sa.stadium)
          )
        ORDER BY TRIM(sa.stadium) ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load enabled stadium list:\n" +
                                 q.lastError().text());
        return allStadiums;
    }

    while (q.next())
    {
        const QString stadium = q.value(0).toString().trimmed();

        if (!stadium.isEmpty())
            allStadiums << stadium;
    }

    return allStadiums;
}

void CustomInOrderTrip::on_confirmButtonCOT_clicked()
{
    const QString startStadium =
        ui->selectStartingStadiumDropdownCOT->currentText().trimmed();

    if (startStadium.isEmpty())
    {
        QMessageBox::warning(this, "No Starting Stadium", "Please select a starting stadium first.");
        return;
    }

    // Lock in the starting stadium.
    m_startLocked = startStadium;

    // Clear the visible trip order list and add the starting stadium.
    if (m_tripList)
    {
        m_tripList->clear();
        m_tripList->addItem(m_startLocked);
    }

    selected.clear();
    remaining.clear();

    selected << m_startLocked;

    // m_allStadiums should contain only stadium_access.enabled = 1 stadiums.
    ui->selectNextStadiumDropdownCOT->clear();

    for (const QString &stadium : m_allStadiums)
    {
        if (stadium.compare(m_startLocked, Qt::CaseInsensitive) != 0)
        {
            ui->selectNextStadiumDropdownCOT->addItem(stadium);
            remaining << stadium;
        }
    }

    // Do not auto-select the first next stadium.
    ui->selectNextStadiumDropdownCOT->setCurrentIndex(-1);

    ui->selectNextStadiumDropdownCOT->setEnabled(true);
    ui->confirmButtonCOT_2->setEnabled(true);

    // Prevent changing the starting stadium after it is locked.
    ui->selectStartingStadiumDropdownCOT->setEnabled(false);
    ui->confirmButtonCOT->setEnabled(false);
}

void CustomInOrderTrip::on_confirmButtonCOT_2_clicked()
{
    const QString current =
        ui->selectNextStadiumDropdownCOT->currentText().trimmed();

    if (current.isEmpty())
    {
        QMessageBox::information(this, "Select Stadium", "Please select the next stadium.");
        return;
    }

    if (selected.contains(current, Qt::CaseInsensitive))
    {
        QMessageBox::information(this, "Already Selected", "That stadium is already part of the custom tour.");
        return;
    }

    selected << current;

    if (m_tripList)
        m_tripList->addItem(current);

    // Rebuild the remaining list from the full enabled=1 stadium list.
    remaining.clear();

    for (const QString &stadium : m_allStadiums)
    {
        if (!selected.contains(stadium, Qt::CaseInsensitive))
            remaining << stadium;
    }

    ui->selectNextStadiumDropdownCOT->clear();

    for (const QString &stadium : remaining)
    {
        ui->selectNextStadiumDropdownCOT->addItem(stadium);
    }

    if (remaining.isEmpty())
    {
        ui->selectNextStadiumDropdownCOT->setEnabled(false);
        ui->confirmButtonCOT_2->setEnabled(false);
    }
    else
    {
        ui->selectNextStadiumDropdownCOT->setEnabled(true);
        ui->confirmButtonCOT_2->setEnabled(true);

        ui->selectNextStadiumDropdownCOT->setCurrentIndex(-1);
    }
}

void CustomInOrderTrip::on_startTripButtonCOT_clicked()
{
    if (m_startLocked.isEmpty())
    {
        QMessageBox::information(this, "Select Start",
                                 "Click Confirm to lock in your starting stadium first.");
        return;
    }


    if (selected.size() <= 1)
    {
        QMessageBox::information(this, "No Stadiums Selected",
                                 "Select at least one stadium to visit in your custom trip.");
        return;
    }

    // Hide this setup window so TripWindow is the only one open.
    this->hide();

    //tripWindow dlg(m_startLocked, selected, selected.size(), /*forceExact=*/true, nullptr);
    tripWindow dlg(m_startLocked, selected, selected.size(), /*forceExact=*/false, nullptr, "custom");
    dlg.setModal(true);
    dlg.exec();

    // Close the setup window once TripWindow is done
    this->close();
}

void CustomInOrderTrip::on_backButtCOT_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}
