#include "user_ui_task.h"
#include "ui_user_ui_task.h"
#include"dlg_allco_c.h"
#include"widget.h"
#include"dlg_delay_ddl.h"
#include"dlg_update.h"
#include"dlg_c_view.h"
#include<QDialog>
#include<QPushButton>
#include<QDebug>
#include<QTextEdit>
#include<QLabel>
#include<dlg_r_alllco.h>
#include<QMessageBox>
#include<QLineEdit>
user_ui_task::user_ui_task(int _role,int _z_No,QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::user_ui_task)
{
    ui->setupUi(this);
    role = _role;
    z_No = _z_No;
    i = Widget::r_passignments->find(z_No);//该大任务的迭代器
    Database here;
    j = new T_assignment;
    here.openDataBase(*j,Widget::key,i->m_snopsis.m_No);//翻译任务
    if(j == NULL)
    {
        QMessageBox::information(this,"提示","打开失败",QMessageBox::Ok);
        return;
    }
    liststr=i->m_translators.split("$");//存储翻译者
    switch (role){
    case 0:
        ui->textBrowser_subtitle_1->setText(i->m_snopsis.m_sub_title);
        ui->textBrowser_tran->setText(liststr.join("\n"));
        break;
    case 1:
        ui->textBrowser_subtitle_2->setText(i->m_snopsis.m_sub_title);
        ui->textBrowser_tran_c->setText(liststr.join("\n"));
        break;
    case 2:

        ui->textBrowser_subtitle_3->setText(j->m_info);
        break;
    case 3:
        ui->textBrowser_subtitle_4->setText(i->m_snopsis.m_sub_title);
        break;
    }
}

user_ui_task::~user_ui_task()
{
    delete ui;
}


//负责人提交译文按钮
void user_ui_task::on_pushButton_c_submit_clicked()
{
    if(i->m_snopsis.m_state != translating){
        QMessageBox::information(this,"提示","当前状态不能查看",QMessageBox::Ok);
        return;
    }
    if(i->m_translators == ""){
        QMessageBox::information(this,"提示","当前没有翻译者",QMessageBox::Ok);
        return;
    }
    dlg_c_view* dlg = new dlg_c_view(1,&(i.value()),2,this);
    dlg->show();
}

//发布者查看译文
void user_ui_task::on_pushButton_view_r_clicked()
{
    QDateTime time1 = QDateTime::fromString(i->m_snopsis.m_ddl,"yyyy-MM-dd hh:mm:ss");
    if(time1.toTime_t() > QDateTime::currentDateTime().toTime_t())
    {
        QMessageBox::information(this,"提示","该任务尚未完成，请耐心等待",QMessageBox::Ok);
    }
    else {
        QDialog* dlg = new QDialog(this);
        dlg->resize(500,300);
        QTextBrowser* textb = new QTextBrowser(dlg);
        textb->setText(i->get_translation());
        textb->move(5,5);
        textb->resize(490,250);
        QPushButton* btn = new QPushButton(dlg);
        btn->setText("评价");
        btn->move(225,265);
        btn->resize(50,20);
        dlg->show();
    }
}
//发布者获取原文
void user_ui_task::on_pushButton_derive_r_clicked()
{
    QDialog* dlg = new QDialog(this);
    dlg->resize(500,300);
    QTextBrowser* textb = new QTextBrowser(dlg);
    textb->setText(i->get_oringin());
    textb->move(5,5);
    textb->resize(490,290);
    dlg->show();
}
//发布者修改原文
void user_ui_task::on_pushButton_modify_r_clicked()
{
    if(i->m_snopsis.m_state == completed){
        QMessageBox::information(this,"提示","当前状态不能查看",QMessageBox::Ok);
        return;
    }
    QDialog* dlg= new QDialog(this);
    dlg->resize(300,500);
    QTextEdit* texte = new QTextEdit(dlg);
    QLabel* label = new QLabel (this);
    label->setText("请直接在原文中修改，并在备注框中表明您修改的部分");
    label->resize(100,20);
    label->move(5,5);
    texte->move(5,30);
    texte->setText(i->get_oringin());
    QTextEdit* texte2 = new QTextEdit(dlg);
    texte2->setPlaceholderText("请于此处填写备注");
    texte->resize(290,215);
    texte2->resize(290,215);
    texte2->move(5,250);
    QPushButton* btn = new QPushButton(dlg);
    btn->setText("确认修改");
    btn->resize(50,20);
    btn->move(125,470);
    connect(btn,&QPushButton::clicked,dlg,[=](){
        if(QMessageBox::Yes == QMessageBox::question(dlg,"确认对话框","您确定要修改吗",
                                                     QMessageBox::Yes|QMessageBox::No)){
            i->m_oringin = texte->toPlainText();
            int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message e("_system",i->m_contractor,texte2->toPlainText(),
                      QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"原文修改");
            Widget::in_pmessages->push_back(e);
            if(i->m_reviewer!="nobody"){
                int num10 = Widget::numof_message;
                num11 = Widget::in_pmessages->size();
                num12= num10 +num11 +1000;
                message e1("_system",i->m_reviewer,texte2->toPlainText(),
                           QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"原文修改");
                Widget::in_pmessages->push_back(e1);
            }
            for(int k = 0;k < liststr.size() - 1; k++){
                int num10 = Widget::numof_message;
                num11 = Widget::in_pmessages->size();
                num12= num10 +num11 +1000;
                message* e2 = new message("_system",i->m_contractor,texte2->toPlainText(),
                                          QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"原文修改");
                Widget::in_pmessages->push_back(*e2);
                delete e2;
            }
            dlg->close();
        }
    });
    dlg->show();
}

