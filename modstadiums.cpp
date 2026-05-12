#include "modstadiums.h"
#include "ui_modstadiums.h"
#include "admin.h"
#include "modinfo.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QAbstractItemView>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

ModStadiums::ModStadiums(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModStadiums)
{
    ui->setupUi(this);
    setWindowTitle("Modify Stadiums");

    // Table setup
    ui->StadiumModtable->setColumnCount(2);
    ui->StadiumModtable->setHorizontalHeaderLabels(QStringList() << "Enabled" << "Stadium");
    ui->StadiumModtable->horizontalHeader()->setStretchLastSection(true);
    ui->StadiumModtable->verticalHeader()->setVisible(false);
    ui->StadiumModtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->StadiumModtable->setSelectionMode(QAbstractItemView::NoSelection);

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this, "Database Error",
                              "Could not open college_tour.sqlite.");
        return;
    }

    ensureStadiumAccessTable();
    syncStadiumsIntoAccessTable();
    loadStadiumChecklist();
}

ModStadiums::~ModStadiums()
{
    delete ui;
}

bool ModStadiums::ensureDbOpen()
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
        candidates << d.filePath("BaseballDatabase.sqlite");
        if (!d.cdUp())
            break;
    }

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

void ModStadiums::ensureStadiumAccessTable()
{
    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        CREATE TABLE IF NOT EXISTS stadium_access (
            stadium  TEXT PRIMARY KEY,
            enabled  INTEGER NOT NULL DEFAULT 1
        )
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Table Error",
                             "Could not create/access campus_access table:\n" + q.lastError().text());
    }
}

void ModStadiums::syncStadiumsIntoAccessTable()
{
    // Pull every stadium from distances
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT DISTINCT TRIM(stadium) AS stadium
        FROM (
            SELECT "Beginning Stadium" AS stadium FROM MLBDistances
            UNION
            SELECT "Ending Stadium" AS stadium FROM MLBDistances
            UNION
            SELECT "Stadium name" AS stadium FROM MLBInformation
            UNION
            SELECT Stadium AS stadium FROM SouvenirList
        )
        WHERE TRIM(stadium) <> ''
        ORDER BY TRIM(stadium) ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load stadiums:\n" + q.lastError().text());
        return;
    }

    QSqlQuery insertQ(QSqlDatabase::database());
    insertQ.prepare(R"(
        INSERT OR IGNORE INTO stadium_access (stadium, enabled)
        VALUES (:stadium, 1)
    )");

    while (q.next())
    {
        const QString stadium = q.value(0).toString().trimmed();
        if (stadium.isEmpty())
            continue;

        insertQ.bindValue(":stadium", stadium);
        insertQ.exec();
    }
}

void ModStadiums::loadStadiumChecklist()
{
    ui->StadiumModtable->setRowCount(0);

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT stadium, enabled
        FROM stadium_access
        WHERE TRIM(stadium) <> ''
        ORDER BY stadium ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load stadium access list:\n" + q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next())
    {
        const QString stadium  = q.value(0).toString().trimmed();
        const int originalEnabled = q.value(1).toInt();

        ui->StadiumModtable->insertRow(row);

        // Column 0: checkbox
        auto *checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

        checkItem->setData(Qt::UserRole, originalEnabled);

        // enabled = 1 is checked
        // enabled = 0 OR = 2 is unchecked
        checkItem->setCheckState(originalEnabled == 1 ? Qt::Checked
                                                      : Qt::Unchecked);

        ui->StadiumModtable->setItem(row, 0, checkItem);

        // Column 1: stadium name
        auto *nameItem = new QTableWidgetItem(stadium);
        nameItem->setFlags(Qt::ItemIsEnabled);

        ui->StadiumModtable->setItem(row, 1, nameItem);

        ++row;
    }

    ui->StadiumModtable->resizeColumnsToContents();
}

void ModStadiums::on_buttonModify_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not start transaction.");
        return;
    }

    QSqlQuery updateQ(db);

    updateQ.prepare(R"(
        UPDATE stadium_access
        SET enabled = :enabled
        WHERE TRIM(stadium) = TRIM(:stadium)
    )");

    bool ok = true;

    for (int row = 0; row < ui->StadiumModtable->rowCount(); ++row)
    {
        QTableWidgetItem *checkItem = ui->StadiumModtable->item(row, 0);
        QTableWidgetItem *nameItem  = ui->StadiumModtable->item(row, 1);

        if (!checkItem || !nameItem)
            continue;

        const QString stadium = nameItem->text().trimmed();
        const int originalEnabled = checkItem->data(Qt::UserRole).toInt();

        int enabled = 0;

        if (checkItem->checkState() == Qt::Checked)
        {
            // checked --> enabled = 1
            enabled = 1;
        }
        else if (originalEnabled == 2)
        {
            enabled = 2;
        }
        else
        {
            // unchecked --> enabled = 0
            enabled = 0;
        }

        updateQ.bindValue(":enabled", enabled);
        updateQ.bindValue(":stadium", stadium);

        if (!updateQ.exec())
        {
            ok = false;
            break;
        }
    }

    if (ok)
    {
        db.commit();

        QMessageBox::information(this,
                                 "Saved",
                                 "Stadium access has been updated.");

        // Reload table so the stored original values update after saving.
        loadStadiumChecklist();
    }
    else
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Save Error",
                             "Could not save changes:\n" +
                                 updateQ.lastError().text());
    }
}

void ModStadiums::on_buttonBack_clicked()
{
    auto *win = new Admin(this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
    this->hide();
}

void ModStadiums::on_buttonReset_clicked()
{
    const QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Reset Stadium Accessibility",
            "Are you sure you want to reset accessibility for all stadiums back to their original states?",
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

    // Clear current stadium accessibility settings.
    if (!q.exec(R"(
        DELETE FROM stadium_access
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not clear stadium_access:\n" +
                                 q.lastError().text());
        return;
    }

    // Restore original accessibility settings.
    if (!q.exec(R"(
        INSERT INTO stadium_access (stadium, enabled)
        SELECT stadium, enabled
        FROM Defaultstadium_access
    )"))
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Reset Error",
                             "Could not restore stadium_access from Defaultstadium_access:\n" +
                                 q.lastError().text());
        return;
    }

    if (!db.commit())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not commit stadium accessibility reset.");
        return;
    }

    QMessageBox::information(this,
                             "Stadium Accessibility Reset",
                             "All stadium accessibility has been reset.");

    auto *win = new Admin(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void ModStadiums::on_buttonModInfo_clicked()
{
    auto *win = new modinfo(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}
