#ifndef MODINFO_H
#define MODINFO_H

#include <QWidget>
#include <QString>

namespace Ui {
class modinfo;
}

class modinfo : public QWidget
{
    Q_OBJECT

public:
    explicit modinfo(QWidget *parent = nullptr);
    ~modinfo();

private slots:
    void on_buttonSubmit_clicked();
    void on_buttonCancel_clicked();
    void on_buttonConfirm_clicked();
    void on_buttonBack_clicked();

private:
    Ui::modinfo *ui;

    QString m_selectedStadium;
    QString m_selectedField;

    bool ensureDbOpen();
    void loadStadiumDropdown();
    void resetToDefaultState();
    void showEditControls(bool show);

    bool isIntegerField(const QString &field) const;
    QString databaseColumnForField(const QString &field) const;
    QString formattedValueForField(const QString &field,
                                   const QString &rawInput,
                                   bool &ok,
                                   QString &errorMessage) const;

    bool updateRegularField(const QString &columnName,
                            const QString &value);

    bool updateStadiumName(const QString &oldStadiumName,
                           const QString &newStadiumName);
};

#endif // MODINFO_H
