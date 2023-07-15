#include "realease_task.h"
#include "ui_realease_task.h"
#include<QMessageBox>
#include"widget.h"
#include"assignment.h"
realease_task::realease_task(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::realease_task)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_2->setDateTime(QDateTime::currentDateTime());
    ui->lineEdit_salary->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字
    ui->lineEdit_salary_2->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
}

realease_task::~realease_task()
{
    delete ui;
}
void realease_task::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()
            ||ui->plainTextEdit->toPlainText().isEmpty()||ui->lineEdit_salary->text().toInt()==0||
            ui->lineEdit_salary->text().isEmpty()||ui->lineEdit_salary_2->text().isEmpty()
            ||ui->plainTextEdit_orin->toPlainText().isEmpty()){
        QMessageBox::information(this,"提示","输入信息不得为空",QMessageBox::Ok);
    }
    else if(ui->dateTimeEdit->dateTime().toTime_t()< QDateTime::currentDateTime().toTime_t()||
            ui->dateTimeEdit->dateTime().toTime_t() > ui->dateTimeEdit_2->dateTime().toTime_t()){
        QMessageBox::information(this,"提示","输入的时间不合法",QMessageBox::Ok);
    }
    else if (ui->lineEdit_salary_2->text().toInt() * 2 > ui->lineEdit_salary->text().toInt()) {
       //负责人的薪酬不得超过总薪酬的一半
        QMessageBox::information(this,"提示","负责人的薪酬不得超过总薪酬的一半",QMessageBox::Ok);
    }
    else if(Widget::pusers->find(Widget::key)->get_balance() < ui->lineEdit_salary->text().toInt()){
        QMessageBox::information(this,"提示","余额不足",QMessageBox::Ok);
    }
    else{
        QString str1 = ui->lineEdit->text();
        QString str2 = ui->lineEdit_2->text();
        QString str3 = ui->lineEdit_3->text();
        QString str4 = ui->plainTextEdit->toPlainText();
        QString str8 = ui->plainTextEdit_orin->toPlainText();
        QString str5 = ui->plainTextEdit->toPlainText();
        int salary = ui->lineEdit_salary->text().toInt();
        QString str6 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString str7 = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString str9 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");
        int salary_c = ui->lineEdit_salary_2->text().toInt();
        int No = Widget::r_passignments->size()+1000;
        Widget::pusers->find(Widget::key)->change_money(-salary);
        R_assignment r_as(str7,str9,str1,Widget::key,str4,str2,str3,recruit_c,No,salary,"",str8,"nobody",salary_c,"",str6,"nobody","","");
        Widget::r_passignments->insert(No,r_as);
        QMessageBox::information(this,"提示","您已经成功发布",QMessageBox::Ok);
        this->close();
    }
}
