/**
 * @file tripwindow.h
 * @brief Defines the tripWindow class.
 *
 * This window manages the campus trip simulation. It calculates
 * a route, tracks trip progress, handles souvenir purchases,
 * and updates the UI as the user moves from campus to campus.
 */

#ifndef TRIPWINDOW_H
#define TRIPWINDOW_H

#include <QDialog>
#include <QHash>
#include <QSet>
#include <QString>

#include <vector>
#include <queue>
#include <utility>
#include <cstddef>

// Forward declaration for the UI class generated from tripwindow.ui
QT_BEGIN_NAMESPACE
namespace Ui { class tripWindow; }
QT_END_NAMESPACE

// Forward declarations to reduce compile dependencies
class QLineEdit;
class QListWidget;
class QPushButton;
class QTableWidget;

/*
 * Class: tripWindow
 * Purpose: Simulates the campus tour by calculating a route,
 *          showing the current stop, and allowing souvenir purchases.
 *
 * Supports both nearest-neighbor routing and an exact dynamic
 * programming solution for the most efficient trip.
 */
class tripWindow : public QDialog
{
    Q_OBJECT

public:

    /*
     * Function: tripWindow constructor
     * Purpose : Initializes the trip window, loads route data,
     *           and prepares the trip simulation.
     */
    explicit tripWindow(const QString &startCampus,
                        const QStringList &campuses,
                        int maxStops,
                        bool forceExact,
                        QWidget *parent = nullptr);

    /*
     * Function: ~tripWindow
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~tripWindow();

private slots:

    /*
     * Function: onGoNextClicked
     * Purpose : Advances the trip to the next campus in the route.
     */
    void onGoNextClicked();

    /*
     * Function: onBuyClicked
     * Purpose : Processes souvenir purchases for the current campus.
     */
    void onBuyClicked();

private:

    // Pointer to UI elements generated from tripwindow.ui
    Ui::tripWindow *ui;

    // =================================================================
    // Vector-backed Binary Search Tree (BST)
    // =================================================================

private:

    /*
     * Struct: VectorBst
     * Purpose: Stores adjacency data for route planning using
     *          a vector-backed binary search tree.
     */
    struct VectorBst
    {
        /*
         * Struct: Node
         * Purpose: Represents a single BST node containing
         *          a destination index and distance value.
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

        // Queue used for inorder traversal output
        mutable std::vector<std::pair<int, double>> q;
        mutable size_t qHead = 0;

        /*
         * Function: clear
         * Purpose : Resets the BST and clears all stored nodes.
         */
        void clear()
        {
            root = -1;
            nodes.clear();
            q.clear();
            qHead = 0;
        }

        /*
         * Function: insert
         * Purpose : Inserts a key/value pair into the BST.
         */
        void insert(int key, double value)
        {
            root = insertRec(root, key, value);
        }

        /*
         * Function: find
         * Purpose : Searches for a key and returns its value if found.
         */
        bool find(int key, double &out) const
        {
            return findRec(root, key, out);
        }

        /*
         * Function: resetQueueInOrder
         * Purpose : Builds the queue using recursive inorder traversal.
         */
        void resetQueueInOrder() const
        {
            q.clear();
            qHead = 0;
            inorderRec(root);
        }

        /*
         * Function: queueEmpty
         * Purpose : Returns true if the traversal queue is empty.
         */
        bool queueEmpty() const
        {
            return qHead >= q.size();
        }

        /*
         * Function: queuePop
         * Purpose : Removes and returns the next queued BST element.
         */
        std::pair<int, double> queuePop() const
        {
            if (queueEmpty())
                return std::make_pair(-1, 0.0);
            return q[qHead++];
        }

    private:

        /*
         * Function: insertRec
         * Purpose : Recursive helper used to insert a node into the BST.
         */
        int insertRec(int cur, int key, double value)
        {
            if (cur == -1)
            {
                nodes.push_back(Node{key, value, -1, -1});
                return static_cast<int>(nodes.size()) - 1;
            }

            if (key < nodes[static_cast<size_t>(cur)].key)
            {
                const int child = insertRec(nodes[static_cast<size_t>(cur)].left, key, value);
                nodes[static_cast<size_t>(cur)].left = child;
            }
            else if (key > nodes[static_cast<size_t>(cur)].key)
            {
                const int child = insertRec(nodes[static_cast<size_t>(cur)].right, key, value);
                nodes[static_cast<size_t>(cur)].right = child;
            }
            else
            {
                nodes[static_cast<size_t>(cur)].value = value;
            }

            return cur;
        }

        /*
         * Function: findRec
         * Purpose : Recursive helper used to search for a node in the BST.
         */
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

        /*
         * Function: inorderRec
         * Purpose : Recursive inorder traversal used to build the queue.
         */
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

