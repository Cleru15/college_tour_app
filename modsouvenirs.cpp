<<<<<<< HEAD
=======
/**
 * @file modsouvenirs.cpp
 * @brief Implements the ModSouvenirs class.
 *
 * This window allows administrators to enable or disable souvenirs,
 * add new souvenirs, change prices, and delete existing souvenirs
 * for a selected campus.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#include "modsouvenirs.h"
#include "ui_modsouvenirs.h"
#include "admin.h"
#include "adminaddsouvenir.h"
#include "adminchangeprice.h"
#include "admindeletesouvenir.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QTableWidgetItem>
<<<<<<< HEAD
#include <QAbstractItemView>
=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

<<<<<<< HEAD
=======
/*
 * Function: ModSouvenirs constructor
 * Purpose : Initializes the souvenir modification window,
 *           prepares the table, and loads the campus list.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
ModSouvenirs::ModSouvenirs(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModSouvenirs)
{
    ui->setupUi(this);
    setWindowTitle("Modify Souvenirs");

    ui->SuvModTabel->setColumnCount(3);
    ui->SuvModTabel->setHorizontalHeaderLabels(QStringList() << "Enabled" << "Souvenir" << "Price");
    ui->SuvModTabel->horizontalHeader()->setStretchLastSection(true);
    ui->SuvModTabel->verticalHeader()->setVisible(false);
    ui->SuvModTabel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->SuvModTabel->setSelectionMode(QAbstractItemView::NoSelection);
    ui->SuvModTabel->setRowCount(0);

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
<<<<<<< HEAD
                              "Could not open BaseballDatabase.sqlite.");
=======
                              "Could not open college_tour.sqlite.");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    ensureSouvenirAccessTable();
    syncSouvenirsIntoAccessTable();
<<<<<<< HEAD
    loadStadiumDropdown();
    resetToDefaultState();
}

=======
    loadCollegeDropdown();
    resetToDefaultState();
}

/*
 * Function: ~ModSouvenirs
 * Purpose : Cleans up UI resources when the window closes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
ModSouvenirs::~ModSouvenirs()
{
    delete ui;
}

<<<<<<< HEAD
=======
/*
 * Function: ensureDbOpen
 * Purpose : Ensures the SQLite database connection is open
 *           before performing admin souvenir operations.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
bool ModSouvenirs::ensureDbOpen()
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
<<<<<<< HEAD
        candidates << d.filePath("BaseballDatabase.sqlite");
=======
        candidates << d.filePath("college_tour.sqlite");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        if (!d.cdUp())
            break;
    }

<<<<<<< HEAD
    candidates << QDir::current().filePath("BaseballDatabase.sqlite");
=======
    candidates << QDir::current().filePath("college_tour.sqlite");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

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

<<<<<<< HEAD
void ModSouvenirs::ensureSouvenirAccessTable()
{
    QSqlDatabase db = QSqlDatabase::database();

    bool hasOldCampusColumn = false;
    bool hasNewStadiumColumn = false;

    QSqlQuery pragmaQ(db);

    if (pragmaQ.exec("PRAGMA table_info(souvenir_access)"))
    {
        while (pragmaQ.next())
        {
            const QString columnName = pragmaQ.value("name").toString();

            if (columnName.compare("campus", Qt::CaseInsensitive) == 0)
                hasOldCampusColumn = true;

            if (columnName.compare("stadium", Qt::CaseInsensitive) == 0)
                hasNewStadiumColumn = true;
        }
    }

    if (hasOldCampusColumn && !hasNewStadiumColumn)
    {
        QSqlQuery dropQ(db);

        if (!dropQ.exec("DROP TABLE souvenir_access"))
        {
            QMessageBox::warning(this,
                                 "Table Error",
                                 "Could not remove old souvenir_access table:\n" +
                                     dropQ.lastError().text());
            return;
        }
    }

    QSqlQuery q(db);

    q.prepare(R"(
        CREATE TABLE IF NOT EXISTS souvenir_access (
            stadium  TEXT NOT NULL,
            item     TEXT NOT NULL,
            enabled  INTEGER NOT NULL DEFAULT 1,
            PRIMARY KEY (stadium, item)
=======
/*
 * Function: ensureSouvenirAccessTable
 * Purpose : Creates the souvenir access table if it does not
 *           already exist in the database.
 */
