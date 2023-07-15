#include "dlg_user_task_ui.h"
#include "ui_dlg_user_task_ui.h"
#include<QMessageBox>
dlg_user_task_ui::dlg_user_task_ui(int _role,int _z_No,QWidget *parent) :
    QDialog(parent),role(_role),z_No(_z_No),
    ui(new Ui::dlg_user_task_ui)
{
    ui->setupUi(this);
    dlg = new user_ui_task(role,z_No,this);
    if(dlg == NULL)
    {
        QMessageBox::information(this,"提示","查看失败",QMessageBox::Ok);
        return;
    }
    dlg->setCurrentIndex(role);
}

dlg_user_task_ui::~dlg_user_task_ui()
{
    delete ui;
}
