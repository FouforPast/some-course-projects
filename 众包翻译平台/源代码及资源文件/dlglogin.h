#ifndef DLGLOGIN_H
#define DLGLOGIN_H
#include<map>
#include<QMessageBox>
#include<QString>
#include<utility>
#include <QDialog>
#include"dlg_register.h"
#include"user.h"

class Widget;
namespace Ui {
class dlgLogin;
}

class dlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit dlgLogin(int* is_ ,QWidget *parent = nullptr);
    ~dlgLogin();
    bool close_flag;
    void closeEvent(QCloseEvent* ev);
    static bool close_flag3;
    dlg_register* dlg_register1 =new dlg_register;

signals:
    void send_login();
    void _close();
private slots:

    void on_btn_register_clicked();

    void on_btn_OK_clicked();

    void on_btn_login_a_clicked();

private:
    Ui::dlgLogin *ui;
    int* is_;
};

#endif // DLGLOGIN_H
