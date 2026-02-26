#include "tripwindow.h"
#include "ui_tripwindow.h"

#include "summarywindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static constexpr double BIG = 1e18;

tripWindow::tripWindow(const QString &startCampus,
                       const QStringList &campuses,
                       int maxStops,
                       bool forceExact,
                       QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::tripWindow)
    , m_start(norm(startCampus))
    , m_maxStops(maxStops)
    , m_forceExact(forceExact)
{
    ui->setupUi(this);

    // Repurpose the existing button as "Go to Next College"
    m_goNextBtn = ui->pushButton;
    if (m_goNextBtn)
    {
        m_goNextBtn->setText("Go to Next College");
        connect(m_goNextBtn, &QPushButton::clicked, this, &tripWindow::onGoNextClicked);
    }

    // Build candidates (unique, includes start)
    QSet<QString> seen;
    for (const QString &c : campuses)
    {
        const QString n = norm(c);
        if (!n.isEmpty() && !seen.contains(n))
        {
            seen.insert(n);
            m_candidates.push_back(n);
        }
    }
    if (!m_start.isEmpty() && !seen.contains(m_start))
        m_candidates.insert(m_candidates.begin(), m_start);

    if (m_candidates.empty())
        m_candidates.push_back(m_start);

    if (m_maxStops < 1)
        m_maxStops = 1;
    if (m_maxStops > static_cast<int>(m_candidates.size()))
        m_maxStops = static_cast<int>(m_candidates.size());

    buildRuntimeUi();

    if (!ensureDbOpen())
    {
        QMessageBox::warning(this, "Database Error",
                             "Could not open college_tour.sqlite.\nDistances will be unavailable.");
    }
    loadAllDistances();

    // Plan route
    if (m_forceExact || m_maxStops == static_cast<int>(m_candidates.size()))
        planExactDp();
    else
        planNearestNeighbor();

    buildStepQueue();
    populateRouteList();
    refreshUi();
}

tripWindow::~tripWindow()
{
    delete ui;
}

QString tripWindow::norm(const QString &s)
{
    return s.trimmed();
}

bool tripWindow::ensureDbOpen()
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
        candidates << d.filePath("college_tour.sqlite");
        if (!d.cdUp())
            break;
    }

    // Also try current working directory (Qt Creator often sets this to the build folder)
    candidates << QDir::current().filePath("college_tour.sqlite");

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

void tripWindow::loadAllDistances()
{
    m_dist.clear();

    if (!QSqlDatabase::contains() || !QSqlDatabase::database().isOpen())
        return;

    QSqlQuery q(QSqlDatabase::database());
    q.prepare(R"(
        SELECT TRIM(from_campus), TRIM(to_campus), miles
        FROM distances
        WHERE TRIM(from_campus) <> '' AND TRIM(to_campus) <> ''
    )");

    if (!q.exec())
        return;

    while (q.next())
    {
        const QString a = norm(q.value(0).toString());
        const QString b = norm(q.value(1).toString());
        const double miles = q.value(2).toDouble();

        if (a.isEmpty() || b.isEmpty())
            continue;

        m_dist[a][b] = miles;
    }
}

double tripWindow::dist(const QString &a, const QString &b) const
{
    if (a == b)
        return 0.0;

    const auto itA = m_dist.constFind(a);
    if (itA == m_dist.constEnd())
        return BIG;

    const auto itB = itA->constFind(b);
    if (itB == itA->constEnd())
        return BIG;

    return *itB;
}

void tripWindow::setReadOnly(QLineEdit *edit)
{
    if (!edit)
        return;
    edit->setReadOnly(true);
    edit->setFocusPolicy(Qt::NoFocus);
}

