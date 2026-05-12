#ifndef CUSTOMTRIPWINDOW_H
#define CUSTOMTRIPWINDOW_H

<<<<<<< HEAD
#include "tripwindow.h"
#include "mainwindow.h"

//#include <QCoreApplication>
//#include <QDir>
//#include <QFileInfo>
//#include <QMessageBox>
//#include <QTableWidgetItem>

//#include <QSqlDatabase>
//#include <QSqlError>
//#include <QSqlQuery>
//#include <QString>

//#include <QMainWindow>
=======
#include <QMainWindow>
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de

QT_BEGIN_NAMESPACE
namespace Ui { class CustomTripWindow; }
QT_END_NAMESPACE

class CustomTripWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomTripWindow(QWidget *parent = nullptr);
    ~CustomTripWindow();

private slots:
    void on_confirmButtonCT_clicked();
    void on_startTripButtonCT_clicked();
    void on_backButtCT_clicked();   // add this

private:
    Ui::CustomTripWindow *ui;

    bool ensureDbOpen();
<<<<<<< HEAD
    QStringList loadStadiums();

    QString m_startLocked;
    QStringList m_allStadiums;
=======
    QStringList loadCampuses();

    QString m_startLocked;
    QStringList m_allCampuses;
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
};

#endif // CUSTOMTRIPWINDOW_H
