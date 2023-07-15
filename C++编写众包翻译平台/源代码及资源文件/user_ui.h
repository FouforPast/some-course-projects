#pragma once
//#ifndef USER_UI_H
//#define USER_UI_H
#include <QMainWindow>
#include"dlg_user_task_ui.h"
namespace Ui {
class user_ui;
}

class user_ui : public QMainWindow
{
    Q_OBJECT

public:
    explicit user_ui(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent* ev);
    dlg_user_task_ui* task_ui;//详细信息的页面
    ~user_ui();
signals:

private slots:
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();
//    void save();

//    void on_pushButton_5_clicked();

    void on_pushButton_c_clicked();

    void on_pushButton_r_clicked();

    void on_pushButton_t_clicked();

    void on_pushButton_t_detail_clicked();

    void on_pushButton_r_detail_clicked();

    void on_pushButton_c_detail_clicked();

    void on_pushButton_recharge_clicked();

    void on_pushButton_changepsd_clicked();

    void on_pushButton_changecer_clicked();

    void on_pushButton_saveexit_clicked();

    void on_pushButton_re_clicked();

    void on_btn_re_detail_clicked();

private:
    Ui::user_ui *ui;

};

//#endif // USER_UI_H
