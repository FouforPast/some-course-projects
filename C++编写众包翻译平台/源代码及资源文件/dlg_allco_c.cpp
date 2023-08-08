#include "dlg_allco_c.h"
#include "ui_dlg_allco_c.h"
#include"dlg_r_alllco.h"
#include<QMessageBox>
#include<QDebug>
#include"widget.h"
#include"database.h"
#include"assignment.h"
int dlg_allco_c::num = 0;

dlg_allco_c::dlg_allco_c(R_assignment* _r,QWidget *parent) :
    QDialog(parent),r(_r),
    ui(new Ui::dlg_allco_c)
{
    ui->setupUi(this);
    ui->comboBox_select->addItem("查看翻译者");
    ui->comboBox_select->setCurrentIndex(0);
    QStringList tran = r->m_translators.split("$");
    ui->lineEdit_sa->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    ui->textBrowser_orin->setText(r->m_oringin);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    int num = 0;
    for(int i = 0;i <tran.size() - 1;i++){
        ui->comboBox_select->addItem(tran.at(i));
        num++;
    }
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
}
dlg_allco_c::~dlg_allco_c()
{
    delete ui;
}
//分配任务
void dlg_allco_c::on_pushButton_allco_clicked()
{
    if(ui->comboBox_select->currentText()=="查看翻译者"){
        QMessageBox::information(this,"提示","请先选中一个译者",QMessageBox::Ok);
        return;
    }
    QDateTime time = ui->dateTimeEdit->dateTime();
    if(time.toTime_t()<QDateTime::currentDateTime().toTime_t()){
        QMessageBox::information(this,"提示","截止日期不能早于当前时间",QMessageBox::Ok);
    }
    else if (ui->lineEdit_sa->text().toInt() > r->m_t_salary_max
             ||ui->lineEdit_sa ->text().toInt() < r->m_t_salary_min) {
        QMessageBox::information(this,"提示","输入的金额应该在最小金额和最大金额之间",QMessageBox::Ok);
    }
    else {
        T_assignment ass(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),time.toString("yyyy-MM-dd hh:mm:ss"),
                         ui->lineEdit_settitle->text(),r->m_snopsis.m_releaser,ui->plainTextEdit_tips->toPlainText(),
                         r->m_snopsis.m_o_language,r->m_snopsis.m_o_language,3,r->m_snopsis.m_No,r->m_snopsis.m_salary,"",
                         ui->textEdit_allco_orin->toPlainText(),ui->lineEdit_sa->text().toInt(),ui->comboBox_select->currentText(),"","","",
                         static_cast<int>(QDateTime::currentDateTime().toTime_t()) + num);
        num ++;
        r->m_t_ddl = time.toString("yyyy-MM-dd hh:mm:ss");
        Database here;
        if (!here.ingressDataBase(ass)){//插入失败
            QMessageBox::information(this,"提示","分配任务失败",QMessageBox::Ok);
        }
        else{
          int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message m("_system",ui->comboBox_select->currentText(),QString("您主任务编号为%1的负责人%2给您发放了任务")
                      .arg(r->m_snopsis.m_No).arg(Widget::key),QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"任务提醒");
            Widget::in_pmessages->push_back(m);
            int index = ui->comboBox_select->currentIndex();
            ui->comboBox_select->removeItem(index);
            QMessageBox::information(this,"提示","分配成功",QMessageBox::Ok);
        }
    }
}
//负责人查看详细信息
//void dlg_allco_c::on_pushButton_detail_clicked()
//{
//    if(ui->comboBox_select->currentText() == "查看翻译者" ||
//            ui->comboBox_select->currentText() == ""){
//        QMessageBox::information(this,"提示","无法查看",QMessageBox::Ok);
//        return;
//    }
//    dlg_r_alllco* dlg = new dlg_r_alllco(r,2,this,ui->comboBox_select->currentText());
//    dlg->show();
//}
