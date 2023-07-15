#ifndef DLG_R_ALLLCO_H
#define DLG_R_ALLLCO_H
#include"assignment.h"
#include"message.h"
#include <QDialog>

namespace Ui {
class dlg_r_alllco;
}

class dlg_r_alllco : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_r_alllco(R_assignment* r,int role,QWidget *parent = nullptr,QString user0="nobody");
    QStringList signups;
    ~dlg_r_alllco();

private slots:
    void on_pushButton_set_clicked();

    void on_comboBox_sign_currentIndexChanged(const QString &arg1);

    void on_pushButton_select_clicked();

    void on_pushButton_endselect_clicked();

    void on_pushButton_set_re_clicked();

private:
    Ui::dlg_r_alllco *ui;
    int role;
    R_assignment* r;
    QString user0;
};

#endif // DLG_R_ALLLCO_H
