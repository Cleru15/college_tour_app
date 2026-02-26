#ifndef TRIPWINDOW_H
#define TRIPWINDOW_H

#include <QDialog>
#include <QHash>
#include <QSet>

#include <vector>
#include <queue>
#include <utility>
#include <cstddef>

QT_BEGIN_NAMESPACE
namespace Ui { class tripWindow; }
QT_END_NAMESPACE

class QLineEdit;
class QListWidget;
class QPushButton;

// Trip window: steps through a planned campus route.
// Uses:
//  - explicit recursion for nearest-neighbor (Story 4)
//  - explicit recursion + DP memo for most efficient route (Stories 3 / 7)
class tripWindow : public QDialog
{
    Q_OBJECT

public:
    explicit tripWindow(const QString &startCampus,
                        const QStringList &campuses,
                        int maxStops,
                        bool forceExact,
                        QWidget *parent = nullptr);
    ~tripWindow();

private slots:
    void onGoNextClicked();

private:
    Ui::tripWindow *ui;

// =================================================================
// Vector-backed Binary Search Tree (BST)
//
// - Stored in a std::vector (left/right are indices, not pointers)
// - Insert and find are implemented recursively
// - Exposes a FIFO queue view backed by a vector
//   resetQueueInOrder() builds the queue via recursive inorder traversal,
//   then queuePop() pops items in FIFO order.
// =================================================================
struct VectorBst
{
    struct Node
    {
        int key = 0;           // destination index
        double value = 0.0;    // miles
        int left = -1;
        int right = -1;
    };

    int root = -1;
    std::vector<Node> nodes;

    // Queue view (FIFO) built from the BST via recursion.
    mutable std::vector<std::pair<int, double>> q;
    mutable size_t qHead = 0;

    void clear()
    {
        root = -1;
        nodes.clear();
        q.clear();
        qHead = 0;
    }

    void insert(int key, double value)
    {
        root = insertRec(root, key, value); // recursion
    }

    bool find(int key, double &out) const
    {
        return findRec(root, key, out); // recursion
    }

    void resetQueueInOrder() const
    {
        q.clear();
        qHead = 0;
        inorderRec(root); // recursion
    }

    bool queueEmpty() const
    {
        return qHead >= q.size();
    }

    std::pair<int, double> queuePop() const
    {
        if (queueEmpty())
            return std::make_pair(-1, 0.0);
        return q[qHead++];
    }

private:
    int insertRec(int cur, int key, double value)
    {
        if (cur == -1)
        {
            nodes.push_back(Node{key, value, -1, -1});
            return static_cast<int>(nodes.size()) - 1;
        }

        Node &n = nodes[static_cast<size_t>(cur)];
        if (key < n.key)
            n.left = insertRec(n.left, key, value);
        else if (key > n.key)
            n.right = insertRec(n.right, key, value);
        else
            n.value = value;

        return cur;
    }

    bool findRec(int cur, int key, double &out) const
    {
        if (cur == -1)
            return false;

        const Node &n = nodes[static_cast<size_t>(cur)];
        if (key == n.key)
        {
            out = n.value;
            return true;
        }

        if (key < n.key)
            return findRec(n.left, key, out);
        return findRec(n.right, key, out);
    }

    void inorderRec(int cur) const
    {
        if (cur == -1)
            return;

        const Node &n = nodes[static_cast<size_t>(cur)];
        inorderRec(n.left);
        q.push_back(std::make_pair(n.key, n.value));
        inorderRec(n.right);
    }
};

    // UI elements we add at runtime (so you don't have to redesign the .ui)
    QLineEdit   *m_currentEdit   = nullptr;
    QLineEdit   *m_totalEdit     = nullptr;
    QLineEdit   *m_souvenirCount = nullptr;
    QLineEdit   *m_souvenirCost  = nullptr;
    QLineEdit   *m_nextEdit      = nullptr;
    QLineEdit   *m_nextDistEdit  = nullptr;
    QListWidget *m_routeList     = nullptr;
    QPushButton *m_goNextBtn     = nullptr;

    void buildRuntimeUi();
    void setReadOnly(QLineEdit *edit);

    // ---- Data / routing ----
    static QString norm(const QString &s);
    bool ensureDbOpen();
    void loadAllDistances();
    double dist(const QString &a, const QString &b) const;

    QString m_start;
    std::vector<QString> m_candidates; // unique campuses eligible to visit (includes start)
    int m_maxStops = 1;
    bool m_forceExact = false;

    // Distance map: from -> (to -> miles)
    QHash<QString, QHash<QString, double>> m_dist;

    // Planned route
    std::vector<QString> m_route;
    std::vector<double>  m_legs;
    int m_index = 0;
    double m_traveled = 0.0;

    // Queue of remaining steps: (nextCampus, milesToNext)
    std::queue<std::pair<QString, double>> m_stepQueue;
    void buildStepQueue();

    // Recursive nearest-neighbor (partial trips)
    void planNearestNeighbor();
    void nearestStepRecursive(const QString &current, QSet<QString> &visited);

    // Recursive exact DP (most efficient)
    void planExactDp();
    double tspRecursive(int mask, int last);

    // DP buffers
    std::vector<QString> m_nodes;
    std::vector<VectorBst> m_adj; // adjacency as vector<BST>
    std::vector<std::vector<double>> m_memo;
    std::vector<std::vector<int>> m_next;
    int m_allMask = 0;

    // UI refresh
    void refreshUi();
    void populateRouteList();
};

#endif // TRIPWINDOW_H
