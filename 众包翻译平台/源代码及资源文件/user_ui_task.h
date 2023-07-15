#ifndef USER_UI_TASK_H
#define USER_UI_TASK_H
#include <QStackedWidget>
class R_assignment;
class T_assignment;
namespace Ui {
class user_ui_task;
}

class user_ui_task : public QStackedWidget
{
    Q_OBJECT

public:
    QHash<int,R_assignment>::iterator i;
    T_assignment* j;
    QStringList  liststr;
    explicit user_ui_task(int role,int z_No,QWidget *parent = nullptr);
    ~user_ui_task();

private slots:

    void on_pushButton_c_submit_clicked();

    void on_pushButton_t_tr_clicked();

    void on_btn_look_my_translation_clicked();


    void on_pushButton_view_r_clicked();

    void on_pushButton_derive_r_clicked();

    void on_pushButton_modify_r_clicked();

    void on_pushButton_sure_r_clicked();

    void on_pushButton_c_view_clicked();

    void on_pushButton_c_derive_clicked();

    void on_pushButton_view_signup_r_clicked();

    void on_pushButton_view_o_t_clicked();

    void on_pushButton_t_submit_clicked();

    void on_pushButton_view_singnup_c_clicked();

    void on_pushButton_c_allco_clicked();

    void on_btn_delay_r_clicked();

    void on_btn_delay_c_clicked();

    void on_btn_look_my_translation_2_clicked();

    void on_pushButton_view_o_t_2_clicked();

    void on_btn_re_exit_clicked();

    void on_pushButton_clicked();

    void on_pushButton_view_signu_r_re_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::user_ui_task *ui;
    int role;
    int z_No;
};

#endif // USER_UI_TASK_H
