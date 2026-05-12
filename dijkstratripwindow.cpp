#include "dijkstratripwindow.h"
#include "ui_dijkstratripwindow.h"

DijkstraTripWindow::DijkstraTripWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DijkstraTripWindow)
{
    ui->setupUi(this);
    setWindowTitle("Shortest Trip From Dodger Stadium");

    ui->selectEndingStadiumDropdownDT->clear();

    if (ensureDbOpen())
        loadStadiumsFromDb();

    // No default Stadium selected
    ui->selectEndingStadiumDropdownDT->setCurrentIndex(-1);
}

DijkstraTripWindow::~DijkstraTripWindow()
{
    delete ui;
}

bool DijkstraTripWindow::ensureDbOpen()
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

    // Look in exe dir and walk upward a few levels (helps when you keep the DB in the project root)
    QDir d(exeDir);
    for (int i = 0; i < 6; ++i)
    {
        candidates << d.filePath("BaseballDatabase.sqlite");
        if (!d.cdUp())
            break;
    }

    // Also try current working directory (Qt Creator often sets this to the build folder)
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

void DijkstraTripWindow::loadStadiumsFromDb()
{
    ui->selectEndingStadiumDropdownDT->clear();
    allStadiums.clear();

    QSqlQuery q(QSqlDatabase::database());

    q.prepare(R"(
        SELECT DISTINCT TRIM(sa.stadium) AS stadium
        FROM stadium_access sa
        WHERE sa.enabled = 1
          AND TRIM(sa.stadium) <> ''
          AND EXISTS (
              SELECT 1
              FROM MLBDistances d
              WHERE TRIM(d."Beginning Stadium") = TRIM(sa.stadium)
                 OR TRIM(d."Ending Stadium") = TRIM(sa.stadium)
          )
        ORDER BY TRIM(sa.stadium) ASC
    )");

    if (!q.exec())
    {
        QMessageBox::warning(this,
                             "Query Error",
                             "Could not load enabled ending stadiums:\n" +
                                 q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString stadium = q.value(0).toString().trimmed();

        if (stadium.isEmpty())
            continue;

        ui->selectEndingStadiumDropdownDT->addItem(stadium);
    }

    QSqlQuery graphQ(QSqlDatabase::database());

    graphQ.prepare(R"(
        SELECT DISTINCT TRIM(sa.stadium) AS stadium
        FROM stadium_access sa
        WHERE sa.enabled IN (0, 1)
          AND TRIM(sa.stadium) <> ''
          AND EXISTS (
              SELECT 1
              FROM MLBDistances d
              WHERE TRIM(d."Beginning Stadium") = TRIM(sa.stadium)
                 OR TRIM(d."Ending Stadium") = TRIM(sa.stadium)
          )
        ORDER BY TRIM(sa.stadium) ASC
    )");

    if (!graphQ.exec())
    {
        QMessageBox::warning(this,
                             "Query Error",
                             "Could not load Dijkstra graph stadiums:\n" +
                                 graphQ.lastError().text());
        return;
    }

    while (graphQ.next())
    {
        const QString stadium = graphQ.value(0).toString().trimmed();

        if (stadium.isEmpty())
            continue;

        vertex newStadium = {stadium, false};
        allStadiums.push_back(newStadium);
    }
}

void DijkstraTripWindow::on_startTripButtonDT_clicked()
{
    const QString end =
        ui->selectEndingStadiumDropdownDT->currentText().trimmed();

    if (end.isEmpty())
    {
        QMessageBox::information(this, "Select End", "Please select an ending stadium.");
        return;
    }

    if (allStadiums.isEmpty())
    {
        QMessageBox::warning(this, "No Stadiums Loaded", "The stadium graph could not be loaded.");
        return;
    }

    stadiumOrder.clear();

    for (vertex &stadium : allStadiums)
    {
        stadium.visited = false;
    }

    smalls = QVector<int>(allStadiums.size(), 5000);
    parents = QVector<int>(allStadiums.size(), -1);

    int num = 0;

    const int startIndex = findStadium("Dodger Stadium");

    if (startIndex < 0 || startIndex >= allStadiums.size())
    {
        QMessageBox::warning(this, "Invalid Start", "Dodger Stadium was not found in the stadium graph.");
        return;
    }

    const int endIndex = findStadium(end);

    if (endIndex < 0 || endIndex >= allStadiums.size())
    {
        QMessageBox::warning(this, "Invalid End", "The selected ending stadium was not found in the stadium graph.");
        return;
    }

    smalls[startIndex] = 0;
    parents[startIndex] = -1;

    dijkstraAlgorithm(startIndex, parents[startIndex], num);

    if (smalls[endIndex] >= 5000)
    {
        QMessageBox::warning(this, "No Path Found", "No valid route from Dodger Stadium to the selected stadium was found.");
        return;
    }

    path(endIndex);

    if (stadiumOrder.size() < 2)
    {
        QMessageBox::warning(this, "Invalid Route", "The route could not be built.");
        return;
    }

    this->hide();

    tripWindow dlg("Dodger Stadium", stadiumOrder, stadiumOrder.size() - 1, /*forceExact=*/false, nullptr, "default");

    dlg.setModal(true);
    dlg.exec();

    this->close();
}

void DijkstraTripWindow::on_backButtonDT_clicked()
{
    MainWindow *mainWin = new MainWindow(nullptr);
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    mainWin->show();

    this->close();
}

int DijkstraTripWindow::findStadium(const QString &name) const
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

int DijkstraTripWindow::nextStadium() const
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

void DijkstraTripWindow::path(int index)
{
    if (index == -1)
    {
        return;
    }
    int parent = parents[index];
    path(parent);
    stadiumOrder << allStadiums[index].name;
    return;
}

void DijkstraTripWindow::dijkstraAlgorithm(int index, int parent, int &num)
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
        QMessageBox::warning(this, "Query Error",
                             "Could not load campus list:\n" + q.lastError().text());
        return;
    }

    while (q.next())
    {
        const QString s = q.value(0).toString().trimmed();
        const QString e = q.value(1).toString().trimmed();
        const int d = q.value(2).toInt();
        if (!s.isEmpty())
        {
            index = findStadium(e);
            if(index == -1)
            {
                QMessageBox::information(this, "Invalid Index",
                                         "Stadium was not found");
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

    index = nextStadium();
    dijkstraAlgorithm(index, parent, num);
}
