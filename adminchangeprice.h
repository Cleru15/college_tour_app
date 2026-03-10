/**
 * @file adminchangeprice.h
 * @brief Defines the adminChangePrice class.
 *
 * This window allows administrators to modify the price
 * of existing souvenir items for a selected campus.
 */

#ifndef ADMINCHANGEPRICE_H
#define ADMINCHANGEPRICE_H

#include <QWidget>
#include <QString>

// Forward declaration for the UI class generated from adminchangeprice.ui
namespace Ui {
class adminChangePrice;
}

/*
 * Class: adminChangePrice
 * Purpose: Provides an interface for administrators to change
 *          the price of souvenirs associated with a specific campus.
 *
 * Inherits from QWidget so it can function as a standalone
 * modification window within the admin tools.
 */
class adminChangePrice : public QWidget
{
    Q_OBJECT

public:

    /*
     * Function: adminChangePrice constructor
     * Purpose : Initializes the window and stores the campus
     *           whose souvenir prices will be modified.
     */
    explicit adminChangePrice(const QString &campus, QWidget *parent = nullptr);

    /*
     * Function: ~adminChangePrice
     * Purpose : Cleans up UI resources when the window closes.
     */
    ~adminChangePrice();

private slots:

    /*
     * Function: on_changePrConfirm_clicked
     * Purpose : Updates the selected souvenir price in the database.
     */
    void on_changePrConfirm_clicked();

    /*
     * Function: on_changePrBack_clicked
     * Purpose : Returns the user to the previous admin window
     *           without making any changes.
     */
    void on_changePrBack_clicked();

private:

    // Pointer to the UI elements generated from adminchangeprice.ui
    Ui::adminChangePrice *ui;

    // Stores the campus whose souvenirs are being modified
    QString m_campus;

    /*
     * Function: ensureDbOpen
     * Purpose : Ensures the SQLite database connection is open
     *           before performing any database operations.
     */
    bool ensureDbOpen();

    /*
     * Function: loadSouvenirsForCampus
     * Purpose : Loads the list of souvenirs for the selected campus
     *           so their prices can be viewed and modified.
     */
    void loadSouvenirsForCampus();
};

#endif // ADMINCHANGEPRICE_H
