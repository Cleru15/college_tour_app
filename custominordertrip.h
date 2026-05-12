#ifndef CUSTOMINORDERTRIP_H
#define CUSTOMINORDERTRIP_H

#include <QMainWindow>
#include "tripwindow.h"
#include "mainwindow.h"

namespace Ui {
class CustomInOrderTrip;
}

class CustomInOrderTrip : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomInOrderTrip(QWidget *parent = nullptr);
    ~CustomInOrderTrip();

private slots:
    void on_confirmButtonCOT_clicked();
    void on_confirmButtonCOT_2_clicked();
    void on_startTripButtonCOT_clicked();
    void on_backButtCOT_clicked();

private:
    Ui::CustomInOrderTrip *ui;

    bool ensureDbOpen();
    QStringList loadStadiums();

    QStringList selected;
    QStringList remaining;

    QListWidget *m_tripList     = nullptr;

    QString m_startLocked;
    QStringList m_allStadiums;

};

#endif // CUSTOMINORDERTRIP_H
