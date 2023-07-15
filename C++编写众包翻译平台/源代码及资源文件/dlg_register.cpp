#include<map>
#include<QMessageBox>
#include<QString>
#include<utility>
#include<QSpinBox>
#include "dlg_register.h"
#include "ui_dlg_register.h"
#include"user.h"
#include"widget.h"
#include<QDebug>
#include<QPainter>
#include<QPixmap>
#include<regex>
#include<string>
#include<QButtonGroup>
using namespace std;
dlg_register::dlg_register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_register)
{
    ui->setupUi(this);
    ui->label_welcome->setAlignment(Qt::AlignCenter);
    ui->linet_phoneNumber->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字
    //设置最小化窗口
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    //设置焦点事件
    ui->linet_username->setFocus();
}

dlg_register::~dlg_register()
{
    delete ui;
}
//点击了确定按钮
void dlg_register::on_btn_confirm_clicked()
{
    QString username=ui->linet_username->text();
    QString psd_begin=ui->linet_psd_begin->text();
    QString psd_sure=ui->linet_psd_sure->text();
    QString phone=ui->linet_phoneNumber->text();
    QString sex;
    QButtonGroup *g_sex=new QButtonGroup(this);
    g_sex->addButton(ui->radioButton,0);
    g_sex->addButton(ui->radioButton_2,1);
    int sel=g_sex->checkedId();
    switch(sel){
    case 0:
        sex="male";
        break;
    case 1:
        sex="female";
        break;
    default:
        QMessageBox::information(this,"提示","请选择性别",QMessageBox::Ok);
        break;
    }
    int age = ui->lineEdit->text().toInt();
    if(!username.isEmpty()&&!psd_begin.isEmpty()&&!psd_sure.isEmpty()&&!phone.isEmpty()&&(age!=0)&&!sex.isEmpty()){//检查输入项是否为空
        if(phone.length() == 11&& 5 <= psd_begin.length() && psd_begin.length() <= 15){
            if(psd_sure==psd_begin){//检查前后两次输入的密码是否相等
                if(Widget::pusers->find(phone)==Widget::pusers->end()){//检查手机号是否已经被注册过
                    QMessageBox::information(this,"提示","您已经注册成功",QMessageBox::Ok);
                    User user0(username,psd_begin,phone,age,sex,0,"none",0,0,"","");
                    Database here;
                    Widget::pusers->insert(phone,user0);
                    here.ingressDataBase(*Widget::pusers);
                    Widget::key=phone;
                    Widget::no_login_flag = false;
                    this->close();
                }

                else {
                    QMessageBox::information(this,"提示","您的手机号码已经被注册过",QMessageBox::Ok);
                }
            }
            else {
                QMessageBox::information(this,"提示","前后输入的密码不一致",QMessageBox::Ok);
            }
        }
        else if(phone.length() != 11){
            QMessageBox::information(this,"提示","您输入的手机号码不符合规范",QMessageBox::Ok);
        }
        else {
            QMessageBox::information(this,"提示","输入的密码应该为5到15位",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this,"提示","输入不得为空",QMessageBox::Ok);
    }
}
//重写退出事件
void dlg_register::closeEvent(QCloseEvent* ev){
    if(QMessageBox::No == QMessageBox::question(this,"提示","确认要退出吗",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
        ev->ignore();
        return;
        }
    dlgLogin::close_flag3 = true;
    if(Widget::no_login_flag){
        emit exit_flag_r();
    }
    else {
        emit register_success();
    }
    ev->accept();
}
//重写绘图事件
void dlg_register:: paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/login.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}
