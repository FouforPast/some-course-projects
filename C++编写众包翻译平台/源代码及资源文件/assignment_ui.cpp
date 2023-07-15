#include "assignment_ui.h"
#include "ui_assignment_ui.h"
#include<assignment.h>
#include"widget.h"
#include<QTextEdit>
#include<QLabel>
#include<QTextBrowser>
#include<algorithm>
#include<QDebug>
assignment_ui::assignment_ui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::assignment_ui)
{
    ui->setupUi(this);
    QStringList list;
    ui->tableWidget_0->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->tableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_0->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
    ui->tableWidget_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableWidget_0->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableWidget_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableWidget_0->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->tableWidget_1->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->tableWidget_0->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget_1->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget_3->horizontalHeader()->setStretchLastSection(true);
//    ui->tableWidget_0->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int rows[4] = {0,0,0,0};
    for (QHash<int,R_assignment>::iterator i =Widget::r_passignments->begin();
         i != Widget::r_passignments->end();i++) {
        if(i->m_snopsis.m_state == 1){
            int row_c = ui->tableWidget_0->rowCount();
            ui->tableWidget_0->insertRow(row_c);
            list<<QString::number(i->m_snopsis.m_No)<<i->m_snopsis.m_title<<
                  i->m_snopsis.m_releaser<< i->m_snopsis.m_o_language
               <<i->m_snopsis.m_language<< QString::number(int(i->m_snopsis.m_salary))
              <<i->m_snopsis.m_date_release<<i->m_snopsis.m_ddl;
            for(int j = 0;j<list.size();j++){
                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_0->setItem(rows[0],j,item1);
            }
            rows[0]++;
            list.clear();
        }
        else if(i->m_snopsis.m_state == 2){
            int row_c = ui->tableWidget_1->rowCount();
            ui->tableWidget_1->insertRow(row_c);
            list<< QString::number(i->m_snopsis.m_No)<<i->m_snopsis.m_title<<i->m_snopsis.m_releaser<<i->m_contractor
                << i->m_snopsis.m_o_language<<i->m_snopsis.m_language
                << QString::number(i->m_t_salary_min)<<QString::number(i->m_t_salary_max)
                <<i->m_snopsis.m_date_release<<i->m_t_ddl<<i->m_snopsis.m_ddl;
            for(int j = 0;j<list.size();j++){
                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_1->setItem(rows[1],j,item1);
            }
            rows[1]++;
            list.clear();
        }
        else if(i->m_snopsis.m_state == 3){
            int row_c = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->insertRow(row_c);
            list<< QString::number(i->m_snopsis.m_No)<<i->m_snopsis.m_title<<i->m_snopsis.m_releaser
                << i->m_snopsis.m_o_language<<i->m_snopsis.m_language
                <<  QString::number(int(i->m_snopsis.m_salary))<<i->m_contractor
                 <<i->m_snopsis.m_date_release<<i->m_snopsis.m_ddl;
            for(int j = 0;j<list.size();j++){
                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_2->setItem(rows[2],j,item1);
            }
            rows[2]++;
            list.clear();
        }
        else{
            int row_c = ui->tableWidget_3->rowCount();
            ui->tableWidget_3->insertRow(row_c);
            list<< QString::number(i->m_snopsis.m_No)<<i->m_snopsis.m_title<<i->m_snopsis.m_releaser
                << i->m_snopsis.m_o_language<<i->m_snopsis.m_language<< i->m_contractor
                <<i->m_translators<<i->m_snopsis.m_date_release<<i->m_snopsis.m_ddl;
            for(int j = 0;j<list.size();j++){
                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_3->setItem(rows[3],j,item1);
            }
            rows[3]++;
            list.clear();
        }
    }



    //设置竖直方向的表头
    QStringList list2[4];
    for(int i = 0;i<4;i++){
        for(int j =0;j<rows[i];j++){
            list2[i].insert(j,QString::number(j+1));
        }
    }
    ui->tableWidget_0->setVerticalHeaderLabels(list2[0]);
    ui->tableWidget_1->setVerticalHeaderLabels(list2[1]);
    ui->tableWidget_2->setVerticalHeaderLabels(list2[2]);
    ui->tableWidget_3->setVerticalHeaderLabels(list2[3]);
}
assignment_ui::~assignment_ui()
{
    delete ui;
}
//招募译者
void assignment_ui::on_btn_001_clicked()
{
    ui->stacked_info->setCurrentIndex(1);
}