void tripWindow::buildRuntimeUi()
{
    // Replace the listView with a real list widget (easier to fill)
    if (ui->listView)
        ui->listView->hide();

    const QRect listRect = ui->listView
                               ? ui->listView->geometry()
                               : QRect(260, 30, 600, 420);

    m_routeList = new QListWidget(this);
    m_routeList->setGeometry(listRect);

    // Create line edits next to existing labels in the .ui
    auto makeEditNextTo = [&](QLabel *lbl) -> QLineEdit* {
        if (!lbl) return nullptr;
        const QRect r = lbl->geometry();
        auto *e = new QLineEdit(this);
        e->setGeometry(r.x() + r.width() + 10, r.y() - 2, 260, r.height() + 6);
        setReadOnly(e);
        return e;
    };

    m_nextEdit      = makeEditNextTo(ui->label);    // Next College
    m_nextDistEdit  = makeEditNextTo(ui->label_2);  // Dist to Next
    m_souvenirCount = makeEditNextTo(ui->label_3);  // Souvenirs purchased
    m_souvenirCost  = makeEditNextTo(ui->label_4);  // Souvenir cost
    m_currentEdit   = makeEditNextTo(ui->label_5);  // Current college
    m_totalEdit     = makeEditNextTo(ui->label_6);  // Distance traveled

    if (m_souvenirCount) m_souvenirCount->setText("0");
    if (m_souvenirCost)  m_souvenirCost->setText("$0.00");
}

void tripWindow::planNearestNeighbor()
{
    m_route.clear();
    m_legs.clear();
    m_index = 0;
    m_traveled = 0.0;

    QSet<QString> visited;
    visited.insert(m_start);

    m_route.push_back(m_start);
    nearestStepRecursive(m_start, visited);

    // Build legs for whatever route we ended with
    for (size_t i = 0; i + 1 < m_route.size(); ++i)
        m_legs.push_back(dist(m_route[i], m_route[i + 1]));
}

void tripWindow::nearestStepRecursive(const QString &current, QSet<QString> &visited)
{
    if (static_cast<int>(m_route.size()) >= m_maxStops)
        return; // base case

    QString best;
    double bestD = BIG;

    for (const QString &cand : m_candidates)
    {
        if (visited.contains(cand))
            continue;

        const double d = dist(current, cand);
        if (d < bestD)
        {
            bestD = d;
            best = cand;
        }
    }

    if (best.isEmpty() || bestD >= BIG/2)
        return; // no reachable next campus

    visited.insert(best);
    m_route.push_back(best);

    // ✅ explicit recursion
    nearestStepRecursive(best, visited);
}

void tripWindow::planExactDp()
{
    // Nodes: start first, then the rest
    m_nodes.clear();
    m_nodes.push_back(m_start);
    for (const QString &c : m_candidates)
    {
        if (c != m_start)
            m_nodes.push_back(c);
    }

    const int n = static_cast<int>(m_nodes.size());
    if (n <= 1)
    {
        m_route = {m_start};
        m_legs.clear();
        return;
    }


// Build adjacency as a vector of vector-backed BSTs (VectorBst).
// Each BST holds edges (destinationIndex -> miles) and is built with recursive insert().
m_adj.assign(n, VectorBst{});
for (int i = 0; i < n; ++i)
{
    m_adj[static_cast<size_t>(i)].clear();
    for (int j = 0; j < n; ++j)
    {
        const double d = dist(m_nodes[i], m_nodes[j]);
        if (d < BIG / 2)
            m_adj[static_cast<size_t>(i)].insert(j, d);
    }
}

    const int masks = 1 << n;
    m_allMask = masks - 1;
    m_memo.assign(masks, std::vector<double>(n, -1.0));
    m_next.assign(masks, std::vector<int>(n, -1));

    // Start recursion from start node only visited
    (void)tspRecursive(1, 0);

    // Reconstruct route
    m_route.clear();
    m_legs.clear();

    int mask = 1;
    int last = 0;
    m_route.push_back(m_nodes[0]);

    while (mask != m_allMask)
    {
        const int nxt = m_next[mask][last];
        if (nxt < 0)
            break;

        m_route.push_back(m_nodes[nxt]);
        double w = BIG;
        (void)m_adj[static_cast<size_t>(last)].find(nxt, w);
        m_legs.push_back(w);

        mask |= (1 << nxt);
        last = nxt;
    }
}


