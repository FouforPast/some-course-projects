#include "dlg_r_alllco.h"
#include "ui_dlg_r_alllco.h"
#include<QDebug>
#include"widget.h"
dlg_r_alllco::dlg_r_alllco(R_assignment* _r,int _role, QWidget *parent,QString _user0) :
    QDialog(parent),r(_r),role(_role),user0(_user0),
    ui(new Ui::dlg_r_alllco)
{
    ui->setupUi(this);
    if(role == 0){
        ui->pushButton_select->hide();
        ui->pushButton_select->setDisabled(true);
        ui->pushButton_endselect->hide();
        ui->pushButton_endselect->setDisabled(true);
        ui->pushButton_set_re->hide();
        ui->pushButton_set_re->setDisabled(true);
        signups = r->m_contractors.split(QRegExp("[#$]"));//负责人的列表
        ui->comboBox_sign->addItem("查看报名者");
        if(signups.join("") != ""){
            for(int i = 0;i<signups.size()-1;i+=2){
                ui->comboBox_sign->addItem(signups[i]);
            }
        }
        QDateTime time = QDateTime::fromString(r->m_c_ddl,"yyyy-MM-dd hh:mm:ss");
        if(time.toTime_t()>QDateTime::currentDateTime().toTime_t() || r->m_snopsis.m_state == 2|| r->m_snopsis.m_state == 3|| r->m_snopsis.m_state == 4)
        {
            ui->pushButton_set->hide();
            ui->pushButton_set->setDisabled(true);
        }
    }
    if(role == 1){
        ui->pushButton_set->hide();
        ui->pushButton_set->setDisabled(true);
        ui->pushButton_set_re->hide();
        ui->pushButton_set_re->setDisabled(true);
        signups = r->m_translatorss.split(QRegExp("[#$]"));//翻译者的列表
        ui->comboBox_sign->addItem("查看报名者");
        if(signups.join("") != ""){
            for(int i = 0;i<signups.size()-1;i+=2){
                ui->comboBox_sign->addItem(signups[i]);
            }
        }
        QDateTime time = QDateTime::fromString(r->m_t_ddl,"yyyy-MM-dd hh:mm:ss");
        if(time.toTime_t()>QDateTime::currentDateTime().toTime_t() || r->m_snopsis.m_state == 3|| r->m_snopsis.m_state == 4)
        {
            ui->pushButton_select->hide();
            ui->pushButton_select->setDisabled(true);
            ui->pushButton_endselect->hide();
            ui->pushButton_endselect->setDisabled(true);
        }

    }
    if(role == 2){
        ui->pushButton_select->hide();
        ui->pushButton_select->setDisabled(true);
        ui->pushButton_endselect->hide();
        ui->pushButton_endselect->setDisabled(true);
        ui->pushButton_set->hide();
        ui->pushButton_set->setDisabled(true);
        ui->pushButton_set_re->hide();
        ui->pushButton_set_re->setDisabled(true);
        ui->comboBox_sign->addItem(user0);
        signups = r->m_translatorss.split(QRegExp("[#$]"));//翻译者的列表
        int index = signups.indexOf(user0);
        QHash<QString,User>::iterator i = Widget::pusers->find(user0);
        if(i == Widget::pusers->end()){
            QMessageBox::information(this,"提示","无法查看",QMessageBox::Ok);
            return;
        }
        ui->textBrowser_001->setText(Widget::pusers->find(user0)->get_certificate());
        ui->textBrowser_002->setText(signups.at(index+1));
    }
    if(role == 3){
        ui->pushButton_set->hide();
        ui->pushButton_set->setDisabled(true);
        ui->pushButton_select->hide();
        ui->pushButton_select->setDisabled(true);
        ui->pushButton_endselect->hide();
        ui->pushButton_endselect->setDisabled(false);
        signups = r->m_reviewers.split(QRegExp("[#$]"));//审核者的列表
        ui->comboBox_sign->addItem("查看报名者");
        if(signups.join("") != ""){
            for(int i = 0;i<signups.size()-1;i+=2){
                ui->comboBox_sign->addItem(signups[i]);
            }
        }
        QDateTime time = QDateTime::fromString(r->m_c_ddl,"yyyy-MM-dd hh:mm:ss");
        if(time.toTime_t()>QDateTime::currentDateTime().toTime_t())
        {
            ui->pushButton_set_re->hide();
            ui->pushButton_set_re->setDisabled(true);
        }
    }
    ui->comboBox_sign->setCurrentIndex(0);
}
dlg_r_alllco::~dlg_r_alllco()
{
    delete ui;
}
//指定负责人
void dlg_r_alllco::on_pushButton_set_clicked()
{
    if(r->m_snopsis.m_state != recruit_c){
        QMessageBox::information(this,"提示","当前不能操作",QMessageBox::Ok);
        return;
    }
    int index = signups.indexOf(ui->comboBox_sign->currentText());
    if(r->m_contractor == "nobody"){
        if(index != -1){
            Widget::r_passignments->find(r->m_snopsis.m_No)->m_contractor = ui->comboBox_sign->currentText();

            //发送消息
            QString text = QString("恭喜您，成功成为了编号为%1的任务的负责人").arg(r->m_snopsis.m_No);
         int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message* m  = new message("_system",ui->comboBox_sign->currentText(),text,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                      num12,1,"成为负责人");
            signups.removeAt(index);//还有一种写法
            index --;
            signups.removeAt(index+1);
            Widget::in_pmessages->push_back(*m);
            for(int i = 0;i <signups.size() - 1;i+=2){
                QString text = QString("很遗憾的通知您，您落选了编号为%1的任务中的负责人，"
                                       "不过您还可以报名该任务的翻译者").arg(r->m_snopsis.m_No);
                int num10 = Widget::numof_message;
                 num11 = Widget::in_pmessages->size();
                 num12= num10 +num11 +1000;
                message* m = new message("_system",signups.at(i),text,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                         num12,1,"落选负责人");
                Widget::in_pmessages->push_back(*m);
            }
            QMessageBox::information(this,"提示","选中负责人成功",QMessageBox::Ok);
        }
        else {
            QMessageBox::information(this,"提示","选中负责人失败，请检查后稍候重试",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this,"提示","您已经选过负责人了",QMessageBox::Ok);
    }
}
//显示详细信息
void dlg_r_alllco::on_comboBox_sign_currentIndexChanged(const QString &arg1)
{
    if(arg1 != "查看报名者"&&arg1 != "暂无人报名"){
        int index = signups.indexOf(arg1);
        ui->textBrowser_002->setText(signups.at(index+1));
        ui->textBrowser_001->setText(Widget::pusers->find(arg1)->get_certificate());
        if(Widget::pusers->find(arg1)==Widget::pusers->end()){
            QMessageBox::information(this,"提示","查看失败",QMessageBox::Ok);
            return;
        }
        ui->pushButton_set->setDisabled(false);
        ui->pushButton_select->setDisabled(false);
        ui->pushButton_set_re->setDisabled(false);
        ui->pushButton_endselect->setDisabled(false);
    }
    else {
        ui->pushButton_set->setDisabled(true);
        ui->pushButton_select->setDisabled(true);
        ui->pushButton_set_re->setDisabled(true);
        ui->pushButton_endselect->setDisabled(true);
    }
}
//选中翻译者
void dlg_r_alllco::on_pushButton_select_clicked()
{
    if(r->m_snopsis.m_state != recruit_t){
        QMessageBox::information(this,"提示","当前不能操作",QMessageBox::Ok);
        return;
    }
    int index = signups.indexOf(ui->comboBox_sign->currentText());
    qDebug()<<signups<<endl;
    qDebug()<<index<<endl;
    if(index != -1){
        Widget::r_passignments->find(r->m_snopsis.m_No)->m_translators += QString(ui->comboBox_sign->currentText() + "$");
        //发送消息
        QString text = QString("恭喜您，成功成为了编号为%1的任务的翻译者").arg(r->m_snopsis.m_No);
      int num10 = Widget::numof_message;
        int num11 = Widget::in_pmessages->size();
        int num12= num10 +num11 +1000;
        message *m = new message("_system",ui->comboBox_sign->currentText(),text,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                num12, 1,"成为翻译者");
        Widget::in_pmessages->push_back(*m);
        delete m;
        signups.removeAt(index);
        index --;
        signups.removeAt(index+1);
        QMessageBox::information(this,"提示","选中翻译者成功",QMessageBox::Ok);
    }
    else {
        QMessageBox::information(this,"提示","选中翻译者失败，请检查后稍候重试",QMessageBox::Ok);
    }
}
//选中翻译者结束
void dlg_r_alllco::on_pushButton_endselect_clicked()
{
    if(r->m_snopsis.m_state != recruit_t){
        QMessageBox::information(this,"提示","当前不能操作",QMessageBox::Ok);
        return;
    }
    if(Widget::r_passignments->find(r->m_snopsis.m_No)->m_snopsis.m_state == recruit_t){
    if(QMessageBox::Yes == QMessageBox::question(this,"对话框","此举会使得您无法再选择翻译者，您确定要结束吗？",
                                                QMessageBox::Yes|QMessageBox::No)){
        for(int i = 0;i <signups.size() - 1;i+=2){
            QString text = QString("很遗憾的通知您，您落选了编号为%1的任务中的翻译者").arg(r->m_snopsis.m_No);
           qDebug()<<signups<<endl;
          int num10 = Widget::numof_message;
           int num11 = Widget::in_pmessages->size();
           int num12= num10 +num11 +1000;
            message *m = new message("_system",signups.at(i),text,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                     num12,1,"落选翻译者");
            Widget::in_pmessages->push_back(*m);
            delete m;
        }
        ui->pushButton_select->setDisabled(true);
        Widget::r_passignments->find(r->m_snopsis.m_No)->m_snopsis.m_state = translating;
        r->m_snopsis.m_state = translating;
    }
    }
    else {
         QMessageBox::information(this,"提示","您已经选过翻译者",QMessageBox::Ok);
    }
}
//指定审核人
void dlg_r_alllco::on_pushButton_set_re_clicked()
{
    if(r->m_snopsis.m_state != recruit_c){
        QMessageBox::information(this,"提示","当前不能操作",QMessageBox::Ok);
        return;
    }
    int index = signups.indexOf(ui->comboBox_sign->currentText());
    if(r->m_contractor == "nobody"){
        if(index != -1){
           Widget::r_passignments->find(r->m_snopsis.m_No)->m_reviewer = QString(ui->comboBox_sign->currentText());
            //发送消息
            QString text = QString("恭喜您，成功成为了编号为%1的任务的审核者").arg(r->m_snopsis.m_No);
            int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message *m = new message("_system",ui->comboBox_sign->currentText(),text,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                     num12,1,"成为翻译者");
            Widget::in_pmessages->push_back(*m);
            Widget::r_passignments->find(r->m_snopsis.m_No)->m_re_salary = r->m_c_salary*4/5;
            delete m;
            signups.removeAt(index);
            index --;
            signups.removeAt(index+1);
            QMessageBox::information(this,"提示","选中审核者成功",QMessageBox::Ok);
            for(int i = 0;i <signups.size() - 1;i+=2){
               int num10 = Widget::numof_message;
                int num11 = Widget::in_pmessages->size();
                int num12= num10 +num11 +1000;
                QString text = QString("很遗憾的通知您，您落选了编号为%1的任务中的审核人").arg(r->m_snopsis.m_No);
                message* m = new message("_system",signups.at(i),text,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                                         num12,1,"落选审核人");
                Widget::in_pmessages->push_back(*m);
            }
        }
        else {
            QMessageBox::information(this,"提示","选中审核者失败，请检查后稍候重试",QMessageBox::Ok);
        }
    }
    else{
        QMessageBox::information(this,"提示","您已经选过审核人了",QMessageBox::Ok);
    }
}
