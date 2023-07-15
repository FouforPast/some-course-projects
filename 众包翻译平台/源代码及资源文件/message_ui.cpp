#include "message_ui.h"
#include "ui_message_ui.h"
#include "message.h"
#include<algorithm>
#include<QPalette>
#include<QDebug>
#include<QColor>
#include<QHash>
#include<QLabel>
#include<QDateTime>
#include<QLineEdit>
message_ui::message_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::message_ui)
{
    ui->setupUi(this);
    ui->btn_delete->setDisabled(true);//设置按钮不可按
    ui->btn_emit->setDisabled(true);
    QPalette pa(QColor(0x00,0xff,0x00,0x00));
    ui->textB_out_message->setPalette(pa);
//    ui->textE_in_message->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->textB_out_message->setAttribute(Qt::WA_TranslucentBackground, true);
    ui->listWidget_contacts->setAttribute(Qt::WA_TranslucentBackground, true);
    m_num = 0;
    for(QVector<Validation>:: iterator i = Widget::pvalidation->begin(); i != Widget::pvalidation->end(); i++){
        if(i->m_receiver == Widget::key && i->m_state==to_be_read){
            m_num++;
        }
    }

    ui->label_num_of_application->setText(QString("%1条").arg(m_num));

    //添加item
    QStringList cont = Widget::pusers->find(Widget::key)->get_contacts().split("$");
    for(int i =0; i < cont.size() - 1; i++)
    {
        QListWidgetItem* item = new QListWidgetItem;
        //        item->setText(Widget::pusers->find(*i)->get_username());
        item->setText(cont.at(i));
        ui->listWidget_contacts->addItem(item);
    }
//    for(QVector<QString>::iterator i= Widget::pcontacts->begin();i!=Widget::pcontacts->end();i++)
//    {
//        QListWidgetItem* item = new QListWidgetItem;
//        //        item->setText(Widget::pusers->find(*i)->get_username());
//        item->setText(*i);
//        ui->listWidget_contacts->addItem(item);
//    }
    ui->listWidget_contacts->setCurrentRow(1);
}
message_ui::~message_ui()
{
    delete ui;
}
//发送消息
void message_ui::on_btn_emit_clicked()
{
    if(ui->textE_in_message->toPlainText().isEmpty()){
        QMessageBox::information(this,"提示","输入消息不得为空",QMessageBox::Ok);
    }
    else if (!Widget::pusers->find(Widget::key)->get_contacts().contains(ui->listWidget_contacts->currentItem()->text())) {
        QMessageBox::information(this,"提示","该好友不存在",QMessageBox::Ok);
    }
    else{
        QDateTime time = QDateTime::currentDateTime();
        QString time_str = time.toString("yyyy-MM-dd hh:mm:ss");
        int num10 = Widget::numof_message;
        int num11 = Widget::in_pmessages->size();
        int num12= num10 +num11 +1000;

        message _message(Widget::pusers->find(Widget::key)->get_phoneNumber(),Widget::pusers->find(ui->listWidget_contacts->currentItem()->text())->get_phoneNumber(),
                         ui->textE_in_message->toPlainText(),time_str,num12,1);
        Widget::in_pmessages->push_back(_message);
        Widget::out_pmessages->push_back(_message);
        m_all_message += QString(Widget::pusers->find(Widget::key)->get_username()+ "         " +time_str+"\n"+ui->textE_in_message->toPlainText()+"\n");
        ui->textB_out_message->setText(m_all_message);
        ui->textE_in_message->clear();
    }
}
//删除好友操作
void message_ui::on_btn_delete_clicked()
{
    QString to_be_deleted = ui->listWidget_contacts->currentItem()->text();
    //删除列表里的好友
    QStringList cont = Widget::pusers->find(Widget::key)->get_contacts().split("$");
    for(int i =0; i < cont.size(); i++)
    {
        if(cont.at(i) == to_be_deleted){
            cont.removeAt(i);
            break;
        }
    }
    Widget::pusers->find(Widget::key)->change_contacts(cont.join("$"));
    //对方删除自己
    QStringList cont1 = Widget::pusers->find(to_be_deleted)->get_contacts().split("$");
    for(int i =0; i < cont.size(); i++)
    {
        if(cont.at(i) == Widget::key){
            cont.removeAt(i);
            break;
        }
    }
    Widget::pusers->find(to_be_deleted)->change_contacts(cont.join("$"));
//    for(QVector<QString>::iterator i = Widget::pcontacts->begin();i!=Widget::pcontacts->end();i++)
//    {
//        if(*i == to_be_deleted){
//            Widget::pcontacts->erase(i);
//            i--;
//        }
//    }
    //删除相关的消息
    Database here;
    for(QVector<message>::iterator i = Widget::out_pmessages->begin();i!=Widget::out_pmessages->end();i++){
        if((i->m_sender==Widget::key&&i->m_receiver==to_be_deleted) ||
                (i->m_receiver==Widget::key&&i->m_sender==to_be_deleted)){
            here.delete_messages(*i);
        }
    }
    QMessageBox::information(this,"提示","您已经删除成功",QMessageBox::Ok);
    ui->listWidget_contacts->removeItemWidget(ui->listWidget_contacts->currentItem());
}
//添加好友
void message_ui::on_btn_add_clicked()
{
    QDialog* dlg_add = new QDialog(this);
    dlg_add->setWindowTitle("添加好友");
    QLabel* label = new QLabel("请输入您要添加的好友的手机号码",dlg_add);
    QLineEdit* linet = new QLineEdit(dlg_add);
    QPushButton* btn = new QPushButton(dlg_add);
    dlg_add->resize(200,120);
    label->resize(140,20);
    label->move(30,10);
    linet->resize(160,20);
    linet->move(20,40);
    btn->resize(50,20);
    btn->move(75,80);
    btn->setText("确定");
    dlg_add->show();
    connect(btn,&QPushButton::clicked,dlg_add,[=](){
        QString phonenum = linet->text();
        QHash<QString,User>::iterator i = Widget::pusers->find(phonenum);//对方的迭代器
        //用户不存在
        if(i == Widget::pusers->end()){
            QMessageBox::information(dlg_add,"提示","您输入的手机号码不存在",QMessageBox::Ok);
            linet->clear();
        }
        else if(Widget::key == phonenum){
            QMessageBox::information(dlg_add,"提示","您不能添加自己为好友",QMessageBox::Ok);
            linet->clear();
        }
        else {
            Validation v(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),Widget::key,phonenum,to_be_read);
            Widget::pvalidation->push_back(v);
            dlg_add->close();
            QMessageBox::information(dlg_add,"提示","您的好友请求已经发送",QMessageBox::Ok);
        }
    });
}
//双击事件
void message_ui::on_listWidget_contacts_itemDoubleClicked(QListWidgetItem *item)
{
    ui->btn_delete->setDisabled(false);//设置按钮变为可按
    ui->btn_emit->setDisabled(false);
    QString target = item->text();
    //导入未读消息(x)，导入所有和自己相关的消息
    m_all_message = "";
    if(Widget::out_pmessages->size() == 1){
        QVector<message>::iterator i =Widget::out_pmessages->begin();
        QString time_str = i->m_datetime;
        m_all_message += QString(Widget::pusers->find(i->get_sender())->get_username()+ "        " +time_str+"\n"+ i->m_text +"\n");
        if(i->m_state==to_be_read){
            i->m_state = read;
        }
    }
    for (QVector<message>::iterator i =Widget::out_pmessages->end()-1;i > Widget::out_pmessages->begin();i--) {
        if((i->m_sender== target&&i->m_receiver==Widget::key)
                ||(i->m_sender==Widget::key&&i->m_receiver==target)/*&&i->m_state==to_be_read*/){
            QString time_str = i->m_datetime;
            m_all_message += QString(Widget::pusers->find(i->get_sender())->get_username()+ "        " +time_str+"\n"+ i->m_text +"\n");

            if(i->m_state==to_be_read){
                i->m_state = read;
            }
        }
    }
    ui->textB_out_message->setText(m_all_message);
}
//单击事件
void message_ui::on_listWidget_contacts_itemClicked(QListWidgetItem *item)
{
    ui->btn_delete->setDisabled(false);//设置按钮变为可按
}