void ModSouvenirs::ensureSouvenirAccessTable()
{
    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        CREATE TABLE IF NOT EXISTS souvenir_access (
            campus   TEXT NOT NULL,
            item     TEXT NOT NULL,
            enabled  INTEGER NOT NULL DEFAULT 1,
            PRIMARY KEY (campus, item)
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        )
    )");

    if (!q.exec())
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Table Error",
=======
        QMessageBox::warning(this, "Table Error",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Could not create/access souvenir_access table:\n" +
                                 q.lastError().text());
    }
}

<<<<<<< HEAD
=======
/*
 * Function: syncSouvenirsIntoAccessTable
 * Purpose : Copies all souvenirs into the souvenir access table
 *           if they are not already present.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void ModSouvenirs::syncSouvenirsIntoAccessTable()
{
    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
<<<<<<< HEAD
        INSERT OR IGNORE INTO souvenir_access (stadium, item, enabled)
        SELECT TRIM("Stadium"), TRIM("Name"), 1
        FROM SouvenirList
        WHERE TRIM("Stadium") <> ''
          AND TRIM("Name") <> ''
=======
        INSERT OR IGNORE INTO souvenir_access (campus, item, enabled)
        SELECT TRIM(campus), TRIM(item), 1
        FROM souvenirs
        WHERE TRIM(campus) <> ''
          AND TRIM(item) <> ''
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    )");

    if (!q.exec())
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Sync Error",
=======
        QMessageBox::warning(this, "Sync Error",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Could not sync souvenirs into souvenir_access:\n" +
                                 q.lastError().text());
    }
}

<<<<<<< HEAD
void ModSouvenirs::loadStadiumDropdown()
=======
/*
 * Function: loadCollegeDropdown
 * Purpose : Loads all campuses into the combo box so the
 *           administrator can choose which campus to manage.
 */
void ModSouvenirs::loadCollegeDropdown()
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
{
    QSignalBlocker blocker(ui->comboBox);

    ui->comboBox->clear();
<<<<<<< HEAD
    ui->comboBox->addItem("Select a Stadium");

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT DISTINCT TRIM(sa.stadium) AS stadium
        FROM stadium_access sa
        WHERE TRIM(sa.stadium) <> ''
        ORDER BY TRIM(sa.stadium) ASC
=======
    ui->comboBox->addItem("Select a College");

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
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    )");

    if (!q.exec())
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Query Error",
                             "Could not load stadium list:\n" +
                                 q.lastError().text());
=======
        QMessageBox::warning(this, "Query Error",
                             "Could not load college list:\n" + q.lastError().text());
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    while (q.next())
    {
<<<<<<< HEAD
        const QString stadium = q.value(0).toString().trimmed();

        if (!stadium.isEmpty())
            ui->comboBox->addItem(stadium);
=======
        const QString campus = q.value(0).toString().trimmed();
        if (!campus.isEmpty())
            ui->comboBox->addItem(campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    }

    ui->comboBox->setCurrentIndex(0);
}

<<<<<<< HEAD
=======
/*
 * Function: clearSouvenirTable
 * Purpose : Clears all rows from the souvenir display table.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void ModSouvenirs::clearSouvenirTable()
{
    ui->SuvModTabel->clearContents();
    ui->SuvModTabel->setRowCount(0);
}

<<<<<<< HEAD
void ModSouvenirs::resetToDefaultState()
{
    m_selectedStadium.clear();
=======
/*
 * Function: resetToDefaultState
 * Purpose : Resets the window to its initial state with
 *           no campus selected and no souvenirs shown.
 */
void ModSouvenirs::resetToDefaultState()
{
    m_selectedCampus.clear();
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    ui->comboBox->setEnabled(true);
    ui->SubmitButt->setEnabled(true);

    {
        QSignalBlocker blocker(ui->comboBox);
        ui->comboBox->setCurrentIndex(0);
    }

    clearSouvenirTable();

    ui->addSouvButt->hide();
    ui->delSouvButt->hide();
    ui->changePrButt->hide();
}

