#include "dlglogin.h"
#include "ui_dlglogin.h"
#include"dlg_register.h"
#include"widget.h"
#include<QIcon>
#include<QMessageBox>
#include<QDebug>

bool dlgLogin::close_flag3 = false;
dlgLogin::dlgLogin(int* is,QWidget *parent) :
    QDialog(parent),is_(is),
    ui(new Ui::dlgLogin)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");//设置登录对话框标题
    //设置最小化窗口
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    //设置焦点事件
    ui->linet_account->setFocus();
    //设置close_flag的属性
    close_flag = true;
    connect(dlg_register1,&dlg_register::exit_flag_r,[=](){
        close_flag = true;
        this->close();
    });
    connect(dlg_register1,&dlg_register::register_success,[=](){
        close_flag=false;
        this->close();
    });

}

dlgLogin::~dlgLogin()
{
    delete ui;
}
//点击了注册按钮的情况
void dlgLogin::on_btn_register_clicked()
{
    close_flag=false;
    this->hide();//将close改为hide
    dlg_register1->exec();
}
//用户点击了确定键
void dlgLogin::on_btn_OK_clicked()
{
    QString account=ui->linet_account->text();
    QString password=ui->linet_password->text();
    if(account.isEmpty()&&password.isEmpty()){//输入为空的情况
        QMessageBox* msgb = new QMessageBox;
        msgb->setFixedSize(sizeHint().height(),sizeHint().width());
        msgb->setText("输入不得为空");
        msgb->setWindowTitle("提示");
        msgb->setStandardButtons(QMessageBox::Ok);
        msgb->button(QMessageBox::Ok)->setText("确定");
        msgb->exec();
        return;
    }
    QHash<QString,User>::iterator i = Widget::pusers->find(account);
    if(i!=Widget::pusers->end()){//手机号在数据库中存在的情况
        if(password==i->get_password()){//输入的密码和数据库中的密码匹配的情况
            Widget::key=account;//将账户手机号赋给key
            Widget::no_login_flag = false;
            close_flag = false;
            this->close();
        }
        else {//密码不匹配
            QMessageBox::information(this,"提示","您输入的手机号码或密码不正确",QMessageBox::Ok);
        }
    }
    else{//用户尚未注册的情况
        if(QMessageBox::Ok == QMessageBox::question(this,"尚未注册",QString("%1尚未完成注册，是否现在立即去注册")
                                                    .arg(account),QMessageBox::Ok|QMessageBox::No)){
            close_flag = false;
            this->hide();
            dlg_register1->exec();
        }
        else {//点击了取消
            ui->linet_account->clear();
            ui->linet_password->clear();
        }
    }
}
void dlgLogin::closeEvent(QCloseEvent *ev){
    qDebug()<<"登录界面即将关闭"<<endl;
    if(close_flag){
        if(close_flag3){
            Widget::close_flag2 = true;
            emit _close();
            ev->ignore();
            this->hide();
            return;
        }
        if(QMessageBox::No == QMessageBox::question(this,"提示","确认要退出吗",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
            ev->ignore();
            return;
        }

        Widget::close_flag2 = true;
        emit _close();
    }
    ev->ignore();
    this->hide();
}
//管理员界面
void dlgLogin::on_btn_login_a_clicked()
{
    QDialog * dlg = new QDialog(this);
    dlg->setObjectName("login");
    dlg->setStyleSheet("Dialog#dlg{border-image:url(:/image/login.jpg)}");
    QLabel* lbl = new QLabel(dlg);
    QPushButton* btn = new QPushButton(dlg);
    QLineEdit* linee = new QLineEdit(dlg);
    dlg->resize(200,112);
    lbl->resize(54,20);
    lbl->setText("请输入密钥");
    lbl->move(6,5);
    btn->resize(50,20);
    btn->setText("确定");
    btn->move(90,80);
    linee->resize(160,22);
    linee->move(20,34);
    linee->setEchoMode(QLineEdit::Password);
    dlg->show();
    connect(btn,&QPushButton::clicked,dlg,[=](){
        QString str = linee->text();
        if("miyao" == str){
            QString account=ui->linet_account->text();
            QString password=ui->linet_password->text();
            if(account.isEmpty()&&password.isEmpty()){//输入为空的情况
                QMessageBox* msgb = new QMessageBox;
                msgb->setFixedSize(sizeHint().height(),sizeHint().width());
                msgb->setText("输入不得为空");
                msgb->setWindowTitle("提示");
                msgb->setStandardButtons(QMessageBox::Ok);
                msgb->button(QMessageBox::Ok)->setText("确定");
                msgb->exec();
                return;
            }
            QHash<QString,User>::iterator i = Widget::pusers->find(account);
            if(i == Widget::pusers->end()){
                QMessageBox::information(this,"提示","您尚未注册",QMessageBox::Ok);
                return;
            }
            *is_ = 1;
            if(password==i->get_password()){//输入的密码和数据库中的密码匹配的情况
                Widget::key=account;//将账户手机号赋给key
                Widget::no_login_flag = false;
                close_flag = false;
                this->close();
                dlg->close();
            }
        }
        else {
            QMessageBox::information(this,"提示","输入的密钥不正确",QMessageBox::Ok);
            linee->clear();
        }
    });
}