// MASK TRACKS CAMPUSES ALREADY VISITED
double tripWindow::tspRecursive(int mask, int last)
{
    if (mask == m_allMask)
        return 0.0; // base case

    double &memo = m_memo[mask][last];
    if (memo >= 0.0)
        return memo;

    double best = BIG;
    int bestNext = -1;


// Use the BST queue view to iterate outgoing edges.
// resetQueueInOrder() traverses the BST recursively and fills a FIFO vector queue.
m_adj[static_cast<size_t>(last)].resetQueueInOrder();

while (!m_adj[static_cast<size_t>(last)].queueEmpty())
{
    const auto edge = m_adj[static_cast<size_t>(last)].queuePop();
    const int nxt = edge.first;
    const double w = edge.second;

    if (nxt < 0)
        break;

    if (mask & (1 << nxt))
        continue;

    if (w >= BIG/2)
        continue;

    // ✅ explicit recursion
    const double cand = w + tspRecursive(mask | (1 << nxt), nxt);
    if (cand < best)
    {
        best = cand;
        bestNext = nxt;
    }
}

    m_next[mask][last] = bestNext;
    memo = best;
    return memo;
}

void tripWindow::buildStepQueue()
{
    while (!m_stepQueue.empty())
        m_stepQueue.pop();

    // Remaining steps start from route[1]
    for (size_t i = 1; i < m_route.size(); ++i)
    {
        const double d = (i - 1 < m_legs.size()) ? m_legs[i - 1] : 0.0;
        m_stepQueue.push({m_route[i], d});
    }
}

void tripWindow::populateRouteList()
{
    if (!m_routeList)
        return;

    m_routeList->clear();

    for (size_t i = 0; i < m_route.size(); ++i)
    {
        if (i == 0)
        {
            m_routeList->addItem(QString("Start: %1").arg(m_route[i]));
        }
        else
        {
            const double d = (i - 1 < m_legs.size()) ? m_legs[i - 1] : 0.0;
            m_routeList->addItem(QString("%1) %2  (+%3 miles)")
                                     .arg(static_cast<int>(i))
                                     .arg(m_route[i])
                                     .arg(d, 0, 'f', 1));
        }
    }
}

void tripWindow::refreshUi()
{
    if (!m_currentEdit || !m_totalEdit || !m_nextEdit || !m_nextDistEdit)
        return;

    if (m_index < 0 || m_index >= static_cast<int>(m_route.size()))
        return;

    m_currentEdit->setText(m_route[m_index]);
    m_totalEdit->setText(QString::number(m_traveled, 'f', 1));

    if (!m_stepQueue.empty())
    {
        const auto &front = m_stepQueue.front();
        m_nextEdit->setText(front.first);
        m_nextDistEdit->setText(QString::number(front.second, 'f', 1));
        if (m_goNextBtn) m_goNextBtn->setText("Go to Next College");
    }
    else
    {
        m_nextEdit->setText("N/A");
        m_nextDistEdit->setText("0.0");
        if (m_goNextBtn) m_goNextBtn->setText("Finish Tour");
    }
}

void tripWindow::onGoNextClicked()
{
    // Advance if steps remain
    if (!m_stepQueue.empty())
    {
        const auto step = m_stepQueue.front();
        m_stepQueue.pop();

        ++m_index;
        m_traveled += step.second;

        refreshUi();
        return;
    }

    // No steps left -> show summary
    summaryWindow dlg(m_route, m_legs, m_traveled, this);
    dlg.setModal(true);
    dlg.exec();

    accept(); // closes trip window
}
