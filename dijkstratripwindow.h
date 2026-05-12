#ifndef DIJKSTRATRIPWINDOW_H
#define DIJKSTRATRIPWINDOW_H

#include "tripwindow.h"
#include "mainwindow.h"

#include <QMainWindow>
#include <QStringList>
#include <QVector>

namespace Ui {
class DijkstraTripWindow;
}

class DijkstraTripWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DijkstraTripWindow(QWidget *parent = nullptr);
    ~DijkstraTripWindow();

private slots:
    void on_startTripButtonDT_clicked();
    void on_backButtonDT_clicked();

private:
    Ui::DijkstraTripWindow *ui;

    // Dynamic arrays for Dijkstra.
    // Size these to allStadiums.size() in the .cpp file.
    QVector<int> smalls;
    QVector<int> parents;

    QStringList stadiumOrder;
    QVector<vertex> allStadiums;

    bool ensureDbOpen();

    // Loads:
    // - dropdown stadiums where enabled = 1
    // - internal graph stadiums where enabled IN (0, 1)
    void loadStadiumsFromDb();

    int findStadium(const QString &name) const;
    int nextStadium() const;
    void path(int index);

    void dijkstraAlgorithm(int index, int parent, int &num);
};

#endif // DIJKSTRATRIPWINDOW_H
