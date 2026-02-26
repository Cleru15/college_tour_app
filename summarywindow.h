#ifndef SUMMARYWINDOW_H
#define SUMMARYWINDOW_H

#include <QDialog>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class summaryWindow; }
QT_END_NAMESPACE

class QStringListModel;
class QLineEdit;

class summaryWindow : public QDialog
{
    Q_OBJECT

public:
    explicit summaryWindow(const std::vector<QString> &route,
                           const std::vector<double> &legs,
                           double totalMiles,
                           QWidget *parent = nullptr);
    ~summaryWindow();

private:
    Ui::summaryWindow *ui;

    QStringListModel *m_model = nullptr;

    QLineEdit *m_totalMilesEdit = nullptr;
    QLineEdit *m_souvenirCountEdit = nullptr;
    QLineEdit *m_souvenirCostEdit = nullptr;

    void setReadOnly(QLineEdit *edit);
};

#endif // SUMMARYWINDOW_H
