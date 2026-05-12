#include "dijkstraalgo.h"

//bool ensureDbOpen();
QVector<vertex> loadStadiumsFromDb()
{
    QVector<vertex> allStadiums;
    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT DISTINCT "Beginning Stadium"
        FROM MLBDistances
    )");

    if (!q.exec())
    {
        return allStadiums;
    }

    while (q.next())
    {
        const QString c = q.value(0).toString().trimmed();
        vertex newStadium = {c, false};
        allStadiums.push_back(newStadium);
    }
    return allStadiums;
}

int findStadium(QString name, QVector<vertex> allStadiums)
{
    for (int j = 0; j < allStadiums.size(); j++)
    {
        if (allStadiums[j].name == name)
        {
            return j;
        }
    }
    return -1;
}

int nextStadium(QVector<vertex> allStadiums, int smalls[])
{
    int shortestDist = 5000;
    int newIndex = -1;
    for (int i = 0; i < allStadiums.size(); i++)
    {
        if (!allStadiums[i].visited && smalls[i] < shortestDist)
        {
            shortestDist = smalls[i];
            newIndex = i;
        }
    }
    return newIndex;
}

void dijkstraAlgorithm(int index, int parent, int &num, QVector<vertex> allStadiums, int smalls[], int parents[])
{
    //base case if the list is full
    if (index < 0 || index >= allStadiums.size() || num >= allStadiums.size())
    {
        return;
    }

    //checking if this vertex has been visited
    //if not, visit this stadium
    if(!allStadiums[index].visited)
    {
        allStadiums[index].visited = true;
        num++;
    }

    //going through adjacent vertices to adjust the costs

    //setting the index to be the parent of any new children
    parent = index;

    //selecting the starting stadium of the parent
    QString currentStadium = allStadiums[parent].name;

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT
            "Beginning Stadium", "Ending Stadium", "Distance"
        FROM MLBDistances
        WHERE "Beginning Stadium" = :begin
    )");
    q.bindValue(":begin", currentStadium);

    if (!q.exec())
    {
        return;
    }

    while (q.next())
    {
        const QString s = q.value(0).toString().trimmed();
        const QString e = q.value(1).toString().trimmed();
        const int d = q.value(2).toInt();
        if (!s.isEmpty())
        {
            index = findStadium(e, allStadiums);
            if(index == -1)
            {
                return;
            }
            if(!allStadiums[index].visited)
            {
                int newSmalls = smalls[parent] + d;
                if(smalls[index] > newSmalls)
                {
                    smalls[index] = newSmalls;
                    parents[index] = parent;
                }
            }
        }
    }

    index = nextStadium(allStadiums, smalls);
    dijkstraAlgorithm(index, parent, num, allStadiums, smalls, parents);
}
