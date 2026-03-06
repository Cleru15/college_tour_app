/**
 * @file tripwindow.h
 * @brief Defines the tripWindow class.
 *
 * Simulates a campus tour by calculating a route and allowing
 * the user to move through campuses step-by-step.
 */

// tripwindow.h defines the tripWindow dialog class.
// This window manages the campus trip simulation by calculating
// a route, stepping through campuses one at a time, and updating
// the UI with progress, distance, and summary information.

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

// Forward declarations for Qt classes used as pointers
class QLineEdit;
class QListWidget;
class QPushButton;

/*
 * Class: tripWindow
 * Purpose: Simulates a campus trip by planning a route and allowing
 *          the user to move through the trip one campus at a time.
 *          Supports both greedy nearest-neighbor and exact DP routing.
 */
class tripWindow : public QDialog
{
    Q_OBJECT

public:

    /*
     * Constructor
     * Purpose: Initializes the trip window, loads route data,
     *          calculates the trip path, and prepares the UI.
     */
    explicit tripWindow(const QString &startCampus,
                        const QStringList &campuses,
                        int maxStops,
                        bool forceExact,
                        QWidget *parent = nullptr);

    /*
     * Destructor
     * Purpose: Cleans up UI resources when the trip window closes.
     */
    ~tripWindow();

private slots:

    /*
     * Slot: onGoNextClicked
     * Purpose: Advances the trip to the next campus or finishes
     *          the trip and opens the summary window.
     */
    void onGoNextClicked();

private:

    // Pointer to the UI generated from tripwindow.ui
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

    /*
     * Struct: VectorBst
     * Purpose: Stores adjacency data using a vector-backed BST.
     *          Supports recursive insert, recursive find,
     *          and recursive inorder traversal.
     */
    struct VectorBst
    {
        /*
         * Struct: Node
         * Purpose: Represents one BST node storing a destination index,
         *          its distance value, and child indices.
         */
        struct Node
        {
            int key = 0;           // destination index
            double value = 0.0;    // miles
            int left = -1;
            int right = -1;
        };

        int root = -1;
        std::vector<Node> nodes;

        // Queue view built from the BST using inorder traversal
        mutable std::vector<std::pair<int, double>> q;
        mutable size_t qHead = 0;

        // Resets the BST and clears all stored nodes and queue data.
        void clear()
        {
            root = -1;
            nodes.clear();
            q.clear();
            qHead = 0;
        }

        // Inserts a key/value pair into the BST using recursion.
        void insert(int key, double value)
        {
            root = insertRec(root, key, value);
        }

        // Searches the BST recursively for a key and outputs its value.
        bool find(int key, double &out) const
        {
            return findRec(root, key, out);
        }

        // Rebuilds the queue view using recursive inorder traversal.
        void resetQueueInOrder() const
        {
            q.clear();
            qHead = 0;
            inorderRec(root);
        }

        // Returns true if the queue view has no remaining elements.
        bool queueEmpty() const
        {
            return qHead >= q.size();
        }

        // Removes and returns the next item from the queue view.
        std::pair<int, double> queuePop() const
        {
            if (queueEmpty())
                return std::make_pair(-1, 0.0);
            return q[qHead++];
        }

    private:

        // Recursive helper for inserting into the BST.
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

        // Recursive helper for searching the BST.
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

        // Recursive inorder traversal used to fill the queue view.
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

    // Runtime UI fields used to display current trip information
    QLineEdit   *m_currentEdit   = nullptr;
    QLineEdit   *m_totalEdit     = nullptr;
    QLineEdit   *m_souvenirCount = nullptr;
    QLineEdit   *m_souvenirCost  = nullptr;
    QLineEdit   *m_nextEdit      = nullptr;
    QLineEdit   *m_nextDistEdit  = nullptr;
    QListWidget *m_routeList     = nullptr;
    QPushButton *m_goNextBtn     = nullptr;

    /*
     * Function: buildRuntimeUi
     * Purpose : Creates the runtime UI widgets used to display
     *           route progress and trip statistics.
     */
    void buildRuntimeUi();

    /*
     * Function: setReadOnly
     * Purpose : Configures a QLineEdit so it can display values
     *           without allowing the user to edit them.
     */
    void setReadOnly(QLineEdit *edit);

    // ---- Data / routing ----

    /*
     * Function: norm
     * Purpose : Normalizes campus names by trimming whitespace.
     */
    static QString norm(const QString &s);

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection exists
     *           and is open before loading distance data.
     */
    bool ensureDbOpen();

    /*
     * Function: loadAllDistances
     * Purpose : Loads all campus-to-campus distance values
     *           from the database into memory.
     */
    void loadAllDistances();

    /*
     * Function: dist
     * Purpose : Returns the distance between two campuses
     *           using the loaded distance map.
     */
    double dist(const QString &a, const QString &b) const;

    // Stores the selected starting campus
    QString m_start;

    // Unique list of campuses eligible to be visited
    std::vector<QString> m_candidates;

    // Maximum number of campuses to include in the trip
    int m_maxStops = 1;

    // True if the exact optimal route should be computed
    bool m_forceExact = false;

    // Distance map: from campus -> (to campus -> miles)
    QHash<QString, QHash<QString, double>> m_dist;

    // Planned route and distances between consecutive campuses
    std::vector<QString> m_route;
    std::vector<double>  m_legs;

    // Current index in the route and total miles traveled so far
    int m_index = 0;
    double m_traveled = 0.0;

    // Queue of remaining trip steps: (nextCampus, milesToNext)
    std::queue<std::pair<QString, double>> m_stepQueue;

    /*
     * Function: buildStepQueue
     * Purpose : Converts the planned route into a queue
     *           of step-by-step trip movements.
     */
    void buildStepQueue();

    /*
     * Function: planNearestNeighbor
     * Purpose : Builds a route using the greedy nearest-neighbor algorithm.
     */
    void planNearestNeighbor();

    /*
     * Function: nearestStepRecursive
     * Purpose : Recursively selects the next closest unvisited campus.
     */
    void nearestStepRecursive(const QString &current, QSet<QString> &visited);

    /*
     * Function: planExactDp
     * Purpose : Computes the most efficient trip route using
     *           dynamic programming and recursion.
     */
    void planExactDp();

    /*
     * Function: tspRecursive
     * Purpose : Recursive helper used by the exact DP route algorithm.
     */
    double tspRecursive(int mask, int last);

    // Data used by the dynamic programming route calculation
    std::vector<QString> m_nodes;
    std::vector<VectorBst> m_adj;
    std::vector<std::vector<double>> m_memo;
    std::vector<std::vector<int>> m_next;
    int m_allMask = 0;

    /*
     * Function: refreshUi
     * Purpose : Updates the UI to reflect the user's current
     *           position and progress in the trip.
     */
    void refreshUi();

    /*
     * Function: populateRouteList
     * Purpose : Displays the calculated route in the route list widget.
     */
    void populateRouteList();
};

#endif // TRIPWINDOW_H
