#include "user_ui.h"
#include "ui_user_ui.h"
#include"user.h"
#include<QHash>
#include<QString>
#include<QPainter>
#include<QPixmap>
#include<QImage>
#include"widget.h"
#include<QDialog>
#include<QSpinBox>
#include<QMessageBox>
#include<QDebug>
user_ui::user_ui(QWidget *parent) :
    QMainWindow (parent),
    ui(new Ui::user_ui)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    this->resize(500,350);
    QHash<QString,User>::iterator i = Widget::pusers->find(Widget::key);
    ui->label_money->setText(QString::number(double(i->get_balance())));
    //设置性别显示
    QPixmap* pixmap_b = new QPixmap;
    QPixmap* pixmap_g = new QPixmap;
    pixmap_b->load("://image/boy.jpg");
    pixmap_g->load("://image/girl.jpeg");
    pixmap_g->scaled(ui->label_sex->size(),Qt::KeepAspectRatio);
    pixmap_b->scaled(ui->label_sex->size(),Qt::KeepAspectRatio);
//    ui->label_sex->setScaledContents(true);
//    ui->label_sex->setPixmap(i->get_sex()=="male"? *pixmap_b: *pixmap_g);
    ui->label_sex->setText(i->get_sex()=="male"? "male":"female");
    ui->label_grade->setText(QString::number(i->get_grade()));
    ui->label_phone->setText(i->get_phoneNumber());
    ui->linet_username->setText(i->get_username());
    ui->linet_age->setText(QString::number(i->get_age()));
    ui->linet_age->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));//只能输入数字
    ui->textBrowser_cer->setText(i->get_certificate());
    //设置任务显示
    ui->tableWidget_r->setSelectionBehavior(QAbstractItemView::SelectRows);//整行选中的方式
    ui->tableWidget_c->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_t->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_re->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_r->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止修改
    ui->tableWidget_c->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_re->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //开始导入数据
    int row_c;
    QStringList list;
    int rows[4] = {0,0,0,0};
    for (QHash<int,R_assignment>::iterator i =Widget::r_passignments->begin();
         i != Widget::r_passignments->end();i++) {
        if(i->m_snopsis.m_releaser == Widget::key){//我发布的
            row_c = ui->tableWidget_r->rowCount();
            ui->tableWidget_r->insertRow(row_c);
            QStringList list;
            list<<QString::number(i->m_snopsis.m_No)<<i->m_snopsis.m_title<<QString::number(int(i->m_snopsis.m_state))<<i->m_contractor<< QString::number(int(i->m_snopsis.m_salary))
               << i->m_snopsis.m_o_language
               <<i->m_snopsis.m_language
              <<i->m_snopsis.m_date_release<<i->m_c_ddl<<i->m_t_ddl<<i->m_snopsis.m_ddl;
            //插入数据
            for(int j = 0;j<list.size();j++){

                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_r->setItem(rows[0],j,item1);
            }
            rows[0]++;
            list.clear();
        }
        //经度有丢失
        if(i->m_contractor == Widget::key){
            row_c = ui->tableWidget_c->rowCount();
            ui->tableWidget_c->insertRow(row_c);
            list<< QString::number(i->m_snopsis.m_No)<<i->m_snopsis.m_title<<QString::number(int(i->m_snopsis.m_state))
                <<QString::number(i->m_c_salary)
               <<i->m_snopsis.m_releaser<< i->m_snopsis.m_o_language<<i->m_snopsis.m_language
              <<i->m_t_ddl<<i->m_snopsis.m_ddl;
            //插入数据

            for(int j = 0;j<list.size();j++){
                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_c->setItem(rows[1],j,item1);
            }
            rows[1]++;
            list.clear();
        }
        if(i->m_translators.contains(Widget::key)){
            row_c = ui->tableWidget_t->rowCount();
            ui->tableWidget_t->insertRow(row_c);
            Database* here = new Database;
            T_assignment t_j;
            here->openDataBase(t_j,Widget::key,i->m_snopsis.m_No);
            list<< QString::number(i->m_snopsis.m_No)<<QString::number(t_j.m_t_No)
                <<i->m_snopsis.m_title<<QString::number(int(i->m_snopsis.m_state))<<QString::number(t_j.m_t_salary)
                <<i->m_contractor<<i->m_snopsis.m_releaser
               << i->m_snopsis.m_o_language<<i->m_snopsis.m_language<<i->m_t_ddl;
            //插入数据
            for(int j = 0;j<list.size();j++){
                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_t->setItem(rows[2],j,item1);
            }
            rows[2]++;
            list.clear();
        }
        if(i->m_reviewer == Widget::key){
            row_c = ui->tableWidget_re->rowCount();
            ui->tableWidget_re->insertRow(row_c);
            list<< QString::number(i->m_snopsis.m_No)<<i->m_snopsis.m_title<<QString::number(int(i->m_snopsis.m_state))<<QString::number(i->m_re_salary)<<i->m_contractor<<i->m_snopsis.m_releaser
                <<i->m_snopsis.m_o_language<<i->m_snopsis.m_language<<i->m_t_ddl;
            //插入数据
            for(int j = 0;j<list.size();j++){
                QTableWidgetItem* item1 = new QTableWidgetItem(list.at(j));
                ui->tableWidget_re->setItem(rows[3],j,item1);
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
    ui->tableWidget_r->setVerticalHeaderLabels(list2[0]);
    ui->tableWidget_c->setVerticalHeaderLabels(list2[1]);
    ui->tableWidget_t->setVerticalHeaderLabels(list2[2]);
    ui->tableWidget_re->setVerticalHeaderLabels(list2[3]);
}

user_ui::~user_ui()
{
    delete ui;
}
void user_ui::on_commandLinkButton_clicked()
{
    this->close();
}

void user_ui::on_commandLinkButton_2_clicked()
{
    this->close();
}
//信息有改动提示窗口，否则直接退出
void user_ui::closeEvent(QCloseEvent *ev){
    QHash<QString,User>::iterator i = Widget::pusers->find(Widget::key);
    if(ui->linet_age->text().toInt()!=i->get_age()||ui->linet_username->text()!=i->get_username()){
        if(QMessageBox::Yes == QMessageBox::question(this,"是否退出","您的信息尚未保存，是否退出",QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
            ev->accept();
        }
        else {
            ev->ignore();
        }
    }
    else {
        ev->accept();
    }
}
//转到我发布的
void user_ui::on_pushButton_r_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
//转到我负责的
void user_ui::on_pushButton_c_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
//转到我翻译的
void user_ui::on_pushButton_t_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
//转到我审核的
void user_ui::on_pushButton_re_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//转到我翻译的详细信息
void user_ui::on_pushButton_t_detail_clicked()
{
    QTableWidgetItem* item = ui->tableWidget_t->item(ui->tableWidget_t->currentRow(),0);
    if(item == NULL){
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    int num = item->text().toInt();
    task_ui = new dlg_user_task_ui(2,num,this);
    if(task_ui == NULL)
    {
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    task_ui->show();
}
//转到我发布的详细信息
void user_ui::on_pushButton_r_detail_clicked()
{
    QTableWidgetItem* item = ui->tableWidget_r->item(ui->tableWidget_r->currentRow(),0);
    if(item == NULL){
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    int num = item->text().toInt();
    task_ui = new dlg_user_task_ui(0,num,this);
    task_ui->show();
}
//转到我负责的详细信息
void user_ui::on_pushButton_c_detail_clicked()
{
    QTableWidgetItem* item = ui->tableWidget_c->item(ui->tableWidget_c->currentRow(),0);
    if(item == NULL){
        QMessageBox::information(this,"提示","请先选中一个对象",QMessageBox::Ok);
        return;
    }
    int num = item->text().toInt();
    task_ui = new dlg_user_task_ui(1,num,this);
    task_ui->show();
}
//转到我审核的详细信息
void user_ui::on_btn_re_detail_clicked()
{
    QTableWidgetItem* item = ui->tableWidget_re->item(ui->tableWidget_re->currentRow(),0);
    if(item == NULL){
        QMessageBox::information(this,"提示","查看失败",QMessageBox::Ok);
        return;
    }
    int num = item->text().toInt();
    task_ui = new dlg_user_task_ui(3,num,this);
    task_ui->show();
}

//充值
void user_ui::on_pushButton_recharge_clicked()
{
    QHash<QString,User>::iterator i = Widget::pusers->find(Widget::key);
    QDialog* recharge = new QDialog(this);
    recharge->setWindowTitle("充值");
    QLabel* label = new QLabel(recharge);
    label->setText("请输入您要充值的金额(0~10000)");
    QLineEdit* linet = new QLineEdit(recharge);
    linet->setPlaceholderText("输入金额");
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
        float money = linet->text().toFloat();//获取money的值应该在点击确定之后
        if(money <=0 ){
            QMessageBox::information(recharge,"提示","输入金额不得为负数",QMessageBox::Ok);
            linet->clear();
        }
        else if(money >10000){
            QMessageBox::information(recharge,"提示","输入金额不得超过10000",QMessageBox::Ok);
            linet->clear();
        }
        else {
            i->change_money(money);
            QMessageBox::information(recharge,"充值成功","恭喜您，充值成功",QMessageBox::Ok);
            ui->label_money->setText(QString::number(double(money)));
            recharge->close();
        }
    });
}
//修改密码
void user_ui::on_pushButton_changepsd_clicked()
{
    QHash<QString,User>::iterator i = Widget::pusers->find(Widget::key);
    QDialog* change_psd = new QDialog(this);
    change_psd->setWindowTitle("修改密码");
//    QLabel* label = new QLabel(change_psd);
//    label->setText("请输入原密码");
//    QLabel* label_2 = new QLabel(change_psd);
//    label->setText("请输入新密码(5~15位)");
    QLineEdit* linet = new QLineEdit(change_psd);
    QLineEdit* linet_2 = new QLineEdit(change_psd);
    linet->setPlaceholderText("请输入原密码");
    linet_2->setPlaceholderText("请输入新密码(5~15位)");
    QPushButton* btn_ok = new QPushButton(change_psd);
    btn_ok->setText("确定");
    //设置大小
    change_psd->resize(200,200);
//    label->resize(60,22);
//    label->move(6,36);
//    label_2->resize(60,22);
//    label_2->move(6,92);
    linet->resize(180,32);
    linet->move(10,36);
    linet_2->resize(180,32);
    linet_2->move(10,92);
    btn_ok->resize(60,20);
    btn_ok->move(70,150);
    change_psd->show();
    //执行操作
    connect(btn_ok,&QPushButton::clicked,change_psd,[=](){
        if(linet->text()!=i->get_password()){
            QMessageBox::warning(change_psd,"错误","您输入的原密码不正确",QMessageBox::Ok);
            linet->clear();
        }
        else if(linet_2->text().length()<5||linet_2->text().length()>15){
            QMessageBox::warning(change_psd,"错误","您输入的密码位数不符合格式",QMessageBox::Ok);
            linet_2->clear();
        }
        else {
            if(i->change_psd(linet_2->text())){
                QMessageBox::information(change_psd,"提示","密码修改成功",QMessageBox::Ok);
                change_psd->close();
            }
        }
    });
}
//修改个人资质证明
void user_ui::on_pushButton_changecer_clicked()
{
    if(Widget::pusers->find(Widget::key)->get_grade() > 0){
        QHash<QString,User>::iterator i = Widget::pusers->find(Widget::key);
        QDialog* change_cer = new QDialog(this);
        QTextEdit* text = new QTextEdit(change_cer);
        QLabel* label = new QLabel(change_cer);
        QPushButton* btn = new QPushButton(change_cer);
        change_cer->setWindowTitle("修改个人资质证明");
        text->setText(i->get_certificate());
        label->setText("请在此处修改您的个人资质证明");
        change_cer->resize(350,300);
        label->resize(150,20);
        label->move(15,12);
        text->resize(320,220);
        text->move(15,42);
        btn->resize(80,16);
        btn->move(135,272);
        btn->setText("确定");
        connect(btn,&QPushButton::clicked,change_cer,[=](){
            QMessageBox::information(change_cer,"提示","您的个人资质证明将会在一周之内得到管理员的审核，待审核通过后，您的"
                                                     "修改才能生效",QMessageBox::Ok);
            change_cer->close();
           int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message me(Widget::key,"admin",text->toPlainText(),
                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"修改资质证明");
            Widget::in_pmessages->push_back(me);
        });
        change_cer->exec();
    }
    else {
        QMessageBox::information(this,"提示","请先在任务界面加入翻译团队",QMessageBox::Ok);
    }
}
//保存并退出按钮
void user_ui::on_pushButton_saveexit_clicked()
{
    QHash<QString,User>::iterator i = Widget::pusers->find(Widget::key);
    if(ui->linet_age->text().toInt() != i->get_age()){
        i->change_age(ui->linet_age->text().toInt());
    }
    if(ui->linet_username->text()!=i->get_username()){
        i->change_username(ui->linet_username->text());
    }
    this->close();
}