<<<<<<< HEAD
void ModSouvenirs::loadSouvenirsForStadium(const QString &stadium)
{
    clearSouvenirTable();

    const QString selectedStadium = stadium.trimmed();

    if (selectedStadium.isEmpty() ||
        selectedStadium.compare("Select a Stadium", Qt::CaseInsensitive) == 0)
=======
/*
 * Function: loadSouvenirsForCollege
 * Purpose : Loads all souvenirs for the selected campus into
 *           the table, including enabled state and price.
 */
void ModSouvenirs::loadSouvenirsForCollege(const QString &campus)
{
    clearSouvenirTable();

    const QString selectedCampus = campus.trimmed();
    if (selectedCampus.isEmpty() ||
        selectedCampus.compare("Select a College", Qt::CaseInsensitive) == 0)
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    {
        return;
    }

    QSqlQuery q(QSqlDatabase::database());
<<<<<<< HEAD

    q.prepare(R"(
        SELECT sa.item, sa.enabled, s.Price
        FROM souvenir_access sa
        LEFT JOIN SouvenirList s
            ON TRIM(s.Stadium) = TRIM(sa.stadium)
           AND TRIM(s.Name)    = TRIM(sa.item)
        WHERE TRIM(sa.stadium) = :stadium
        ORDER BY sa.item ASC
    )");

    q.bindValue(":stadium", selectedStadium);
=======
    q.prepare(R"(
        SELECT sa.item, sa.enabled, s.price
        FROM souvenir_access sa
        LEFT JOIN souvenirs s
            ON TRIM(s.campus) = TRIM(sa.campus)
           AND TRIM(s.item)   = TRIM(sa.item)
        WHERE TRIM(sa.campus) = :campus
        ORDER BY sa.item ASC
    )");
    q.bindValue(":campus", selectedCampus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
<<<<<<< HEAD
                             "Could not load souvenirs for selected stadium:\n" +
=======
                             "Could not load souvenirs for selected college:\n" +
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                                 q.lastError().text());
        return;
    }

    int row = 0;
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    while (q.next())
    {
        const QString item = q.value(0).toString().trimmed();
        const bool enabled = q.value(1).toInt() != 0;
        const double price = q.value(2).toDouble();

        ui->SuvModTabel->insertRow(row);

        auto *checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        checkItem->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
        ui->SuvModTabel->setItem(row, 0, checkItem);

        auto *nameItem = new QTableWidgetItem(item);
        nameItem->setFlags(Qt::ItemIsEnabled);
        ui->SuvModTabel->setItem(row, 1, nameItem);

        auto *priceItem = new QTableWidgetItem(QString::number(price, 'f', 2));
        priceItem->setFlags(Qt::ItemIsEnabled);
        ui->SuvModTabel->setItem(row, 2, priceItem);

        ++row;
    }

    ui->SuvModTabel->resizeColumnsToContents();
}

<<<<<<< HEAD
=======
/*
 * Function: on_comboBox_currentIndexChanged
 * Purpose : Clears the current souvenir display when the
 *           selected campus changes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void ModSouvenirs::on_comboBox_currentIndexChanged(int)
{
    if (ui->comboBox->isEnabled())
    {
        clearSouvenirTable();
        ui->addSouvButt->hide();
        ui->delSouvButt->hide();
        ui->changePrButt->hide();
<<<<<<< HEAD
        m_selectedStadium.clear();
    }
}

void ModSouvenirs::on_SubmitButt_clicked()
{
    const QString stadium = ui->comboBox->currentText().trimmed();

    if (stadium.isEmpty() ||
        stadium.compare("Select a Stadium", Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium first.");
        return;
    }

    m_selectedStadium = stadium;

    loadSouvenirsForStadium(m_selectedStadium);
=======
        m_selectedCampus.clear();
    }
}

/*
 * Function: on_SubmitButt_clicked
 * Purpose : Confirms the selected campus and loads its
 *           souvenirs into the table.
 */
