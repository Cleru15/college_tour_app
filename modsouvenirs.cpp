/**
 * @file modsouvenirs.cpp
 * @brief Implements the ModSouvenirs class.
 *
 * This window allows administrators to enable or disable souvenirs,
 * add new souvenirs, change prices, and delete existing souvenirs
 * for a selected campus.
 */

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

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
 * Function: ModSouvenirs constructor
 * Purpose : Initializes the souvenir modification window,
 *           prepares the table, and loads the campus list.
 */
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
                              "Could not open college_tour.sqlite.");
        return;
    }

    ensureSouvenirAccessTable();
    syncSouvenirsIntoAccessTable();
    loadCollegeDropdown();
    resetToDefaultState();
}

/*
 * Function: ~ModSouvenirs
 * Purpose : Cleans up UI resources when the window closes.
 */
ModSouvenirs::~ModSouvenirs()
{
    delete ui;
}

/*
 * Function: ensureDbOpen
 * Purpose : Ensures the SQLite database connection is open
 *           before performing admin souvenir operations.
 */
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
        )
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Table Error",
                             "Could not create/access souvenir_access table:\n" +
                                 q.lastError().text());
    }
}

/*
 * Function: syncSouvenirsIntoAccessTable
 * Purpose : Copies all souvenirs into the souvenir access table
 *           if they are not already present.
 */
void ModSouvenirs::syncSouvenirsIntoAccessTable()
{
    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        INSERT OR IGNORE INTO souvenir_access (campus, item, enabled)
        SELECT TRIM(campus), TRIM(item), 1
        FROM souvenirs
        WHERE TRIM(campus) <> ''
          AND TRIM(item) <> ''
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Sync Error",
                             "Could not sync souvenirs into souvenir_access:\n" +
                                 q.lastError().text());
    }
}

/*
 * Function: loadCollegeDropdown
 * Purpose : Loads all campuses into the combo box so the
 *           administrator can choose which campus to manage.
 */
void ModSouvenirs::loadCollegeDropdown()
{
    QSignalBlocker blocker(ui->comboBox);

    ui->comboBox->clear();
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
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load college list:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString campus = q.value(0).toString().trimmed();
        if (!campus.isEmpty())
            ui->comboBox->addItem(campus);
    }

    ui->comboBox->setCurrentIndex(0);
}

/*
 * Function: clearSouvenirTable
 * Purpose : Clears all rows from the souvenir display table.
 */
void ModSouvenirs::clearSouvenirTable()
{
    ui->SuvModTabel->clearContents();
    ui->SuvModTabel->setRowCount(0);
}

/*
 * Function: resetToDefaultState
 * Purpose : Resets the window to its initial state with
 *           no campus selected and no souvenirs shown.
 */
void ModSouvenirs::resetToDefaultState()
{
    m_selectedCampus.clear();

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
    {
        return;
    }

    QSqlQuery q(QSqlDatabase::database());
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

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load souvenirs for selected college:\n" +
                                 q.lastError().text());
        return;
    }

    int row = 0;
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

/*
 * Function: on_comboBox_currentIndexChanged
 * Purpose : Clears the current souvenir display when the
 *           selected campus changes.
 */
void ModSouvenirs::on_comboBox_currentIndexChanged(int)
{
    if (ui->comboBox->isEnabled())
    {
        clearSouvenirTable();
        ui->addSouvButt->hide();
        ui->delSouvButt->hide();
        ui->changePrButt->hide();
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

    ui->comboBox->setEnabled(false);
    ui->SubmitButt->setEnabled(false);

    ui->addSouvButt->show();
    ui->delSouvButt->show();
    ui->changePrButt->show();
}

/*
 * Function: on_cancelButt_clicked
 * Purpose : Resets the window without saving changes.
 */
void ModSouvenirs::on_cancelButt_clicked()
{
    resetToDefaultState();
}

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
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction())
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not start database transaction.");
        return;
    }

    QSqlQuery updateQ(db);
    updateQ.prepare(R"(
        UPDATE souvenir_access
        SET enabled = :enabled
        WHERE campus = :campus
          AND item = :item
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
        updateQ.bindValue(":campus", campus);
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
        QMessageBox::information(this, "Saved",
                                 "Souvenir access has been updated.");
    }
    else
    {
        db.rollback();
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
        return -1;

    if (!ensureDbOpen())
        return -1;

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery q(db);
    q.prepare(R"(
        SELECT COUNT(*)
        FROM souvenirs
        WHERE TRIM(campus) = :campus
    )");
    q.bindValue(":campus", campus.trimmed());

    if (!q.exec() || !q.next())
        return -1;

    return q.value(0).toInt();
}

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

    if (souvenirCount < 0)
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not verify the number of souvenirs for this college.");
        return;
    }

    if (souvenirCount >= 7)
    {
        QMessageBox::information(this, "Maximum Souvenirs Reached",
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
void ModSouvenirs::on_pushButton_clicked()
{
    auto *win = new Admin(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
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
    {
        resetToDefaultState();
        return;
    }

    int index = ui->comboBox->findText(trimmedCampus, Qt::MatchFixedString);
    if (index < 0)
    {
        resetToDefaultState();
        return;
    }

    {
        QSignalBlocker blocker(ui->comboBox);
        ui->comboBox->setCurrentIndex(index);
    }

    m_selectedCampus = trimmedCampus;
    loadSouvenirsForCollege(m_selectedCampus);

    ui->comboBox->setEnabled(false);
    ui->SubmitButt->setEnabled(false);

    ui->addSouvButt->show();
    ui->delSouvButt->show();
    ui->changePrButt->show();
}
