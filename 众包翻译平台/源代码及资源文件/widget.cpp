#include "widget.h"
#include "administrator.h"
#include "ui_widget.h"
#include<QHash>
#include<QtGui>
#include<QPainter>
#include<QPixmap>
#include<QCloseEvent>
#include<QDebug>
#include"dlg_register.h"
#include"user_ui.h"
#include"message.h"
#include"message_ui.h"
#include"dlg_system_message.h"

QHash<QString,User>* Widget::pusers = new QHash<QString,User>;
QHash<int,R_assignment>* Widget::r_passignments = new QHash<int,R_assignment>;
QString Widget::key = "0";
QVector<Validation>* Widget::pvalidation = new QVector<Validation>;
QVector<message>* Widget::out_pmessages = new QVector<message>;
QVector<message>* Widget::in_pmessages = new QVector<message>;
//QVector<QString>* Widget::pcontacts = new QVector<QString>;
bool Widget::no_login_flag = true;
bool Widget::exit_flag = false;
int Widget::numof_message = 0;
bool Widget::close_flag2 = false;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //设置图标
    ui->setupUi(this);
    Widget::key = "nobody";
    QImage _image;
    _image.load(":/image/backg.jpg");
    setAutoFillBackground(true);
    QPalette pal(palette());
    pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                                                        Qt::SmoothTransformation)));
    setPalette(pal);
    is_A = 0;
    _database = Database();
    //非管理员
    ui->pushButton_admin->setDisabled(true);
    ui->pushButton_admin->hide();
    ui->label->hide();
    //打开数据库
    QSqlQuery query(Database::s_database);
    if(Database::s_database.open()){
        qDebug()<<"成功打开数据库"<<endl;
        _database.openDataBase(*Widget::pusers);
    }
    else {
        qDebug()<<"数据库打开失败"<<query.lastError();
        exit(1);
    }
    //登录界面配置
    w_login = new dlgLogin(&is_A, this);
    w_login->setObjectName("login");
    w_login->resize(480,270);
    w_login->setStyleSheet("dlgLogin#login{border-image:url(:/image/login.jpg)}");
    connect(w_login,&dlgLogin::_close,[=](){
        close();
        qDebug()<<"接收到关闭信号并且关闭"<<endl;
    });
    w_login->exec();

    if(!Widget::no_login_flag){
//    _database.openDataBase(*Widget::pcontacts,Widget::key);
    _database.openDataBase(*Widget::out_pmessages);
    _database.openDataBase(*Widget::pvalidation);
    _database.openDataBase(*Widget::r_passignments);
    Database here;
    numof_message =  here.num_of_messages();
    }
    if(is_A)
    {
        ui->pushButton_admin->setDisabled(false);
        ui->pushButton_admin->show();
        ui->label->show();
    }
}
Widget::~Widget()
{
    delete ui;
    delete r_passignments;
    delete pvalidation;
    delete out_pmessages;
    delete in_pmessages;
    delete pusers;
}
void Widget::closeEvent(QCloseEvent *ev){
    if(Widget::close_flag2){
        qDebug()<<"开始导入数据"<<endl;
        if(!Widget::no_login_flag){
//            _database.ingressDataBase(*Widget::pcontacts,Widget::key);
            _database.ingressDataBase(*Widget::pvalidation);
            _database.ingressDataBase(*Widget::in_pmessages);
            _database.ingressDataBase(*Widget::r_passignments);
        }
        _database.ingressDataBase(*Widget::pusers);
        Database::s_database.close();  //数据库连接关闭
        qDebug()<<"数据库连接关闭"<<endl;
        ev->accept();
        Widget::exit_flag=true;
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this,"提示","确认要退出吗",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
        //以下开始将数据导入到数据库
        qDebug()<<"开始导入数据"<<endl;
        if(!Widget::no_login_flag){
//            _database.ingressDataBase(*Widget::pcontacts,Widget::key);
            _database.ingressDataBase(*Widget::pvalidation);
            _database.ingressDataBase(*Widget::in_pmessages);
            _database.ingressDataBase(*Widget::r_passignments);
        }
        _database.ingressDataBase(*Widget::pusers);
        Database::s_database.close();  //数据库连接关闭
        qDebug()<<"数据库连接关闭"<<endl;
        ev->accept();
        Widget::exit_flag=true;
    }
    else if(Widget::key == "nobody"){
        w_login = new dlgLogin(&is_A, this);
        w_login->setObjectName("login");
        w_login->resize(480,270);
        w_login->setStyleSheet("dlgLogin#login{border-image:url(:/image/login.jpg)}");
        w_login->exec();
    }
    else {
        ev->ignore();
    }

}
//进入任务界面
void Widget::on_btn_assignment_clicked()
{
    w_assignment_ui = new assignment_ui(this);

    w_assignment_ui->show();
}
//进入聊天界面
void Widget::on_btn_communication_clicked()
{
    w_message_ui = new message_ui(this);
    w_message_ui->setObjectName("dlg");
    w_message_ui->resize(480,300);
    w_message_ui->setStyleSheet("message_ui#dlg{border-image:url(:/image/backg.jpg)}");
    w_message_ui->show();
}
//进入个人主页
void Widget::on_btn_mine_clicked()
{
    w_user_ui = new user_ui(this);
    w_user_ui->setObjectName("dlg");
    w_user_ui->resize(560,350);
    w_user_ui->setStyleSheet("user_ui#dlg{border-image:url(:/image/backg.jpg)}");
    w_user_ui->show();
}
//系统消息
void Widget::on_pushButton_message_system_clicked()
{
    dlg_system_message* dlg = new dlg_system_message(this);
//    dlg->setObjectName("dlg");
//    dlg->resize(400,250);
//    dlg->setStyleSheet("dlg_system_message#dlg{border-image:url(:/image/backg.jpg)}");
    dlg->show();
}
//进入管理员界面
void Widget::on_pushButton_admin_clicked()
{
    administrator* dlg_admin = new administrator(this);
//    dlg_admin->setObjectName("dlg");
//    dlg_admin->resize(400,250);
//    dlg_admin->setStyleSheet("administrator#dlg{border-image:url(:/image/backg.jpg)}");
    dlg_admin->show();
}
