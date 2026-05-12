#include "modinfo.h"
#include "ui_modinfo.h"

#include "modstadiums.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QSignalBlocker>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

modinfo::modinfo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::modinfo)
{
    ui->setupUi(this);

    setWindowTitle("Modify Stadium Information");

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return;
    }

    loadStadiumDropdown();
    resetToDefaultState();
}

modinfo::~modinfo()
{
    delete ui;
}

bool modinfo::ensureDbOpen()
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

void modinfo::loadStadiumDropdown()
{
    QSignalBlocker blocker(ui->comboBoxStadium);

    ui->comboBoxStadium->clear();
    ui->comboBoxStadium->addItem("Select a Stadium");

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT DISTINCT TRIM(i."Stadium name")
        FROM MLBInformation i
        JOIN stadium_access sa
          ON TRIM(sa.stadium) = TRIM(i."Stadium name")
        WHERE sa.enabled IN (0, 1)
          AND TRIM(i."Stadium name") <> ''
        ORDER BY TRIM(i."Stadium name") ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Query Error",
                             "Could not load stadium list:\n" +
                                 q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString stadium = q.value(0).toString().trimmed();

        if (!stadium.isEmpty())
            ui->comboBoxStadium->addItem(stadium);
    }

    ui->comboBoxStadium->setCurrentIndex(0);
}

void modinfo::resetToDefaultState()
{
    m_selectedStadium.clear();
    m_selectedField.clear();

    {
        QSignalBlocker stadiumBlocker(ui->comboBoxStadium);
        QSignalBlocker fieldBlocker(ui->comboBoxField);

        ui->comboBoxStadium->setCurrentIndex(0);
        ui->comboBoxField->setCurrentIndex(0);
    }

    ui->comboBoxStadium->setEnabled(true);
    ui->comboBoxField->setEnabled(true);
    ui->buttonSubmit->setEnabled(true);

    ui->updatedField->clear();

    showEditControls(false);
}

void modinfo::showEditControls(bool show)
{
    ui->labelSubtitle_2->setVisible(show);
    ui->updatedField->setVisible(show);
    ui->buttonConfirm->setVisible(show);
}

bool modinfo::isIntegerField(const QString &field) const
{
    return field.compare("Capacity", Qt::CaseInsensitive) == 0 ||
           field.compare("Date opened", Qt::CaseInsensitive) == 0 ||
           field.compare("Distance to center field (ft)", Qt::CaseInsensitive) == 0;
}

QString modinfo::databaseColumnForField(const QString &field) const
{
    if (field.compare("Capacity", Qt::CaseInsensitive) == 0)
        return "Seating Capacity";

    if (field.compare("Stadium name", Qt::CaseInsensitive) == 0)
        return "Stadium name";

    if (field.compare("Playing surface", Qt::CaseInsensitive) == 0)
        return "Playing Surface";

    if (field.compare("Roof type", Qt::CaseInsensitive) == 0)
        return "Roof Type";

    if (field.compare("Ballpark typology", Qt::CaseInsensitive) == 0)
        return "Ballpark typology";

    if (field.compare("Date opened", Qt::CaseInsensitive) == 0)
        return "Date Opened";

    if (field.compare("Distance to center field (ft)", Qt::CaseInsensitive) == 0)
        return "Distance to center field";

    if (field.compare("Location", Qt::CaseInsensitive) == 0)
        return "Location";

    return QString();
}

QString modinfo::formattedValueForField(const QString &field,
                                        const QString &rawInput,
                                        bool &ok,
                                        QString &errorMessage) const
{
    ok = false;
    errorMessage.clear();

    const QString trimmed = rawInput.trimmed();

    if (trimmed.isEmpty())
    {
        errorMessage = "The new field value cannot be blank.";
        return QString();
    }

    if (!isIntegerField(field))
    {
        ok = true;
        return trimmed;
    }

    bool conversionOk = false;
    const int value = trimmed.toInt(&conversionOk);

    if (!conversionOk)
    {
        errorMessage = "This field requires an integer value.";
        return QString();
    }

    if (value < 0)
    {
        errorMessage = "The integer value cannot be negative.";
        return QString();
    }

    if (field.compare("Date opened", Qt::CaseInsensitive) == 0)
    {
        if (value < 1800 || value > 3000)
        {
            errorMessage = "Enter a valid year for Date opened.";
            return QString();
        }

        ok = true;
        return QString::number(value);
    }

    if (field.compare("Capacity", Qt::CaseInsensitive) == 0)
    {
        if (value == 0)
        {
            errorMessage = "Capacity must be greater than 0.";
            return QString();
        }

        ok = true;
        return QString::number(value);
    }

    if (field.compare("Distance to center field (ft)", Qt::CaseInsensitive) == 0)
    {
        if (value == 0)
        {
            errorMessage = "Distance to center field must be greater than 0.";
            return QString();
        }

        const int meters = qRound(value * 0.3048);

        ok = true;
        return QString("%1 feet (%2m)").arg(value).arg(meters);
    }

    ok = true;
    return QString::number(value);
}

