#include "dlg_system_message.h"
#include "ui_dlg_system_message.h"
#include"Widget.h"
#include<QDebug>
dlg_system_message::dlg_system_message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg_system_message)
{
    //导入消息
    ui->setupUi(this);
    int num = 0;
    ui->tableWidget_contents->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->tableWidget_contents->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
//    ui->tableWidget_contents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->tableWidget_contents->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    ui->tableWidget_contents->horizontalHeader()->setStretchLastSection(true);
    for(QVector<message>::iterator i = Widget::out_pmessages->begin() ; i < Widget::out_pmessages->end();i++){
        if(i->get_sender()=="_system"){
            int row_c = ui->tableWidget_contents->rowCount();
            ui->tableWidget_contents->insertRow(row_c);
            QTableWidgetItem* item0 = new QTableWidgetItem(i->get_date());
            QTableWidgetItem* item1 = new QTableWidgetItem(i->get_title());
            QTableWidgetItem* item2 = new QTableWidgetItem(QString::number(((static_cast<int>(i->get_state())))));
            ui->tableWidget_contents->setItem(num,0,item0);
            ui->tableWidget_contents->setItem(num,1,item1);
            ui->tableWidget_contents->setItem(num,2,item2);
            num++;
        }
    }
    if(num == 0){
        ui->btn_delete->hide();
        ui->btn_delete->setDisabled(true);
    }
    QStringList list1;
    for(int i = 1; i <= num; i++){
        list1.push_back(QString::number(i));
    }
    ui->tableWidget_contents->setVerticalHeaderLabels(list1);
}

dlg_system_message::~dlg_system_message()
{
    delete ui;
}
//双击事件
void dlg_system_message::on_tableWidget_contents_itemDoubleClicked(QTableWidgetItem *item1)
{
    int num = ui->tableWidget_contents->currentRow();
    QString date = ui->tableWidget_contents->item(num,0)->text();
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("详细信息");
    dlg->resize(300,200);
    QTextBrowser* textb = new QTextBrowser(dlg);
    textb->resize(290,190);
    textb->move(5,5);
    for(QVector<message>::iterator i = Widget::out_pmessages->begin();i!=Widget::out_pmessages->end();i++){
        if(i->get_date()==date){
            textb->setText(i->get_text());
            i->change_state();
            break;
        }
    }
    dlg->show();

}
//删除消息
void dlg_system_message::on_btn_delete_clicked()
{
    if(ui->tableWidget_contents->item(ui->tableWidget_contents->currentRow(),ui->tableWidget_contents->currentColumn()) != NULL){
        int num = ui->tableWidget_contents->currentRow();
        if(num != -1){
            QString date = ui->tableWidget_contents->item(num,0)->text();
            for(QVector<message>::iterator i = Widget::out_pmessages->begin();i!=Widget::out_pmessages->end();i++){
                if(i->get_date()==date&&i->get_sender()=="_system"){
                    Database here;
                    here.delete_messages(*i);
                    Widget::out_pmessages->erase(i);
                    break;
                }
            }
            //从表格里删除消息
            ui->tableWidget_contents->takeItem(num,0);
            ui->tableWidget_contents->takeItem(num,1);
            ui->tableWidget_contents->takeItem(num,2);
            ui->tableWidget_contents->removeRow(num);

        }
        else {
            QMessageBox::information(this,"提示","删除失败",QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this,"提示","不能删除",QMessageBox::Ok);
    }
}
