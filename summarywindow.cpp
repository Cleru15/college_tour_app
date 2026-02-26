#include "summarywindow.h"
#include "ui_summarywindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QStringListModel>

summaryWindow::summaryWindow(const std::vector<QString> &route,
                             const std::vector<double> &legs,
                             double totalMiles,
                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::summaryWindow)
{
    ui->setupUi(this);
    setWindowTitle("Trip Summary");

    auto makeEditNextTo = [&](QLabel *lbl) -> QLineEdit* {
        if (!lbl) return nullptr;
        const QRect r = lbl->geometry();
        auto *e = new QLineEdit(this);
        e->setGeometry(r.x() + r.width() + 10, r.y() - 2, 220, r.height() + 6);
        setReadOnly(e);
        return e;
    };

    m_totalMilesEdit     = makeEditNextTo(ui->label);   // Distance Traveled
    m_souvenirCountEdit  = makeEditNextTo(ui->label_2); // Souvenirs Purchased
    m_souvenirCostEdit   = makeEditNextTo(ui->label_3); // Souvenir Costs

    if (m_totalMilesEdit)    m_totalMilesEdit->setText(QString::number(totalMiles, 'f', 1));
    if (m_souvenirCountEdit) m_souvenirCountEdit->setText("0");
    if (m_souvenirCostEdit)  m_souvenirCostEdit->setText("$0.00");

    // Build a readable route breakdown
    QStringList lines;
    if (!route.empty())
        lines << QString("Start: %1").arg(route[0]);

    for (size_t i = 1; i < route.size(); ++i)
    {
        const double d = (i - 1 < legs.size()) ? legs[i - 1] : 0.0;
        lines << QString("%1) %2  (+%3 miles)")
                     .arg(static_cast<int>(i))
                     .arg(route[i])
                     .arg(d, 0, 'f', 1);
    }

    m_model = new QStringListModel(lines, this);
    ui->listView->setModel(m_model);
}

summaryWindow::~summaryWindow()
{
    delete ui;
}

void summaryWindow::setReadOnly(QLineEdit *edit)
{
    if (!edit) return;
    edit->setReadOnly(true);
    edit->setFocusPolicy(Qt::NoFocus);
}
