#ifndef MODSTADIUMS_H
#define MODSTADIUMS_H

#include <QDialog>

namespace Ui {
class ModStadiums;
}

class ModStadiums : public QDialog
{
    Q_OBJECT

public:
    explicit ModStadiums(QWidget *parent = nullptr);
    ~ModStadiums();

private slots:
    void on_buttonBack_clicked();
    void on_buttonModify_clicked();
    void on_buttonReset_clicked();
    void on_buttonModInfo_clicked();

private:
    Ui::ModStadiums *ui;

    bool ensureDbOpen();
    void ensureStadiumAccessTable();
    void syncStadiumsIntoAccessTable();
    void loadStadiumChecklist();
};

#endif // MODSTADIUMS_H