//招募负责人
void assignment_ui::on_btn_002_clicked()
{
    ui->stacked_info->setCurrentIndex(0);
}
//翻译中
void assignment_ui::on_bt_003_clicked()
{
    ui->stacked_info->setCurrentIndex(2);
}
//已完成
void assignment_ui::on_btn_004_clicked()
{
    ui->stacked_info->setCurrentIndex(3);
}
//报名负责人
void assignment_ui::on_btn_s_contractor_clicked()
{
    if(ui->tableWidget_0->currentItem() == NULL)
    {
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    //设置积分达到20分才能担当负责人
    if(Widget::pusers->find(Widget::key)->get_grade() >= 20){
        int current_row = ui->tableWidget_0->currentRow();
        int num = ui->tableWidget_0->item(current_row,0)->text().toInt();
        QHash<int,R_assignment>::iterator i =Widget::r_passignments->find(num);

        QStringList signups = i->m_contractors.split(QRegExp("[#$]"));
        for(int j = 0;j <signups.size();j++){
            if(signups.at(j)==Widget::key){
                QMessageBox::information(this,"提示","您已经成功报过名",QMessageBox::Ok);
                return;
            }
        }
        if(i -> m_snopsis.m_releaser == Widget::key){
            QMessageBox::information(this,"提示","您已经是发布者了",QMessageBox::Ok);
            return;
        }
        if(QDateTime::fromString(i->m_c_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t() >
                QDateTime::currentDateTime().toTime_t()){

            const QString str;
            //填写理由
            QDialog* change_cer = new QDialog(this);
            QTextEdit* text = new QTextEdit(change_cer);
            QLabel* label = new QLabel(change_cer);
            QPushButton* btn = new QPushButton(change_cer);
            change_cer->setWindowTitle("填写报名理由");
            label->setText("请填写您的报名理由，例如您有什么优势(不要填写$ #等字符)");
            change_cer->resize(350,300);
            label->resize(250,20);
            label->move(15,12);
            text->resize(320,220);
            text->move(15,42);
            btn->resize(80,16);
            btn->move(135,272);
            btn->setText("确定");
            connect(btn,&QPushButton::clicked,change_cer,[=](){
                QMessageBox::information(change_cer,"提示","您成功报名",QMessageBox::Ok);
                Widget::r_passignments->find(num)->m_contractors  += QString("%1$%2#").arg(Widget::key).arg(text->toPlainText());
                change_cer->close();
            });
            change_cer->exec();
        }
        else {
            QMessageBox::information(this,"提示","报名期限已过",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this,"提示","您的积分不足20，不能报名负责人",QMessageBox::Ok);
    }
}
//报名翻译者
void assignment_ui::on_btn_s_interpreter_clicked()
{
    if(ui->tableWidget_1->currentItem() == NULL)
    {
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    if(Widget::pusers->find(Widget::key)->get_can_translate()
            || Widget::pusers->find(Widget::key)->get_grade()){
        int current_row = ui->tableWidget_1->currentRow();
        int num = ui->tableWidget_1->item(current_row,0)->text().toInt();
        QHash<int,R_assignment>::iterator i =Widget::r_passignments->find(num);
        QStringList signups = i->m_translatorss.split(QRegExp("[#$]"));
        for(int j = 0;j <signups.size() - 1;j += 2 ){
            if(signups.at(j)==Widget::key){
                QMessageBox::information(this,"提示","您已经成功报过名",QMessageBox::Ok);
                return;
            }
        }
        if(i->m_contractor == Widget::key){
            QMessageBox::information(this,"提示","您已经是负责人了",QMessageBox::Ok);
            return;
        }
        if(i->m_reviewer == Widget::key){
            QMessageBox::information(this,"提示","您已经是审核人了",QMessageBox::Ok);
            return;
        }
        if(QDateTime::fromString(i->m_t_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t() > QDateTime::currentDateTime().toTime_t()){
            QStringList signups = i->m_translatorss.split(QRegExp("[#$]"));
            const QString str;
            //填写理由
            QDialog* change_cer = new QDialog(this);
            QTextEdit* text = new QTextEdit(change_cer);
            QLabel* label = new QLabel(change_cer);
            QPushButton* btn = new QPushButton(change_cer);
            change_cer->setWindowTitle("填写报名理由");
            label->setText("请填写您的报名理由，例如您有什么优势(不要填写$ #等字符)");
            change_cer->resize(350,300);
            label->resize(250,20);
            label->move(15,12);
            text->resize(320,220);
            text->move(15,42);
            btn->resize(80,16);
            btn->move(135,272);
            btn->setText("确定");
            connect(btn,&QPushButton::clicked,change_cer,[=](){
                QMessageBox::information(change_cer,"提示","您成功报名",QMessageBox::Ok);
                i->m_translatorss  += QString("%1$%2#").arg(Widget::key).arg(text->toPlainText());
                change_cer->close();
            });
            change_cer->exec();
        }
        else {
            QMessageBox::information(this,"提示","报名期限已过",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this,"提示","请先加入翻译团队",QMessageBox::Ok);
    }
}
//发布任务
void assignment_ui::on_pushButton_r_clicked()
{
    realease_task* dlg = new realease_task(this);
    dlg->exec();
}
//报名负责人查看简介
void assignment_ui::on_pushButton_detail1_clicked(){
    int current_row = ui->tableWidget_0->currentRow();
    if(ui->tableWidget_0->item(current_row,0) == NULL){
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    int num = ui->tableWidget_0->item(current_row,0)->text().toInt();
    QDialog* dlg = new QDialog(this);
    QTextBrowser* bro = new QTextBrowser(dlg);
    dlg->resize(250,190);
    QHash<int,R_assignment>::iterator i = Widget::r_passignments->find(num);
    if(i == Widget::r_passignments->end()){
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    bro->setText(i->m_snopsis.m_sub_title);
    bro->resize(240,180);
    bro->move(5,5);
    dlg->show();

}
//报名译者查看简介
void assignment_ui::on_pushButton_detail2_clicked()
{
    int current_row = ui->tableWidget_1->currentRow();
    if(ui->tableWidget_1->item(current_row,0) == NULL){
        QMessageBox::information(this,"提示","未找到对象",QMessageBox::Ok);
        return;
    }
    int num = ui->tableWidget_1->item(current_row,0)->text().toInt();
    QDialog* dlg = new QDialog(this);
    QTextBrowser* bro = new QTextBrowser(dlg);
    dlg->resize(250,190);
    QHash<int,R_assignment>::iterator i = Widget::r_passignments->find(num);
    if(i == Widget::r_passignments->end()){
        QMessageBox::information(this,"提示","未找到对象",QMessageBox::Ok);
        return;
    }
    bro->setText(i->m_info);
    bro->resize(bro->sizeHint());
    dlg->show();
}
//加入翻译团队
void assignment_ui::on_btn_joinin_clicked()
{
    if(Widget::pusers->find(Widget::key)->get_can_translate() == 1 ||
            Widget::pusers->find(Widget::key)->get_grade() != 0){
         QMessageBox::information(this,"提示","您已经加入翻译团队了",QMessageBox::Ok);
         return;
    }
    QDialog* dlg =new QDialog(this);
    QTextEdit* textt = new QTextEdit(dlg);
    QPushButton* btn = new QPushButton(dlg);
    btn->resize(50,20);
    btn->setText("确定");
    textt->setPlaceholderText("请在此处填写您要修改的语言资质证明");
    dlg->resize(310,210);
    textt->resize(300,180);
    textt->move(5,5);
    btn->move(130,185);
    connect(btn,&QPushButton::clicked,dlg,[=](){
        if(textt->toPlainText()== ""){
            QMessageBox::information(this,"提示","输入不得为空",QMessageBox::Ok);
        }
        else {
            QString text = textt->toPlainText();
           int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message me(Widget::key,"admin",text,
                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"加入翻译团队");
            QMessageBox::information(this,"提示","发送成功",QMessageBox::Ok);
            Widget::in_pmessages->push_back(me);
            Widget::out_pmessages->push_back(me);
            dlg->close();
        }
    });
    dlg->show();
}
//报名审核人
void assignment_ui::on_btn_sign_up_reviewer_clicked()
{
    if(ui->tableWidget_0->currentItem() == NULL)
    {
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    //设置积分达到30分才能担当审核人
    if(Widget::pusers->find(Widget::key)->get_grade() >= 30){
        //将该负责人的id转入临时数组中
        int current_row = ui->tableWidget_0->currentRow();
        int num = ui->tableWidget_0->item(current_row,0)->text().toInt();
        QHash<int,R_assignment>::iterator i =Widget::r_passignments->find(num);
        QStringList signups = i->m_reviewers.split(QRegExp("[#$]"));
        for(int j = 0;j <signups.size();j++){
            if(signups.at(j)==Widget::key){
                QMessageBox::information(this,"提示","您已经成功报过名",QMessageBox::Ok);
                return;
            }
        }
        if(i -> m_snopsis.m_releaser == Widget::key){
            QMessageBox::information(this,"提示","您已经是发布者了",QMessageBox::Ok);
            return;
        }
        if(QDateTime::fromString(i->m_c_ddl,"yyyy-MM-dd hh:mm:ss").toTime_t() > QDateTime::currentDateTime().toTime_t()){
            const QString str;
            //填写理由
            QDialog* change_cer = new QDialog(this);
            QTextEdit* text = new QTextEdit(change_cer);
            QLabel* label = new QLabel(change_cer);
            QPushButton* btn = new QPushButton(change_cer);
            change_cer->setWindowTitle("填写报名理由");
            label->setText("请填写您的报名理由，例如您有什么优势(不要填写$ #等字符)");
            change_cer->resize(350,300);
            label->resize(250,20);
            label->move(15,12);
            text->resize(320,220);
            text->move(15,42);
            btn->resize(80,16);
            btn->move(135,272);
            btn->setText("确定");
            connect(btn,&QPushButton::clicked,change_cer,[=](){
                QMessageBox::information(change_cer,"提示","您成功报名",QMessageBox::Ok);
                i->m_reviewers  += QString("%1$%2#").arg(Widget::key).arg(text->toPlainText());
                change_cer->close();
            });
            change_cer->exec();
        }
        else {
            QMessageBox::information(this,"提示","报名期限已过",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this,"提示","您的积分不足30，不能报名审核人",QMessageBox::Ok);
    }
}
//发布任务界面
void assignment_ui::on_pushButton_sort_clicked()
{
    if(ui->comboBox_001->currentIndex() != 0){
        QString item = ui->comboBox_001->currentText();
        if(ui->comboBox->currentText()=="正序"){
            ui->tableWidget_0->sortItems(ui->comboBox_001->currentIndex()-1,Qt::AscendingOrder);
        }
        else {
            ui->tableWidget_0->sortItems(ui->comboBox_001->currentIndex()-1,Qt::DescendingOrder);
        }
    }
}
//排序功能
void assignment_ui::on_pushButton_sort_2_clicked()
{
    if(ui->comboBox_002->currentIndex() != 0){
        QString item = ui->comboBox_002->currentText();
        if(ui->comboBox_2->currentText()=="正序"){
            ui->tableWidget_1->sortItems(ui->comboBox_002->currentIndex()-1,Qt::AscendingOrder);
        }
        else {
            ui->tableWidget_1->sortItems(ui->comboBox_002->currentIndex()-1,Qt::DescendingOrder);
        }
    }
}
//查看详细信息
void assignment_ui::on_tableWidget_0_itemDoubleClicked(QTableWidgetItem *item)
{
    int current_row = item->row();
    int num = ui->tableWidget_0->item(current_row,0)->text().toInt();
    QDialog* dlg = new QDialog(this);
    QTextBrowser* bro = new QTextBrowser(dlg);
    dlg->resize(250,190);
    bro->setText(Widget::r_passignments->find(num)->m_snopsis.m_sub_title);
    bro->resize(240,180);
    bro->move(5,5);
    dlg->show();
}
