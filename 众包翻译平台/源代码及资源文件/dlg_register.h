#ifndef DLG_REGISTER_H
#define DLG_REGISTER_H
#include<map>
#include<QString>
#include<utility>
#include <QDialog>
#include"user.h"


namespace Ui {
class dlg_register;
}

class dlg_register : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_register(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    ~dlg_register();
signals:
    void register_success();
    void exit_flag_r();
private slots:
    void on_btn_confirm_clicked();
    void closeEvent(QCloseEvent* ev);
private:
    Ui::dlg_register *ui;

};

#endif // DLG_REGISTER_H
