#include "dlg_delay_ddl.h"
#include "ui_dlg_delay_ddl.h"
#include"assignment.h"
#include<widget.h>
dlg_delay_ddl::dlg_delay_ddl(R_assignment* r_,int role_,QWidget *parent) :
    QDialog(parent),r(r_),role(role_),
    ui(new Ui::dlg_delay_ddl)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    g_ = new QButtonGroup(this);
    g_->addButton(ui->radioButton,0);
    g_->addButton(ui->radioButton_2,1);
    g_->addButton(ui->radioButton_3,2);

}

dlg_delay_ddl::~dlg_delay_ddl()
{
    delete ui;
}
//修改日期
void dlg_delay_ddl::on_pushButton_clicked()
{
    sel=g_->checkedId();
    switch (sel) {
    case 0://修改负责人报名ddl
        if(role == 1){
            if(r->m_snopsis.m_state != recruit_c){
                QMessageBox::information(this,"提示","期限已过，不能更改",QMessageBox::Ok);
            }
            else {
                if(ui->dateTimeEdit->dateTime().toTime_t() >
                        QDateTime::fromString(r->m_snopsis.m_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t()){
                    QMessageBox::information(this,"提示","修改日期不能比最终期限迟",QMessageBox::Ok);
                }
                else if(ui->dateTimeEdit->dateTime().toTime_t() < QDateTime::currentDateTime().toTime_t()){
                    QMessageBox::information(this,"提示","修改日期不能早于当前时间",QMessageBox::Ok);
                }
                else{
                    Widget::r_passignments->find(r->m_snopsis.m_No)->m_t_ddl = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
                    QMessageBox::information(this,"提示","修改成功",QMessageBox::Ok);
                    this->close();
                }
            }
        }
        else{
            QMessageBox::information(this,"提示","不能更改",QMessageBox::Ok);
        }
        break;
    case 1:
        if(role == 2){
            if(r->m_snopsis.m_state != recruit_t){
                QMessageBox::information(this,"提示","期限已过，不能更改",QMessageBox::Ok);
            }
            else {
                if(ui->dateTimeEdit->dateTime().toTime_t() >
                        QDateTime::fromString(r->m_snopsis.m_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t()){
                    QMessageBox::information(this,"提示","修改日期不能比最终期限迟",QMessageBox::Ok);
                }
                else if(ui->dateTimeEdit->dateTime().toTime_t() < QDateTime::currentDateTime().toTime_t()){
                    QMessageBox::information(this,"提示","修改日期不能早于当前时间",QMessageBox::Ok);
                }
                else{
                    Widget::r_passignments->find(r->m_snopsis.m_No)->m_c_ddl = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
                    QMessageBox::information(this,"提示","修改成功",QMessageBox::Ok);
                    this->close();
                }
            }
        }
        else {
            QMessageBox::information(this,"提示","不能更改",QMessageBox::Ok);
        }
        break;
    case 2:
        if(role == 1 ){
            if(r->m_snopsis.m_state == completed){
                QMessageBox::information(this,"提示","期限已过，不能更改",QMessageBox::Ok);
            }
            else {

                if(ui->dateTimeEdit->dateTime().toTime_t() <
                        QDateTime::fromString(r->m_snopsis.m_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t()){
                    QMessageBox::information(this,"提示","修改的日期不能比原有日期提前",QMessageBox::Ok);
                }
                else if(ui->dateTimeEdit->dateTime().toTime_t() < QDateTime::currentDateTime().toTime_t()){
                    QMessageBox::information(this,"提示","修改日期不能早于当前时间",QMessageBox::Ok);
                }
                else{
                   Widget::r_passignments->find(r->m_snopsis.m_No)->m_snopsis.m_ddl = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
                    QMessageBox::information(this,"提示","修改成功",QMessageBox::Ok);
                    this->close();
                }
            }
        }
        else {
            QMessageBox::information(this,"提示","不能更改",QMessageBox::Ok);
        }
        break;
    }
}