    /*
     * Struct: Souvenir
     * Purpose: Represents a souvenir selected during the trip,
     *          including campus, item name, price, and quantity.
     */
    struct Souvenir
    {
        QString college;
        QString name;
        double price = 0.0;
        int quantity = 1;
    };

    // UI elements added or managed at runtime
    QLineEdit   *m_currentEdit   = nullptr;
    QLineEdit   *m_totalEdit     = nullptr;
    QLineEdit   *m_souvenirCount = nullptr;
    QLineEdit   *m_souvenirCost  = nullptr;
    QLineEdit   *m_nextEdit      = nullptr;
    QLineEdit   *m_nextDistEdit  = nullptr;
    QListWidget *m_routeList     = nullptr;
    QPushButton *m_goNextBtn     = nullptr;
    QPushButton *m_buyBtn        = nullptr;
    QTableWidget *m_souvTable    = nullptr;

    /*
     * Function: buildRuntimeUi
     * Purpose : Creates and configures runtime UI elements
     *           used during the trip simulation.
     */
    void buildRuntimeUi();

    /*
     * Function: setReadOnly
     * Purpose : Configures a QLineEdit so it displays values
     *           without allowing the user to edit them.
     */
    void setReadOnly(QLineEdit *edit);

    // Routing / DB

    /*
     * Function: norm
     * Purpose : Normalizes text such as campus names
     *           by trimming whitespace.
     */
    static QString norm(const QString &s);

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection is open
     *           before performing route or souvenir queries.
     */
    bool ensureDbOpen();

    /*
     * Function: loadAllDistances
     * Purpose : Loads all campus-to-campus distances from
     *           the database into memory.
     */
    void loadAllDistances();

    /*
     * Function: dist
     * Purpose : Returns the stored distance between two campuses.
     */
    double dist(const QString &a, const QString &b) const;

    // Stores the starting campus for the trip
    QString m_start;

    // List of campuses that may be visited
    std::vector<QString> m_candidates;

    // Maximum number of campuses to visit
    int m_maxStops = 1;

    // True if exact dynamic programming route calculation is required
    bool m_forceExact = false;

    // Distance map: from campus -> (to campus -> miles)
    QHash<QString, QHash<QString, double>> m_dist;

    // Planned trip route and the distance for each leg
    std::vector<QString> m_route;
    std::vector<double>  m_legs;

    // Current route index and total miles traveled so far
    int m_index = 0;
    double m_traveled = 0.0;

    // Queue of upcoming trip steps
    std::queue<std::pair<QString, double>> m_stepQueue;

    /*
     * Function: buildStepQueue
     * Purpose : Converts the planned route into a queue
     *           of step-by-step movements.
     */
    void buildStepQueue();

    /*
     * Function: planNearestNeighbor
     * Purpose : Builds a trip route using the nearest-neighbor algorithm.
     */
    void planNearestNeighbor();

    /*
     * Function: nearestStepRecursive
     * Purpose : Recursive helper that selects the next
     *           closest unvisited campus.
     */
    void nearestStepRecursive(const QString &current, QSet<QString> &visited);

    /*
     * Function: planExactDp
     * Purpose : Computes the most efficient trip route using
     *           dynamic programming.
     */
    void planExactDp();

    /*
     * Function: tspRecursive
     * Purpose : Recursive helper used for the exact TSP calculation.
     */
    double tspRecursive(int mask, int last);

    // Data used by the dynamic programming route calculation
    std::vector<QString> m_nodes;
    std::vector<VectorBst> m_adj;
    std::vector<std::vector<double>> m_memo;
    std::vector<std::vector<int>> m_next;
    int m_allMask = 0;

    // Souvenirs

    // Queue storing all souvenirs purchased during the trip
    std::queue<Souvenir> m_purchasedSouvenirs;

    // Running souvenir totals
    int m_totalSouvenirCount = 0;
    double m_totalSouvenirCost = 0.0;

    /*
     * Function: loadSouvenirsForCurrentCollege
     * Purpose : Loads souvenir options for the current campus.
     */
    void loadSouvenirsForCurrentCollege();

    /*
     * Function: updateSouvenirTotals
     * Purpose : Recalculates and updates the souvenir totals shown in the UI.
     */
    void updateSouvenirTotals();

    /*
     * Function: clearSouvenirChecks
     * Purpose : Clears all checked souvenir selections in the table.
     */
    void clearSouvenirChecks();

    // UI refresh

    /*
     * Function: refreshUi
     * Purpose : Updates all trip-related UI fields to reflect
     *           the current trip state.
     */
    void refreshUi();

    /*
     * Function: populateRouteList
     * Purpose : Displays the planned route in the route list widget.
     */
    void populateRouteList();
};

#endif // TRIPWINDOW_H