void modinfo::on_buttonSubmit_clicked()
{
    const QString stadium = ui->comboBoxStadium->currentText().trimmed();
    const QString field = ui->comboBoxField->currentText().trimmed();

    if (stadium.isEmpty() ||
        stadium.compare("Select a Stadium", Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(this,
                                 "Select Stadium",
                                 "Please select a stadium first.");
        return;
    }

    if (field.isEmpty() ||
        field.compare("Select a Field", Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(this,
                                 "Select Field",
                                 "Please select a field first.");
        return;
    }

    m_selectedStadium = stadium;
    m_selectedField = field;

    ui->comboBoxStadium->setEnabled(false);
    ui->comboBoxField->setEnabled(false);
    ui->buttonSubmit->setEnabled(false);

    ui->updatedField->clear();

    if (isIntegerField(m_selectedField))
    {
        ui->labelSubtitle_2->setText("Enter the new integer value:");
    }
    else
    {
        ui->labelSubtitle_2->setText("Enter the new field value:");
    }

    showEditControls(true);
}

void modinfo::on_buttonCancel_clicked()
{
    resetToDefaultState();
}

void modinfo::on_buttonBack_clicked()
{
    auto *win = new ModStadiums(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

void modinfo::on_buttonConfirm_clicked()
{
    if (m_selectedStadium.isEmpty() || m_selectedField.isEmpty())
    {
        QMessageBox::warning(this,
                             "Missing Selection",
                             "Select a stadium and field first.");
        return;
    }

    bool valueOk = false;
    QString errorMessage;

    const QString newValue =
        formattedValueForField(m_selectedField,
                               ui->updatedField->text(),
                               valueOk,
                               errorMessage);

    if (!valueOk)
    {
        QMessageBox::warning(this,
                             "Invalid Input",
                             errorMessage);
        return;
    }

    const QString columnName = databaseColumnForField(m_selectedField);

    if (columnName.isEmpty())
    {
        QMessageBox::warning(this,
                             "Invalid Field",
                             "The selected field is not recognized.");
        return;
    }

    bool updateOk = false;

    if (columnName.compare("Stadium name", Qt::CaseInsensitive) == 0)
    {
        updateOk = updateStadiumName(m_selectedStadium, newValue);
    }
    else
    {
        updateOk = updateRegularField(columnName, newValue);
    }

    if (!updateOk)
        return;

    QMessageBox::information(this,
                             "Update Successful",
                             "The data field has been successfully updated");

    auto *win = new ModStadiums(nullptr);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();

    this->close();
}

bool modinfo::updateRegularField(const QString &columnName,
                                 const QString &value)
{
    if (!ensureDbOpen())
    {
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return false;
    }

    QSqlQuery q(QSqlDatabase::database());

    const QString sql =
        QString(R"(
            UPDATE MLBInformation
            SET "%1" = :value
            WHERE TRIM("Stadium name") = TRIM(:stadium)
        )").arg(columnName);

    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":stadium", m_selectedStadium);

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Update Error",
                             "Could not update stadium information:\n" +
                                 q.lastError().text());
        return false;
    }

    if (q.numRowsAffected() <= 0)
    {
        QMessageBox::warning(this,
                             "Update Error",
                             "No stadium information was updated.");
        return false;
    }

    return true;
}

bool modinfo::updateStadiumName(const QString &oldStadiumName,
                                const QString &newStadiumName)
{
    const QString oldName = oldStadiumName.trimmed();
    const QString newName = newStadiumName.trimmed();

    if (oldName.isEmpty() || newName.isEmpty())
    {
        QMessageBox::warning(this,
                             "Invalid Stadium Name",
                             "Stadium name cannot be blank.");
        return false;
    }

    if (oldName.compare(newName, Qt::CaseInsensitive) == 0)
    {
        QMessageBox::information(this,
                                 "No Change",
                                 "The new stadium name is the same as the current stadium name.");
        return false;
    }

    if (!ensureDbOpen())
    {
        QMessageBox::critical(this,
                              "Database Error",
                              "Could not open BaseballDatabase.sqlite.");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery duplicateQ(db);

    duplicateQ.prepare(R"(
        SELECT COUNT(*)
        FROM MLBInformation
        WHERE TRIM("Stadium name") = TRIM(:newName)
    )");

    duplicateQ.bindValue(":newName", newName);

    if (!duplicateQ.exec() || !duplicateQ.next())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not check for duplicate stadium names.");
        return false;
    }

    if (duplicateQ.value(0).toInt() > 0)
    {
        QMessageBox::warning(this,
                             "Duplicate Stadium",
                             "Another stadium already uses that name.");
        return false;
    }

    if (!db.transaction())
    {
        QMessageBox::warning(this,
                             "Database Error",
                             "Could not start database transaction.");
        return false;
    }

    auto execOrRollback = [&](QSqlQuery &query, const QString &message) -> bool
    {
        if (!query.exec())
        {
            db.rollback();

            QMessageBox::warning(this,
                                 "Update Error",
                                 message + "\n" + query.lastError().text());
            return false;
        }

        return true;
    };

    QSqlQuery q1(db);
    q1.prepare(R"(
        UPDATE MLBInformation
        SET "Stadium name" = :newName
        WHERE TRIM("Stadium name") = TRIM(:oldName)
    )");
    q1.bindValue(":newName", newName);
    q1.bindValue(":oldName", oldName);

    if (!execOrRollback(q1, "Could not update MLBInformation."))
        return false;

    QSqlQuery q2(db);
    q2.prepare(R"(
        UPDATE stadium_access
        SET stadium = :newName
        WHERE TRIM(stadium) = TRIM(:oldName)
    )");
    q2.bindValue(":newName", newName);
    q2.bindValue(":oldName", oldName);

    if (!execOrRollback(q2, "Could not update stadium_access."))
        return false;

    QSqlQuery q3(db);
    q3.prepare(R"(
        UPDATE MLBDistances
        SET "Beginning Stadium" = :newName
        WHERE TRIM("Beginning Stadium") = TRIM(:oldName)
    )");
    q3.bindValue(":newName", newName);
    q3.bindValue(":oldName", oldName);

    if (!execOrRollback(q3, "Could not update MLBDistances beginning stadiums."))
        return false;

    QSqlQuery q4(db);
    q4.prepare(R"(
        UPDATE MLBDistances
        SET "Ending Stadium" = :newName
        WHERE TRIM("Ending Stadium") = TRIM(:oldName)
    )");
    q4.bindValue(":newName", newName);
    q4.bindValue(":oldName", oldName);

    if (!execOrRollback(q4, "Could not update MLBDistances ending stadiums."))
        return false;

    QSqlQuery q5(db);
    q5.prepare(R"(
        UPDATE SouvenirList
        SET Stadium = :newName
        WHERE TRIM(Stadium) = TRIM(:oldName)
    )");
    q5.bindValue(":newName", newName);
    q5.bindValue(":oldName", oldName);

    if (!execOrRollback(q5, "Could not update SouvenirList."))
        return false;

    QSqlQuery q6(db);
    q6.prepare(R"(
        UPDATE souvenir_access
        SET stadium = :newName
        WHERE TRIM(stadium) = TRIM(:oldName)
    )");
    q6.bindValue(":newName", newName);
    q6.bindValue(":oldName", oldName);

    if (!execOrRollback(q6, "Could not update souvenir_access."))
        return false;

    if (!db.commit())
    {
        db.rollback();

        QMessageBox::warning(this,
                             "Database Error",
                             "Could not commit stadium name update.");
        return false;
    }

    return true;
}