//验证消息框
void message_ui::on_btn_apply_clicked()
{
    //构建及布局
    QDialog* dlg_app = new QDialog(this);
    QListWidget* list = new QListWidget(dlg_app);
    QPushButton* btn_1 = new QPushButton(dlg_app);
    QPushButton* btn_2 = new QPushButton(dlg_app);
    dlg_app->resize(268,191);
    list->resize(256,160);
    list->move(6,6);
    btn_1->resize(50,15);
    btn_1->move(70,170);
    btn_2->resize(50,15);
    btn_2->move(150,170);
    dlg_app->setWindowTitle("验证消息");
    btn_1->setText("接受");
    btn_2->setText("拒绝");
    QListWidgetItem* item = new QListWidgetItem(list);
    item->setText(QString("日期")+"                                       "+"发送者"+"       "+"状态");
    list->setCurrentRow(1);
    btn_1->setDisabled(true);
    btn_2->setDisabled(true);
    //设置内容
    for(QVector<Validation>::iterator i =Widget::pvalidation->begin();i!= Widget::pvalidation->end();i++){
        if(i->m_sender != Widget::key&& i->m_receiver == Widget::key){//过滤自己发送的消息
            QListWidgetItem* item = new QListWidgetItem(list);
            item->setText(i->m_date+"       "+Widget::pusers->find(i->m_sender)->get_username()+"   "+"未读");
        }
    }
    connect(list,&QListWidget::itemSelectionChanged,btn_1,[=](){
        if(list->currentRow() != 0){
            btn_1->setDisabled(false);
            btn_2->setDisabled(false);
            if(list->currentItem()->text()==(QString("日期")+"                                        "+"发送者"+"       "+"状态"))//已读和日期的条目
            {
                btn_1->setDisabled(true);
                btn_2->setDisabled(true);
            }
            else{
                for(QVector<Validation>::iterator i =Widget::pvalidation->begin();i!= Widget::pvalidation->end();i++)//遍历pvalidation
                {
                    if(QString(i->m_date+"       "+Widget::pusers->find(i->m_sender)->get_username()+"   "+"已拒绝")
                            ==list->currentItem()->text()||QString(i->m_date+"       "+Widget::pusers->find(i->m_sender)->get_username()+" "+"已接受")
                            ==list->currentItem()->text()){
                        btn_1->setDisabled(true);
                        btn_2->setDisabled(true);
                    }
                }
            }
        }
    });
    connect(btn_1,&QPushButton::clicked,dlg_app,[=](){ //用户点了接受按钮
        for(QVector<Validation>::iterator i =Widget::pvalidation->begin();i!= Widget::pvalidation->end();i++)//遍历pvalidation
        {
            if(i->m_date+QString("       ")+Widget::pusers->find(i->m_sender)->get_username()+QString("   ")+QString("未读")
                    ==list->currentItem()->text()){//更新已读消息状态
                //更新contacts
                QMessageBox::information(dlg_app,"提示","您已经成功添加对方为好友",QMessageBox::Ok);

                QString cont = Widget::pusers->find(Widget::key)->get_contacts();
                cont += QString(i->m_sender + "$");
                qDebug()<<cont<<endl;
                Widget::pusers->find(Widget::key)->change_contacts(cont);
//                Widget::pcontacts->push_back(i->m_sender);
                //更新对方的联系人状态
//                Database here_database;
//                QVector<QString> reverse_contacts;
//                here_database.openDataBase(reverse_contacts,i->m_sender);
//                reverse_contacts.push_back(Widget::key);
//                here_database.ingressDataBase(reverse_contacts,i->m_sender);

                QString cont1 = Widget::pusers->find(i->m_sender)->get_contacts();
                cont1 += QString(Widget::key + "$");
                Widget::pusers->find(i->m_sender)->change_contacts(cont1);

                i->m_state = read;//更新消息状态
                list->currentItem()->setText(i->m_date+"       "+Widget::pusers->find(i->m_sender)->get_username()+"   "+"已接受");
                QListWidgetItem* item = new QListWidgetItem;
                item->setText(i->m_sender);//此处设置的是显示唯一标识
                ui->listWidget_contacts->addItem(item);
                //防止列表好友重复
                QStringList contlist = Widget::pusers->find(Widget::key)->get_contacts().split("$");
                for(QStringList::iterator j = contlist.begin();j < contlist.end(); j++){
                    if((*j)==i->m_sender){
                        ui->listWidget_contacts->removeItemWidget(item);
                        break;
                    }
                }
//                for(QVector<QString>::iterator j = Widget::pcontacts->begin();j<Widget::pcontacts->end();j++){
//                    if((*j)==i->m_sender){
//                        ui->listWidget_contacts->removeItemWidget(item);
//                        break;
//                    }
//                }
                ui->label_num_of_application->setText(QString("%1条").arg(--m_num));
                btn_1->setDisabled(true);
                btn_2->setDisabled(true);
                //对方添加好友
            }
        }
    });
    connect(btn_2,&QPushButton::clicked,dlg_app,[=](){ //用户点拒绝按钮
        for(QVector<Validation>::iterator i =Widget::pvalidation->begin();i!= Widget::pvalidation->end();i++){
            if(i->m_date+QString("       ")+Widget::pusers->find(i->m_sender)->get_username()+QString("   ")+QString("未读")
                    ==list->currentItem()->text()){
                QMessageBox::information(dlg_app,"提示","您已经拒绝添加对方为好友",QMessageBox::Ok);
                i->m_state = read;
                list->currentItem()->setText(i->m_date+"       "+Widget::pusers->find(i->m_sender)->get_username()+"   "+"已拒绝");
                btn_1->setDisabled(true);
                btn_2->setDisabled(true);
            }
        }
    });
    dlg_app->exec();
}
