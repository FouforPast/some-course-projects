#include "dlg_update.h"
#include "ui_dlg_update.h"
#include<QMessageBox>
#include<widget.h>
dlg_update::dlg_update(R_assignment* _r, QWidget *parent) :
    QDialog(parent),r(_r),
    ui(new Ui::dlg_update)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字
    ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字

}

dlg_update::~dlg_update()
{
    delete ui;
}

void dlg_update::on_pushButton_clicked()
{
    if(ui->textEdit->toPlainText().isEmpty()||ui->spinBox->value()==0)
    {
        QMessageBox::information(this,"提示","输入不得为空",QMessageBox::Ok);
    }
    else if (ui->dateTimeEdit->dateTime().toTime_t() < QDateTime::currentDateTime().toTime_t()||
             ui->dateTimeEdit->dateTime().toTime_t() >
             QDateTime::fromString(r->m_snopsis.m_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t()) {
        QMessageBox::information(this,"提示","输入的时间不合法",QMessageBox::Ok);
    }
    else {
        QString str = ui->textEdit->toPlainText();
        int min = ui->lineEdit->text().toInt();
        int max = ui->lineEdit_2->text().toInt();
        if(min < max || ((min + max) /2) * ui->spinBox->value() + r->m_c_salary +
                (r->m_reviewer == "nobody"? 0: (r->m_c_salary *4/5)) > r->m_snopsis.m_salary){
            QMessageBox::information(this,"提示","输入的金额不合适",QMessageBox::Ok);
            return;
        }
        Widget::r_passignments->find(r->m_snopsis.m_No)->m_t_ddl = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
        Widget::r_passignments->find(r->m_snopsis.m_No)->m_t_salary_max = max;
        Widget::r_passignments->find(r->m_snopsis.m_No)->m_t_salary_min = min;
        Widget::r_passignments->find(r->m_snopsis.m_No)->m_info = str;
        Widget::r_passignments->find(r->m_snopsis.m_No)->m_snopsis.m_state = recruit_t;
        this->close();
    }
}
