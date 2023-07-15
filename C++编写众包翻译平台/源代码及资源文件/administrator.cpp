#include "administrator.h"
#include "ui_administrator.h"
#include "admin_view_cer.h"
#include"widget.h"
#include<QTableWidget>
#include<QLineEdit>
#include<QDebug>
#include<QLabel>
#include<QTableWidgetItem>
administrator::administrator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::administrator)
{
    ui->setupUi(this);
    num = 0;
    ui->tableWidget_user_message->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_user_message->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_user_message->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    qDebug()<<Widget::out_pmessages->size();
    for(QVector<message>::iterator i = Widget::out_pmessages->begin() ; i < Widget::out_pmessages->end();i++){
        if(i->get_receiver() == "admin"){
            qDebug()<<1233;
            int col = ui->tableWidget_user_message->rowCount();
            ui->tableWidget_user_message->insertRow(col);
            QTableWidgetItem* item3 = new QTableWidgetItem(i->get_sender());
            QTableWidgetItem* item0 = new QTableWidgetItem(i->get_date());
            QTableWidgetItem* item1 = new QTableWidgetItem(i->get_title());
            QTableWidgetItem* item2 = new QTableWidgetItem(i->get_state());
            ui->tableWidget_user_message->setItem(num,1,item0);
            ui->tableWidget_user_message->setItem(num,2,item1);
            ui->tableWidget_user_message->setItem(num,3,item2);
            ui->tableWidget_user_message->setItem(num,0,item3);
            num++;
            h_me.push_back(*i);
            qDebug()<<1234;
        }
    }
    QStringList list1;
    for(int i = 1; i <= num; i++){
        list1.push_back(QString::number(i));
    }
    ui->tableWidget_user_message->setVerticalHeaderLabels(list1);
}

administrator::~administrator()
{
    delete ui;
}
//查看详细信息
void administrator::on_pushButton_clicked()
{
    if(ui->tableWidget_user_message->currentItem() == NULL)
    {
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    if(num != 0){
        int num = ui->tableWidget_user_message->currentRow();
        admin_view_cer* dlg = new admin_view_cer(h_me.at(num),this);
        dlg->show();
    }
    else{
        QMessageBox::information(this,"提示","当前没有新消息",QMessageBox::Ok);
    }
}
//修改积分
void administrator::on_pushButton_2_clicked()
{
    if(ui->tableWidget_user_message->currentItem() == NULL)
    {
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    if(num != 0){
        QDialog* recharge = new QDialog(this);
        recharge->setWindowTitle("修改积分");
        QLabel* label = new QLabel(recharge);
        label->setText("请输入您要修改到的积分(-10~100)");
        QLineEdit* linet = new QLineEdit(recharge);
        linet->setPlaceholderText("输入积分");
        linet->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字
        QPushButton* btn_rc = new QPushButton(recharge);
        //设置部件的位置和大小
        recharge->resize(200,150);
        label->resize(150,25);
        linet->resize(70,25);
        label->move(30,30);
        linet->move(60,70);
        btn_rc->resize(50,20);
        btn_rc->move(70,110);
        btn_rc->setText("确定");
        recharge->show();
        connect(btn_rc,&QPushButton::clicked,[=](){
            float money = linet->text().toInt();
            if(money <= -10 ){
                QMessageBox::information(recharge,"提示","输入值不得小于-10",QMessageBox::Ok);
                linet->clear();
            }
            else if(money >100){
                QMessageBox::information(recharge,"提示","输入值不得超过100",QMessageBox::Ok);
                linet->clear();
            }
            else {
                QMessageBox::information(recharge,"提示","修改成功",QMessageBox::Ok);
                int row = ui->tableWidget_user_message ->currentRow();
                if(ui->tableWidget_user_message->item(row, 0) == nullptr){
                    QMessageBox::information(recharge,"提示","未找到对象",QMessageBox::Ok);
                }
                QString sender = ui->tableWidget_user_message->item(row,0)->text();
                Widget::pusers->find(sender)->change_grade(money);
                recharge->close();
            }
        });
    }
    else {
        QMessageBox::information(this,"提示","当前未选中对象",QMessageBox::Ok);
    }
}
