<<<<<<< HEAD
=======
/**
 * @file adminchangeprice.cpp
 * @brief Implements the adminChangePrice class.
 *
 * This window allows administrators to update the price
 * of an existing souvenir for a selected campus.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#include "adminchangeprice.h"
#include "ui_adminchangeprice.h"
#include "modsouvenirs.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

<<<<<<< HEAD
adminChangePrice::adminChangePrice(const QString &stadium, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminChangePrice)
    , m_stadium(stadium.trimmed())
=======
/*
 * Function: adminChangePrice constructor
 * Purpose : Initializes the change price window, stores the
 *           selected campus, and loads its souvenir list.
 */
adminChangePrice::adminChangePrice(const QString &campus, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adminChangePrice)
    , m_campus(campus.trimmed())
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
{
    ui->setupUi(this);
    setWindowTitle("Change Price");

    if (!ensureDbOpen())
    {
<<<<<<< HEAD
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return;
    }

    loadSouvenirsForStadium();
}

=======
        QMessageBox::critical(this, "Database Error",
                              "Could not open college_tour.sqlite.");
        return;
    }

    loadSouvenirsForCampus();
}

/*
 * Function: ~adminChangePrice
 * Purpose : Cleans up UI resources when the window closes.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
adminChangePrice::~adminChangePrice()
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
bool adminChangePrice::ensureDbOpen()
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
void adminChangePrice::loadSouvenirsForStadium()
{
    ui->changePrCombo->clear();

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

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Query Error",
                             "Could not load souvenirs for this stadium:\n" +
=======
/*
 * Function: loadSouvenirsForCampus
 * Purpose : Loads all souvenirs for the selected campus into
 *           the combo box so one can be chosen for price editing.
 */
void adminChangePrice::loadSouvenirsForCampus()
{
    ui->changePrCombo->clear();

    if (m_campus.isEmpty())
        return;

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT item
        FROM souvenirs
        WHERE TRIM(campus) = :campus
          AND TRIM(item) <> ''
        ORDER BY item ASC
    )");
    q.bindValue(":campus", m_campus);

    if (!q.exec())
    {
        QMessageBox::warning(this, "Query Error",
                             "Could not load souvenirs for this college:\n" +
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                                 q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString item = q.value(0).toString().trimmed();
<<<<<<< HEAD

        if (!item.isEmpty())
            ui->changePrCombo->addItem(item);
    }

    ui->changePrCombo->setCurrentIndex(-1);
}

=======
        if (!item.isEmpty())
            ui->changePrCombo->addItem(item);
    }
}

/*
 * Function: on_changePrConfirm_clicked
 * Purpose : Validates the new price and updates the selected
 *           souvenir price in the database.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void adminChangePrice::on_changePrConfirm_clicked()
{
    const QString item = ui->changePrCombo->currentText().trimmed();
    const QString priceText = ui->enterNewPrice->text().trimmed();

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

    if (item.isEmpty())
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Missing Souvenir",
=======
        QMessageBox::warning(this, "Missing Souvenir",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Select a souvenir first.");
        return;
    }

    bool ok = false;
    const double newPrice = priceText.toDouble(&ok);

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

    if (newPrice <= 0.0)
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

    QSqlQuery q(QSqlDatabase::database());
<<<<<<< HEAD

    q.prepare(R"(
        UPDATE SouvenirList
        SET Price = :price
        WHERE TRIM(Stadium) = TRIM(:stadium)
          AND TRIM(Name) = TRIM(:item)
    )");

    q.bindValue(":price", newPrice);
    q.bindValue(":stadium", m_stadium);
=======
    q.prepare(R"(
        UPDATE souvenirs
        SET price = :price
        WHERE TRIM(campus) = :campus
          AND TRIM(item) = :item
    )");
    q.bindValue(":price", newPrice);
    q.bindValue(":campus", m_campus);
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    q.bindValue(":item", item);

    if (!q.exec())
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Update Error",
=======
        QMessageBox::warning(this, "Update Error",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "Could not update souvenir price:\n" +
                                 q.lastError().text());
        return;
    }

    if (q.numRowsAffected() <= 0)
    {
<<<<<<< HEAD
        QMessageBox::warning(this,
                             "Update Error",
=======
        QMessageBox::warning(this, "Update Error",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "No souvenir price was updated.");
        return;
    }

<<<<<<< HEAD
    QMessageBox::information(this,
                             "Price Updated",
=======
    QMessageBox::information(this, "Price Updated",
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
                             "The souvenir price was updated successfully.");

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

<<<<<<< HEAD
=======
/*
 * Function: on_changePrBack_clicked
 * Purpose : Returns the user to the souvenir modification window
 *           without changing any souvenir prices.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
void adminChangePrice::on_changePrBack_clicked()
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
