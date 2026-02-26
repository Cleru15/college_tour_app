#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class NewWindow; }
QT_END_NAMESPACE

class NewWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructs the souvenirs dialog for a specific college and loads its items.
    explicit NewWindow(const QString &collegeName, QWidget *parent = nullptr);

    // Destroys the dialog and ensures the DB connection is closed/removed.
    ~NewWindow();

private:
    Ui::NewWindow *ui;
    QString m_collegeName;

    QSqlDatabase m_db;
    QString m_connName;

    // Opens a unique SQLite connection for this dialog instance.
    bool openDb();

    // Closes the dialog’s DB connection and removes the named connection from Qt.
    void closeDb();

    // Queries and displays souvenirs (item, price) for the selected college.
    void loadSouvenirs();
};

#endif // NEWWINDOW_H