void ModSouvenirs::on_SubmitButt_clicked()
{
    const QString campus = ui->comboBox->currentText().trimmed();

    if (campus.isEmpty() ||
        campus.compare("Select a College", Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(this, "Select a College",
                                 "Choose a college first.");
        return;
    }

    m_selectedCampus = campus;

    loadSouvenirsForCollege(m_selectedCampus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    ui->comboBox->setEnabled(false);
    ui->SubmitButt->setEnabled(false);

    ui->addSouvButt->show();
    ui->delSouvButt->show();
    ui->changePrButt->show();
}

<<<<<<< HEAD
=======
/*
 * Function: on_cancelButt_clicked
 * Purpose : Resets the window without saving changes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void ModSouvenirs::on_cancelButt_clicked()
{
    resetToDefaultState();
}

<<<<<<< HEAD
void ModSouvenirs::on_SuvModifybutton_clicked()
{
    const QString stadium = m_selectedStadium.trimmed();

    if (stadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
=======
/*
 * Function: on_SuvModifybutton_clicked
 * Purpose : Saves the current enabled/disabled souvenir
 *           checkbox states into the database.
 */
void ModSouvenirs::on_SuvModifybutton_clicked()
{
    const QString campus = m_selectedCampus.trimmed();

    if (campus.isEmpty())
    {
        QMessageBox::information(this, "Select a College",
                                 "Choose a college and click Submit first.");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    if (!db.transaction())
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not start database transaction.");
        return;
    }

    QSqlQuery updateQ(db);
<<<<<<< HEAD

    updateQ.prepare(R"(
        UPDATE souvenir_access
        SET enabled = :enabled
        WHERE TRIM(stadium) = TRIM(:stadium)
          AND TRIM(item) = TRIM(:item)
=======
    updateQ.prepare(R"(
        UPDATE souvenir_access
        SET enabled = :enabled
        WHERE campus = :campus
          AND item = :item
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    )");

    bool ok = true;

    for (int row = 0; row < ui->SuvModTabel->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = ui->SuvModTabel->item(row, 0);
        QTableWidgetItem *nameItem  = ui->SuvModTabel->item(row, 1);

        if (!checkItem || !nameItem)
            continue;

        const QString item = nameItem->text().trimmed();
        const int enabled = (checkItem->checkState() == Qt::Checked) ? 1 : 0;

        updateQ.bindValue(":enabled", enabled);
<<<<<<< HEAD
        updateQ.bindValue(":stadium", stadium);
=======
        updateQ.bindValue(":campus", campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        updateQ.bindValue(":item", item);

        if (!updateQ.exec())
        {
            ok = false;
            break;
        }
    }

    if (ok)
    {
        db.commit();
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        QMessageBox::information(this, "Saved",
                                 "Souvenir access has been updated.");
    }
    else
    {
        db.rollback();
<<<<<<< HEAD

        QMessageBox::warning(this, "Save Error",
                             "Could not save souvenir settings:\n" +
                                 updateQ.lastError().text());
    }
}

int ModSouvenirs::souvenirCountForStadium(const QString &stadium)
{
    if (stadium.trimmed().isEmpty())
=======
        QMessageBox::warning(this, "Save Error",
                             "Could not save souvenir settings.");
    }
}

/*
 * Function: souvenirCountForCampus
 * Purpose : Returns the number of souvenirs currently
 *           associated with the selected campus.
 */
int ModSouvenirs::souvenirCountForCampus(const QString &campus)
{
    if (campus.trimmed().isEmpty())
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return -1;

    if (!ensureDbOpen())
        return -1;

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery q(db);
<<<<<<< HEAD

    q.prepare(R"(
        SELECT COUNT(*)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
    )");

    q.bindValue(":stadium", stadium.trimmed());
=======
    q.prepare(R"(
        SELECT COUNT(*)
        FROM souvenirs
        WHERE TRIM(campus) = :campus
    )");
    q.bindValue(":campus", campus.trimmed());
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    if (!q.exec() || !q.next())
        return -1;

    return q.value(0).toInt();
}

<<<<<<< HEAD
void ModSouvenirs::on_addSouvButt_clicked()
{
    if (m_selectedStadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
        return;
    }

    const int souvenirCount = souvenirCountForStadium(m_selectedStadium);
=======
/*
 * Function: on_addSouvButt_clicked
 * Purpose : Opens the add souvenir window for the
 *           currently selected campus.
 */
void ModSouvenirs::on_addSouvButt_clicked()
{
    if (m_selectedCampus.isEmpty())
    {
        QMessageBox::information(this, "Select a College",
                                 "Choose a college and click Submit first.");
        return;
    }

    const int souvenirCount = souvenirCountForCampus(m_selectedCampus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    if (souvenirCount < 0)
    {
        QMessageBox::warning(this, "Database Error",
<<<<<<< HEAD
                             "Could not verify the number of souvenirs for this stadium.");
=======
                             "Could not verify the number of souvenirs for this college.");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    if (souvenirCount >= 7)
    {
        QMessageBox::information(this, "Maximum Souvenirs Reached",
<<<<<<< HEAD
                                 "Stadiums cannot have more than 7 souvenirs at once.");
        return;
    }

    auto *win = new adminAddSouvenir(m_selectedStadium, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

=======
                                 "Colleges cannot have more than 7 souvenirs at once.");
        return;
    }

    auto *win = new adminAddSouvenir(m_selectedCampus, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}

/*
 * Function: on_pushButton_clicked
 * Purpose : Returns the user to the admin menu.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void ModSouvenirs::on_pushButton_clicked()
{
    auto *win = new Admin(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
<<<<<<< HEAD

    this->hide();
}

void ModSouvenirs::on_changePrButt_clicked()
{
    if (m_selectedStadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
        return;
    }

    auto *win = new adminChangePrice(m_selectedStadium, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void ModSouvenirs::on_delSouvButt_clicked()
{
    if (m_selectedStadium.isEmpty())
    {
        QMessageBox::information(this, "Select a Stadium",
                                 "Choose a stadium and click Submit first.");
        return;
    }

    auto *win = new AdminDeleteSouvenir(m_selectedStadium, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void ModSouvenirs::setSelectedStadium(const QString &stadium)
{
    const QString trimmedStadium = stadium.trimmed();

    if (trimmedStadium.isEmpty() ||
        trimmedStadium.compare("Select a Stadium", Qt::CaseInsensitive) == 0)
=======
    this->hide();
}

/*
 * Function: on_changePrButt_clicked
 * Purpose : Opens the change price window for the
 *           currently selected campus.
 */
void ModSouvenirs::on_changePrButt_clicked()
{
    if (m_selectedCampus.isEmpty())
    {
        QMessageBox::information(this, "Select a College",
                                 "Choose a college and click Submit first.");
        return;
    }

    auto *win = new adminChangePrice(m_selectedCampus, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}

/*
 * Function: on_delSouvButt_clicked
 * Purpose : Opens the delete souvenir window for the
 *           currently selected campus.
 */
void ModSouvenirs::on_delSouvButt_clicked()
{
    if (m_selectedCampus.isEmpty())
    {
        QMessageBox::information(this, "Select a College",
                                 "Choose a college and click Submit first.");
        return;
    }

    auto *win = new AdminDeleteSouvenir(m_selectedCampus, nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->close();
}

/*
 * Function: setSelectedCampus
 * Purpose : Restores the souvenir modification window with
 *           the specified campus already selected.
 */
void ModSouvenirs::setSelectedCampus(const QString &campus)
{
    const QString trimmedCampus = campus.trimmed();

    if (trimmedCampus.isEmpty() ||
        trimmedCampus.compare("Select a College", Qt::CaseInsensitive) == 0)
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    {
        resetToDefaultState();
        return;
    }

<<<<<<< HEAD
    int index = ui->comboBox->findText(trimmedStadium, Qt::MatchFixedString);

=======
    int index = ui->comboBox->findText(trimmedCampus, Qt::MatchFixedString);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    if (index < 0)
    {
        resetToDefaultState();
        return;
    }

    {
        QSignalBlocker blocker(ui->comboBox);
        ui->comboBox->setCurrentIndex(index);
    }

<<<<<<< HEAD
    m_selectedStadium = trimmedStadium;

    loadSouvenirsForStadium(m_selectedStadium);
=======
    m_selectedCampus = trimmedCampus;
    loadSouvenirsForCollege(m_selectedCampus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

    ui->comboBox->setEnabled(false);
    ui->SubmitButt->setEnabled(false);

    ui->addSouvButt->show();
    ui->delSouvButt->show();
    ui->changePrButt->show();
}
<<<<<<< HEAD

void ModSouvenirs::on_SuvResetButt_clicked()
{
    const QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Reset Souvenirs",
            "Are you sure you want to reset the list of souvenirs for all stadiums back to their original states?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

    if (reply == QMessageBox::No)
    {
        return;
    }

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not start database transaction.");
        return;
    }

    QSqlQuery q(db);

    // Clear current souvenir list.
    if (!q.exec(R"(
        DELETE FROM SouvenirList
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not clear SouvenirList:\n" +
                                 q.lastError().text());
        return;
    }

    // Restore original souvenirs from DefaultSouvenirList.
    if (!q.exec(R"(
        INSERT INTO SouvenirList (Team, Stadium, Name, Price)
        SELECT Team, Stadium, Name, Price
        FROM DefaultSouvenirList
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not restore souvenirs from DefaultSouvenirList:\n" +
                                 q.lastError().text());
        return;
    }

    /*
        Rebuild souvenir_access so it matches the restored souvenir list.

        This prevents souvenir_access from keeping old deleted items,
        missing restored default items, or referencing souvenirs that
        no longer exist after the reset.
    */
    if (!q.exec(R"(
        DELETE FROM souvenir_access
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not clear souvenir_access:\n" +
                                 q.lastError().text());
        return;
    }

    if (!q.exec(R"(
        INSERT INTO souvenir_access (stadium, item, enabled)
        SELECT Stadium, Name, 1
        FROM DefaultSouvenirList
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not rebuild souvenir_access:\n" +
                                 q.lastError().text());
        return;
    }

    if (!db.commit())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not commit souvenir reset.");
        return;
    }

    QMessageBox::information(this,
                             "Souvenirs Reset",
                             "All souvenirs have been reset.");

    auto *win = new Admin(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}
=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