//发布者查看负责人报名者
void user_ui_task::on_pushButton_view_signup_r_clicked()
{
    R_assignment* r = new R_assignment(i.value());
    dlg_r_alllco* dlg = new dlg_r_alllco(r,0,this);
    dlg->show();
}
//发布者确认完成
void user_ui_task::on_pushButton_sure_r_clicked()
{
    if(i->m_snopsis.m_state != translating){
        QMessageBox::information(this,"提示","当前状态不是正在翻译的状态",QMessageBox::Ok);
        return;
    }
    QDialog* dlg= new QDialog(this);
    dlg->resize(300,500);
    QTextEdit* texte = new QTextEdit(dlg);
    QLabel* label = new QLabel (dlg);
    label->setText("任务完成，赶紧来评价一下吧");
    label->resize(40,20);
    label->move(5,5);
    texte->move(5,30);

    QTextEdit* texte2 = new QTextEdit(dlg);
    texte->setPlaceholderText("对负责人有什么想说的");
    if(i->m_reviewer!="nobody"){
        texte2->setPlaceholderText("对审核人有什么想说的");
        texte->resize(290,215);
        texte2->resize(290,215);
        texte2->move(5,250);
    }
    else {
        texte->resize(290,440);
        delete texte2;
    }
    QPushButton* btn = new QPushButton(dlg);
    btn->setText("确认发布");
    btn->resize(50,20);
    btn->move(125,470);
    connect(btn,&QPushButton::clicked,dlg,[=](){
        //执行转账操作
        Widget::pusers->find(Widget::key)->change_money(-(i->m_snopsis.m_salary));
        Widget::r_passignments->find(i->m_snopsis.m_No)->m_c_evaluation = texte->toPlainText();
        QString* t = new QString(QString("恭喜您，您负责的编号为%1的任务已经确认完成").arg(i->m_snopsis.m_No));
        int num10 = Widget::numof_message;
        int num11 = Widget::in_pmessages->size();
        int num12= num10 +num11 +1000;
        message *e = new message("_system",i->m_contractor,*t,
                                 QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"任务完成");
        Widget::pusers->find(i->m_contractor)->change_money(i->m_c_salary);
        qDebug()<<Widget::in_pmessages->size();
        Widget::in_pmessages->push_back(*e);
        qDebug()<<Widget::in_pmessages->size();
        qDebug()<<i->m_reviewer;
        if(i->m_reviewer != "nobody"){
            Widget::pusers->find(i->m_reviewer)->change_money(i->m_re_salary);
            QString* t = new QString(QString("恭喜您，您审核的编号为%1的任务已经确认完成").arg(i->m_snopsis.m_No));
            int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message *e = new message("_system",i->m_reviewer,*t,
                                     QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"#",num12,1,"任务完成");

            Widget::r_passignments->find(i->m_snopsis.m_No)->m_re_evaluation = texte2->toPlainText();

            Widget::in_pmessages->push_back(*e);
        }
        delete t;
        delete e;
        for(int k = 0;k < liststr.size() - 1; k++){
            int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            T_assignment t;
            Database _here;
            _here.openDataBase(t,liststr[k],i->m_snopsis.m_No);
            Widget::pusers->find(liststr[k])->change_money(t.m_t_salary);
            QString* text1 = new QString(QString("恭喜您，您翻译的主任务编号为%1的任务已经确认完成").arg(i->m_snopsis.m_No));
            message *e = new message("_system",liststr.at(k),*text1,
                                     QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+ "$",num12,1,"任务完成");
            Widget::in_pmessages->push_back(*e);
            delete text1;
            delete e;
        }
        //任务状态发生变化
        i->m_snopsis.m_state = completed;
        QMessageBox::information(this,"提示","发布完成",QMessageBox::Ok);
        dlg->close();
    });
    dlg->show();
}
//负责人查看、评价译文
void user_ui_task::on_pushButton_c_view_clicked()
{
    if(i->m_snopsis.m_state != translating){
        QMessageBox::information(this,"提示","当前状态不能查看",QMessageBox::Ok);
        return;
    }
    if(i->m_translators == ""){
        QMessageBox::information(this,"提示","当前没有翻译者",QMessageBox::Ok);
        return;
    }
    dlg_c_view* dlg = new dlg_c_view(0,&(i.value()),2,this);
    dlg->show();
}
//负责人查看报名者
void user_ui_task::on_pushButton_view_singnup_c_clicked()
{
    if(i->m_snopsis.m_state != recruit_t){
        QMessageBox::information(this,"提示","当前不能查看",QMessageBox::Ok);
        return;
    }
    R_assignment* r = new R_assignment(i.value());
    dlg_r_alllco* dlg = new dlg_r_alllco(r,1,this);
    dlg->show();
}
//负责人分配任务
void user_ui_task::on_pushButton_c_allco_clicked()
{
    if(i->m_snopsis.m_state != translating){
        QMessageBox::information(this,"提示","当前状态不能操作",QMessageBox::Ok);
        return;
    }
    if(QDateTime::fromString(i->m_t_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t() > QDateTime::currentDateTime().toTime_t()){
        QMessageBox::information(this,"提示","还没有到报名截止日期，请耐心等待",QMessageBox::Ok);
        return;
    }
    R_assignment* r = new R_assignment(i.value());
    dlg_allco_c* dlg = new dlg_allco_c(r,this);
    dlg->show();
}
//负责人获取原文
void user_ui_task::on_pushButton_c_derive_clicked()
{
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->resize(500,350);
    textb->resize(490,340);
    textb->move(5,5);
    textb->setText(i->m_oringin);
    dlg->show();
}
//翻译者查看原文
void user_ui_task::on_pushButton_view_o_t_clicked()
{
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->resize(500,350);
    textb->resize(490,340);
    textb->move(5,5);
    textb->setText(j->m_oringin);
    dlg->show();
}
//翻译者继续翻译
void user_ui_task::on_pushButton_t_tr_clicked()
{
    if(i->m_snopsis.m_state != translating){
        QMessageBox::information(this,"提示","当前状态不能查看",QMessageBox::Ok);
        return;
    }
    QDialog* dlg = new QDialog(this);
    QLabel* lbl = new QLabel(dlg);
    QPushButton* btn = new QPushButton(dlg);
    QTextEdit* texte = new QTextEdit(dlg);
    QLabel* lbl2 = new QLabel(dlg);
    QTextBrowser* textb = new QTextBrowser(dlg);

    dlg->setWindowTitle("翻译");
    lbl->setText("原文：");
    lbl2->setText("请输入译文：");
    btn->setText("保存");
    dlg->resize(508,330);
    lbl->resize(160,16);
    lbl2->resize(160,16);
    lbl->move(6,6);
    lbl2->move(256,6);
    textb->resize(246,275);
    texte->resize(246,275);
    textb->move(6,19);
    texte->move(256,19);
    btn->resize(50,20);
    btn->move(440,300);
    qDebug()<<j->get_translation()<<endl;
    //输入原本的译文
    textb->setText(j->get_oringin());
    texte->setText(j->get_translation());
    dlg->show();
    connect(btn,&QPushButton::clicked,this,[=](){
        j->change_translation(texte->toPlainText());
        qDebug()<<j->get_translation();
        Database here;
        if(!here.ingressDataBase(*j)){
            QMessageBox::information(this,"提示","保存失败",QMessageBox::Ok);
            return;
        }
        QMessageBox::information(this,"提示","您的译文已经保存",QMessageBox::Ok);
    });
}
//翻译者查看译文
void user_ui_task::on_btn_look_my_translation_clicked()
{
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->setWindowTitle("查看译文");
    dlg->resize(500,300);
    textb->resize(490,290);
    textb->move(5,5);
    textb->setText(j->get_translation());
    dlg->show();
}
//翻译者提交译文
void user_ui_task::on_pushButton_t_submit_clicked()
{
    if(i->m_snopsis.m_state != translating){
        QMessageBox::information(this,"提示","当前状态不能查看",QMessageBox::Ok);
        return;
    }
    if(QMessageBox::Yes == QMessageBox::question(this,"警告","您确定要提交译文吗",QMessageBox::Yes|QMessageBox::No)){
        int num10 = Widget::numof_message;
        int num11 = Widget::in_pmessages->size();
        int num12= num10 +num11 +1000;
        QString str = QString("您负责的主任务编号为%1的翻译者%2已经完成子任务并提交")
                .arg(i->m_snopsis.m_No).arg(j->m_translator);
        message* me = new message("_system",i->m_contractor,str,QDateTime::currentDateTime()
                                  .toString("yyyy-MM-dd hh:mm:ss"),num12,1,"子任务完成");
        Widget::in_pmessages->push_back(*me);
        delete me;
    }
}

//发布者推迟ddl
void user_ui_task::on_btn_delay_r_clicked()
{
    if(i->m_snopsis.m_state == completed){
        QMessageBox::information(this,"提示","当前状态不能操作",QMessageBox::Ok);
        return;
    }
    dlg_delay_ddl* dlg = new dlg_delay_ddl(&(i.value()),1,this);
    dlg->show();
}
//负责人推迟ddl
void user_ui_task::on_btn_delay_c_clicked()
{
    if(i->m_snopsis.m_state == completed){
        QMessageBox::information(this,"提示","当前状态不能操作",QMessageBox::Ok);
        return;
    }
    dlg_delay_ddl* dlg = new dlg_delay_ddl(&(i.value()),2,this);
    dlg->show();
}
//审核人查看、审核译文
void user_ui_task::on_btn_look_my_translation_2_clicked()
{
    if(i->m_snopsis.m_state != translating){
        QMessageBox::information(this,"提示","当前状态不能操作",QMessageBox::Ok);
        return;
    }
    dlg_c_view* dlg = new dlg_c_view(0,&(i.value()),4,this);
    dlg->show();
}
//审核人查看原文
void user_ui_task::on_pushButton_view_o_t_2_clicked()
{
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->resize(500,350);
    textb->resize(490,340);
    textb->move(5,5);
    textb->setText(i->m_oringin);
    dlg->show();
}
//审核人查看评价
void user_ui_task::on_btn_re_exit_clicked()
{
    if(i->m_snopsis.m_state != completed){
        QMessageBox::information(this,"提示","本任务尚未完成",QMessageBox::Ok);
        return;
    }
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->setWindowTitle("对话框");
    dlg->resize(500,350);
    textb->resize(490,340);
    textb->setText(i->m_re_evaluation);
    textb->move(5,5);
    dlg->show();
}
//翻译者查看fuzeren评价
void user_ui_task::on_pushButton_clicked()
{
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->setWindowTitle("查看评价");
    dlg->resize(500,300);
    textb->resize(490,290);
    textb->move(5,5);
    textb->setText(j->m_c_evaluation);
    dlg->show();
}
//查看审核人报名
void user_ui_task::on_pushButton_view_signu_r_re_clicked()
{
    R_assignment* r = new R_assignment(i.value());
    dlg_r_alllco* dlg = new dlg_r_alllco(r,3,this);
    dlg->show();
}
//发布者删除任务
void user_ui_task::on_pushButton_del_clicked()
{
    if(i->m_snopsis.m_state == 3||i->m_snopsis.m_state == 4){
        QMessageBox::question(this,"对话框","当前任务正在进行中，不能取消",QMessageBox::Ok);
        return;
    }
    else{
        if(QMessageBox::Yes == QMessageBox::question(this,"警告","您确定要删除任务吗",QMessageBox::Yes|QMessageBox::No))
        {
            Widget::pusers->find(Widget::key)->change_money(i->m_snopsis.m_salary);
            Widget::r_passignments->erase(i);
        }
    }
}
//负责人更新发布任务
void user_ui_task::on_pushButton_update_clicked()
{
    if(i->m_snopsis.m_state == recruit_t){
        QMessageBox::information(this,"提示","您已经更新过任务",QMessageBox::Ok);
        return;
    }
    dlg_update* dlg = new dlg_update(&(i.value()),this);
    dlg->show();
}
//翻译者查看审核人评价
void user_ui_task::on_pushButton_2_clicked()
{
    if(i->m_reviewer == "nobody"){
        QMessageBox::information(this,"提示","本任务没有审核人",QMessageBox::Ok);
        return;
    }
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->setWindowTitle("查看评价");
    dlg->resize(500,300);
    textb->resize(490,290);
    textb->move(5,5);
    textb->setText(j->m_r_evaluation);
    dlg->show();
}
//负责人查看发布者评价
void user_ui_task::on_pushButton_3_clicked()
{
    if(i->m_snopsis.m_state != 4){
        QMessageBox::information(this,"提示","本任务尚未完成",QMessageBox::Ok);
        return;
    }
    QDialog* dlg = new QDialog(this);
    QTextBrowser* textb = new QTextBrowser(dlg);
    dlg->setWindowTitle("查看评价");
    dlg->resize(500,300);
    textb->resize(490,290);
    textb->move(5,5);
    textb->setText(i->m_c_evaluation);
    dlg->show();
}
