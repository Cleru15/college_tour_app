#ifndef DIJKSTRAALGO_H
#define DIJKSTRAALGO_H

//#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <vector>

struct vertex
{
    QString name;
    bool visited = false;
};

QVector<vertex> loadStadiumsFromDb();

int findStadium(QString name, QVector<vertex> allStadiums);
int nextStadium(QVector<vertex> allStadiums, int smalls[]);

void dijkstraAlgorithm(int index, int parent, int &num, QVector<vertex> allStadiums, int smalls[], int parents[]);
#endif // DIJKSTRAALGO_H
