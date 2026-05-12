<<<<<<< HEAD
=======
/**
 * @file summarywindow.h
 * @brief Defines the summaryWindow class.
 *
 * This window displays the final results of a completed campus tour.
 * It shows the route taken, total miles traveled, and a summary of
 * souvenirs purchased during the trip.
 */

>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include <QDialog>
#include <QString>
#include <vector>

<<<<<<< HEAD
=======
// Forward declaration for the UI class generated from summarywindow.ui
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
QT_BEGIN_NAMESPACE
namespace Ui { class summaryWindow; }
QT_END_NAMESPACE

<<<<<<< HEAD
class QStringListModel;
class QLineEdit;

=======
// Forward declarations to reduce compile dependencies
class QStringListModel;
class QLineEdit;

/*
 * Class: summaryWindow
 * Purpose: Displays a summary of the completed campus tour.
 *
 * The window lists the campuses visited, the distances between
 * each stop, and the total distance traveled. It also summarizes
 * the number and cost of souvenirs purchased during the trip.
 */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
class summaryWindow : public QDialog
{
    Q_OBJECT

public:
<<<<<<< HEAD
    struct PurchasedSouvenir
    {
        QString stadium;
=======

    /*
     * Struct: PurchasedSouvenir
     * Purpose: Represents a souvenir purchased during the trip.
     *          Stores the campus where it was purchased, the
     *          souvenir name, and its price.
     */
    struct PurchasedSouvenir
    {
        QString college;
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
        QString name;
        double price = 0.0;
    };

<<<<<<< HEAD
=======
    /*
     * Function: summaryWindow constructor
     * Purpose : Initializes the summary window and displays
     *           the trip route, distances, and souvenir summary.
     */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    explicit summaryWindow(const std::vector<QString> &route,
                           const std::vector<double> &legs,
                           double totalMiles,
                           const std::vector<PurchasedSouvenir> &purchases,
                           QWidget *parent = nullptr);
<<<<<<< HEAD
    ~summaryWindow();

private slots:
    void on_backButtonSum_clicked();

private:
    Ui::summaryWindow *ui = nullptr;

    QStringListModel *m_model = nullptr;

    QLineEdit *m_totalMilesEdit = nullptr;
    QLineEdit *m_souvenirCountEdit = nullptr;
    QLineEdit *m_souvenirCostEdit = nullptr;

=======

    /*
     * Function: ~summaryWindow
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~summaryWindow();

private slots:

    /*
     * Function: on_backButtonSum_clicked
     * Purpose : Closes the summary window and returns the user
     *           to the previous screen.
     */
    void on_backButtonSum_clicked();

private:

    // Pointer to UI elements generated from summarywindow.ui
    Ui::summaryWindow *ui = nullptr;

    // Model used to display the route list in the UI
    QStringListModel *m_model = nullptr;

    // Displays the total miles traveled during the trip
    QLineEdit *m_totalMilesEdit = nullptr;

    // Displays the total number of souvenirs purchased
    QLineEdit *m_souvenirCountEdit = nullptr;

    // Displays the total cost of souvenirs purchased
    QLineEdit *m_souvenirCostEdit = nullptr;

    /*
     * Function: setReadOnly
     * Purpose : Configures a QLineEdit to display values
     *           without allowing the user to edit them.
     */
>>>>>>> f8aaa0a89393c209bbee31fe8a23ac118f91f9de
    void setReadOnly(QLineEdit *edit);
};

#endif // SUMMARYWINDOW_H
