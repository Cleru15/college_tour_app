<<<<<<< HEAD
=======
/**
 * @file adminaddsouvenir.cpp
 * @brief Implements the adminAddSouvenir class.
 *
 * This window allows administrators to add a new souvenir
 * for a selected campus. It validates input, updates the
 * database, and returns to the souvenir modification menu.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#include "adminaddsouvenir.h"
#include "ui_adminaddsouvenir.h"
#include "modsouvenirs.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

<<<<<<< HEAD
adminAddSouvenir::adminAddSouvenir(const QString &stadium, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminAddSouvenir)
    , m_stadium(stadium.trimmed())
=======
/*
 * Function: adminAddSouvenir constructor
 * Purpose : Initializes the add souvenir window and stores
 *           the campus where the souvenir will be added.
 */
adminAddSouvenir::adminAddSouvenir(const QString &campus, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminAddSouvenir)
    , m_campus(campus.trimmed())
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
{
    ui->setupUi(this);
    setWindowTitle("Add Souvenir");
}

<<<<<<< HEAD
=======
/*
 * Function: ~adminAddSouvenir
 * Purpose : Cleans up UI resources when the window closes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
adminAddSouvenir::~adminAddSouvenir()
{
    delete ui;
}

<<<<<<< HEAD
=======
/*
 * Function: ensureDbOpen
 * Purpose : Ensures the SQLite database connection is open
 *           before performing any database operations.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
bool adminAddSouvenir::ensureDbOpen()
{
    QSqlDatabase db;

    if (QSqlDatabase::contains())
    {
        db = QSqlDatabase::database();
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
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
<<<<<<< HEAD

    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("BaseballDatabase.sqlite");

=======
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("college_tour.sqlite");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        if (!d.cdUp())
            break;
    }

<<<<<<< HEAD
    candidates << QDir::current().filePath("BaseballDatabase.sqlite");

    QString dbPath;

=======
    candidates << QDir::current().filePath("college_tour.sqlite");

    QString dbPath;
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
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
=======
/*
 * Function: on_addSouvConfirm_clicked
 * Purpose : Validates the new souvenir information and inserts
 *           it into the database for the selected campus.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void adminAddSouvenir::on_addSouvConfirm_clicked()
{
    const QString itemName = ui->souvName->text().trimmed();
    const QString priceText = ui->souvName_2->text().trimmed();

    if (itemName.isEmpty())
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Invalid Name",
=======
        QMessageBox::warning(this, "Invalid Name",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Souvenir name cannot be blank.");
        return;
    }

    bool ok = false;
    const double price = priceText.toDouble(&ok);

    if (!ok)
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Invalid Price",
=======
        QMessageBox::warning(this, "Invalid Price",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Enter a valid numeric price.");
        return;
    }

    if (price <= 0.0)
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Invalid Price",
=======
        QMessageBox::warning(this, "Invalid Price",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Price must be greater than 0.00.");
        return;
    }

<<<<<<< HEAD
    if (m_stadium.isEmpty())
    {
        QMessageBox::warning(this,
                             "Missing Stadium",
                             "No stadium was selected.");
=======
    if (m_campus.isEmpty())
    {
        QMessageBox::warning(this, "Missing College",
                             "No college was selected.");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    if (!ensureDbOpen())
    {
<<<<<<< HEAD
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
=======
        QMessageBox::critical(this, "Database Error",
                              "Could not open college_tour.sqlite.");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();

<<<<<<< HEAD
    // Enforce max of 7 souvenirs per stadium.
    QSqlQuery countQ(db);

    countQ.prepare(R"(
        SELECT COUNT(*)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
    )");

    countQ.bindValue(":stadium", m_stadium);

    if (!countQ.exec() || !countQ.next())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not verify how many souvenirs this stadium already has.");
=======
    // Enforce max of 7 souvenirs per college
    QSqlQuery countQ(db);
    countQ.prepare(R"(
        SELECT COUNT(*)
        FROM souvenirs
        WHERE TRIM(campus) = :campus
    )");
    countQ.bindValue(":campus", m_campus);

    if (!countQ.exec() || !countQ.next())
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not verify how many souvenirs this college already has.");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    if (countQ.value(0).toInt() >= 7)
    {
<<<<<<< HEAD
        QMessageBox::information(this,
                                 "Maximum Souvenirs Reached",
                                 "Stadiums cannot have more than 7 souvenirs at once.");
=======
        QMessageBox::information(this, "Maximum Souvenirs Reached",
                                 "Colleges cannot have more than 7 souvenirs at once.");
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    if (!db.transaction())
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Database Error",
=======
        QMessageBox::warning(this, "Database Error",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Could not start database transaction.");
        return;
    }

<<<<<<< HEAD
    // Prevent duplicate souvenir names at the same stadium.
    QSqlQuery dupQ(db);

    dupQ.prepare(R"(
        SELECT COUNT(*)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) = TRIM(:item)
    )");

    dupQ.bindValue(":stadium", m_stadium);
=======
    // Prevent duplicate souvenir names at the same campus
    QSqlQuery dupQ(db);
    dupQ.prepare(R"(
        SELECT COUNT(*)
        FROM souvenirs
        WHERE TRIM(campus) = :campus
          AND TRIM(item) = :item
    )");
    dupQ.bindValue(":campus", m_campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    dupQ.bindValue(":item", itemName);

    if (!dupQ.exec() || !dupQ.next())
    {
        db.rollback();
<<<<<<< HEAD

        QMessageBox::warning(this,
                             "Database Error",
=======
        QMessageBox::warning(this, "Database Error",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Could not validate souvenir name.");
        return;
    }

    if (dupQ.value(0).toInt() > 0)
    {
        db.rollback();
<<<<<<< HEAD

        QMessageBox::warning(this,
                             "Duplicate Souvenir",
                             "That stadium already has a souvenir with this name.");
        return;
    }

    // Look up the MLB team that belongs to this stadium.
    QString teamName;

    QSqlQuery teamQ(db);

    teamQ.prepare(R"(
        SELECT TRIM("Team name")
        FROM MLBInformation
        WHERE TRIM("Stadium name") = TRIM(:stadium)
        LIMIT 1
    )");

    teamQ.bindValue(":stadium", m_stadium);

    if (!teamQ.exec() || !teamQ.next())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not find the team name for this stadium.");
        return;
    }

    teamName = teamQ.value(0).toString().trimmed();

    if (teamName.isEmpty())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "The selected stadium does not have a valid team name.");
        return;
    }

    // Insert into the baseball souvenir table.
    // Important: include Team so it does not become NULL.
    QSqlQuery insertSouvenir(db);

    insertSouvenir.prepare(R"(
        INSERT INTO SouvenirList (Team, Stadium, Name, Price)
        VALUES (:team, :stadium, :item, :price)
    )");

    insertSouvenir.bindValue(":team", teamName);
    insertSouvenir.bindValue(":stadium", m_stadium);
=======
        QMessageBox::warning(this, "Duplicate Souvenir",
                             "That college already has a souvenir with this name.");
        return;
    }

    QSqlQuery insertSouvenir(db);
    insertSouvenir.prepare(R"(
        INSERT INTO souvenirs (campus, item, price)
        VALUES (:campus, :item, :price)
    )");
    insertSouvenir.bindValue(":campus", m_campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    insertSouvenir.bindValue(":item", itemName);
    insertSouvenir.bindValue(":price", price);

    if (!insertSouvenir.exec())
    {
        db.rollback();
<<<<<<< HEAD

        QMessageBox::warning(this,
                             "Insert Error",
                             "Could not add souvenir:\n" +
                                 insertSouvenir.lastError().text());
        return;
    }

    // Make sure the access table exists using baseball field names.
    QSqlQuery ensureAccess(db);

    ensureAccess.prepare(R"(
        CREATE TABLE IF NOT EXISTS souvenir_access (
            stadium  TEXT NOT NULL,
            item     TEXT NOT NULL,
            enabled  INTEGER NOT NULL DEFAULT 1,
            PRIMARY KEY (stadium, item)
=======
        QMessageBox::warning(this, "Insert Error",
                             "Could not add souvenir:\n" + insertSouvenir.lastError().text());
        return;
    }

    QSqlQuery ensureAccess(db);
    ensureAccess.prepare(R"(
        CREATE TABLE IF NOT EXISTS souvenir_access (
            campus   TEXT NOT NULL,
            item     TEXT NOT NULL,
            enabled  INTEGER NOT NULL DEFAULT 1,
            PRIMARY KEY (campus, item)
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        )
    )");

    if (!ensureAccess.exec())
    {
        db.rollback();
<<<<<<< HEAD

        QMessageBox::warning(this,
                             "Table Error",
                             "Could not access souvenir_access table:\n" +
                                 ensureAccess.lastError().text());
        return;
    }

    // New souvenirs should start enabled.
    QSqlQuery insertAccess(db);

    insertAccess.prepare(R"(
        INSERT OR REPLACE INTO souvenir_access (stadium, item, enabled)
        VALUES (:stadium, :item, 1)
    )");

    insertAccess.bindValue(":stadium", m_stadium);
=======
        QMessageBox::warning(this, "Table Error",
                             "Could not access souvenir_access table:\n" + ensureAccess.lastError().text());
        return;
    }

    QSqlQuery insertAccess(db);
    insertAccess.prepare(R"(
        INSERT OR REPLACE INTO souvenir_access (campus, item, enabled)
        VALUES (:campus, :item, 1)
    )");
    insertAccess.bindValue(":campus", m_campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    insertAccess.bindValue(":item", itemName);

    if (!insertAccess.exec())
    {
        db.rollback();
<<<<<<< HEAD

        QMessageBox::warning(this,
                             "Insert Error",
                             "Could not enable new souvenir:\n" +
                                 insertAccess.lastError().text());
=======
        QMessageBox::warning(this, "Insert Error",
                             "Could not enable new souvenir:\n" + insertAccess.lastError().text());
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        return;
    }

    if (!db.commit())
    {
        db.rollback();
<<<<<<< HEAD

        QMessageBox::warning(this,
                             "Database Error",
=======
        QMessageBox::warning(this, "Database Error",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Could not commit souvenir changes.");
        return;
    }

<<<<<<< HEAD
    QMessageBox::information(this,
                             "Souvenir Added",
=======
    QMessageBox::information(this, "Souvenir Added",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "The new souvenir was added successfully.");

    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
<<<<<<< HEAD
    win->setSelectedStadium(m_stadium);

    this->close();
}

=======
    win->setSelectedCampus(m_campus);
    this->close();
}

/*
 * Function: on_addSouvBack_clicked
 * Purpose : Returns the user to the souvenir modification window
 *           without adding a new souvenir.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void adminAddSouvenir::on_addSouvBack_clicked()
{
    auto *win = new ModSouvenirs(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
<<<<<<< HEAD
    win->setSelectedStadium(m_stadium);

=======
    win->setSelectedCampus(m_campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    this->close();
}
