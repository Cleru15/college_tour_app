<<<<<<< HEAD
=======
/**
 * @file admindeletesouvenir.cpp
 * @brief Implements the AdminDeleteSouvenir class.
 *
 * This window allows administrators to delete an existing
 * souvenir from a selected campus in the database.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#include "admindeletesouvenir.h"
#include "ui_admindeletesouvenir.h"
#include "modsouvenirs.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

<<<<<<< HEAD
AdminDeleteSouvenir::AdminDeleteSouvenir(const QString &stadium, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDeleteSouvenir)
    , m_stadium(stadium.trimmed())
=======
/*
 * Function: AdminDeleteSouvenir constructor
 * Purpose : Initializes the delete souvenir window, stores
 *           the selected campus, and loads its souvenirs.
 */
AdminDeleteSouvenir::AdminDeleteSouvenir(const QString &campus, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminDeleteSouvenir)
    , m_campus(campus.trimmed())
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
{
    ui->setupUi(this);
    setWindowTitle("Delete Souvenir");

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

    loadSouvenirDropdown();
}

<<<<<<< HEAD
=======
/*
 * Function: ~AdminDeleteSouvenir
 * Purpose : Cleans up UI resources when the window closes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
AdminDeleteSouvenir::~AdminDeleteSouvenir()
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
bool AdminDeleteSouvenir::ensureDbOpen()
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
 * Function: loadSouvenirDropdown
 * Purpose : Loads the souvenir names for the selected campus
 *           into the dropdown menu.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void AdminDeleteSouvenir::loadSouvenirDropdown()
{
    ui->deleteSouvCombo->clear();
    ui->deleteSouvCombo->addItem("");

<<<<<<< HEAD
    if (m_stadium.isEmpty())
        return;

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT TRIM(Name)
        FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) <> ''
        ORDER BY TRIM(Name) ASC
    )");

    q.bindValue(":stadium", m_stadium);
=======
    if (m_campus.isEmpty())
        return;

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT TRIM(item)
        FROM souvenirs
        WHERE TRIM(campus) = :campus
          AND TRIM(item) <> ''
        ORDER BY TRIM(item) ASC
    )");
    q.bindValue(":campus", m_campus);
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

    while (q.next())
    {
        const QString item = q.value(0).toString().trimmed();
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        if (!item.isEmpty())
            ui->deleteSouvCombo->addItem(item);
    }

    ui->deleteSouvCombo->setCurrentIndex(0);
}

<<<<<<< HEAD
=======
/*
 * Function: on_delSouvConfirm_clicked
 * Purpose : Deletes the selected souvenir from both the
 *           souvenirs table and the souvenir access table.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void AdminDeleteSouvenir::on_delSouvConfirm_clicked()
{
    const QString itemName = ui->deleteSouvCombo->currentText().trimmed();

    if (itemName.isEmpty())
    {
        QMessageBox::warning(this, "Missing Souvenir",
                             "Select the souvenir you want to delete.");
        return;
    }

<<<<<<< HEAD
    if (m_stadium.isEmpty())
    {
        QMessageBox::warning(this, "Missing Stadium",
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
        QMessageBox::critical(this, "Database Error",
<<<<<<< HEAD
                              "Could not open BaseballDatabase.sqlite.");
=======
                              "Could not open college_tour.sqlite.");
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

    QSqlQuery deleteSouvenir(db);
<<<<<<< HEAD

    deleteSouvenir.prepare(R"(
        DELETE FROM SouvenirList
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) = TRIM(:item)
    )");

    deleteSouvenir.bindValue(":stadium", m_stadium);
=======
    deleteSouvenir.prepare(R"(
        DELETE FROM souvenirs
        WHERE TRIM(campus) = :campus
          AND TRIM(item) = :item
    )");
    deleteSouvenir.bindValue(":campus", m_campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    deleteSouvenir.bindValue(":item", itemName);

    if (!deleteSouvenir.exec())
    {
        db.rollback();
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        QMessageBox::warning(this, "Delete Error",
                             "Could not delete souvenir:\n" +
                                 deleteSouvenir.lastError().text());
        return;
    }

    QSqlQuery deleteAccess(db);
<<<<<<< HEAD

    deleteAccess.prepare(R"(
        DELETE FROM souvenir_access
        WHERE TRIM(stadium) = TRIM(:stadium)
          AND TRIM(item) = TRIM(:item)
    )");

    deleteAccess.bindValue(":stadium", m_stadium);
=======
    deleteAccess.prepare(R"(
        DELETE FROM souvenir_access
        WHERE TRIM(campus) = :campus
          AND TRIM(item) = :item
    )");
    deleteAccess.bindValue(":campus", m_campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    deleteAccess.bindValue(":item", itemName);

    if (!deleteAccess.exec())
    {
        db.rollback();
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        QMessageBox::warning(this, "Delete Error",
                             "Could not delete souvenir access entry:\n" +
                                 deleteAccess.lastError().text());
        return;
    }

    if (!db.commit())
    {
        db.rollback();
<<<<<<< HEAD

=======
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        QMessageBox::warning(this, "Database Error",
                             "Could not commit souvenir deletion.");
        return;
    }

    QMessageBox::information(this, "Souvenir Deleted",
                             "The souvenir was deleted successfully.");

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
 * Function: on_delSouvBack_clicked
 * Purpose : Returns the user to the souvenir modification window
 *           without deleting a souvenir.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void AdminDeleteSouvenir::on_delSouvBack_clicked()
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
