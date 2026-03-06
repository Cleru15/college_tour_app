/**
 * @file summarywindow.h
 * @brief Defines the summaryWindow class.
 *
 * Displays a summary of the completed campus trip including
 * the route taken, total miles traveled, and souvenir totals.
 */

// summarywindow.h defines the summaryWindow dialog class.
// This dialog displays a final summary of the campus trip,
// including the route taken, total distance traveled,
// and souvenir purchase totals.

#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include <QDialog>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class summaryWindow; }
QT_END_NAMESPACE

// Forward declarations for Qt classes used as pointers
class QStringListModel;
class QLineEdit;

/*
 * Class: summaryWindow
 * Purpose: Displays a summary of the completed campus tour.
 *          It shows the route breakdown, total miles traveled,
 *          and souvenir purchase information.
 */
class summaryWindow : public QDialog
{
    Q_OBJECT

public:

    /*
     * Constructor
     * Purpose: Builds the summary window and displays the
     *          trip route, leg distances, and total miles traveled.
     */
    explicit summaryWindow(const std::vector<QString> &route,
                           const std::vector<double> &legs,
                           double totalMiles,
                           QWidget *parent = nullptr);

    /*
     * Destructor
     * Purpose: Cleans up the UI resources when the summary window closes.
     */
    ~summaryWindow();

private:

    // Pointer to the UI generated from summarywindow.ui
    Ui::summaryWindow *ui;

    // Model used to display the route breakdown in the list view
    QStringListModel *m_model = nullptr;

    // Displays the total distance traveled during the trip
    QLineEdit *m_totalMilesEdit = nullptr;

    // Displays the number of souvenirs purchased
    QLineEdit *m_souvenirCountEdit = nullptr;

    // Displays the total cost of souvenirs purchased
    QLineEdit *m_souvenirCostEdit = nullptr;

    /*
     * Function: setReadOnly
     * Purpose : Configures a QLineEdit so the user can view
     *           its value without being able to edit it.
     */
    void setReadOnly(QLineEdit *edit);
};

#endif // SUMMARYWINDOW_H
