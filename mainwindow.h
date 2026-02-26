#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QComboBox;
class QTableView;
class QSqlQueryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Connected in mainwindow.ui (explicit connections section)
    void previewSouvenirButtonClick();
    void cancelButtonClick();

    // Auto-connected by Qt (connectSlotsByName) from object names in mainwindow.ui
    void on_submitButtonSouvenirPreview_2_clicked(); // Show distances
    void on_submitButtonSouvenirPreview_3_clicked(); // Basic Trip
    void on_submitButtonSouvenirPreview_4_clicked(); // Custom Trip

private:
    Ui::MainWindow *ui;

    // DB helpers
    bool ensureDbOpen();

    // Distances UI/model
    void setupDistanceUi();
    void populateStartingCampusCombo();
    void loadDistancesForCampus(const QString &fromCampus);

private:
    QComboBox *m_fromCampusCombo = nullptr;   // points at ui->dropdownSouvenirPreview_2
    QTableView *m_distanceTable = nullptr;    // created at runtime (replaces listView)
    QSqlQueryModel *m_distanceModel = nullptr;
};

#endif // MAINWINDOW_H
