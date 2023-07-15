#include "admin_view_cer.h"
#include "ui_admin_view_cer.h"

admin_view_cer::admin_view_cer(const message& me, QWidget *parent) :
    QDialog(parent),mess(me),
    ui(new Ui::admin_view_cer)
{
    ui->setupUi(this);
    ui->textBrowser->setText(Widget::pusers->find(mess.get_sender())->get_certificate());
    ui->textBrowser_2->setText(mess.get_text());
}

admin_view_cer::~admin_view_cer()
{
    delete ui;
}
//同意
void admin_view_cer::on_pushButton_clicked()
{
    int num0 = Widget::numof_message;
    int num1 = Widget::in_pmessages->size();
    int num2= num0 +num1 +1000;
    Widget::pusers->find(mess.get_sender())->change_cer(ui->textBrowser_2->toPlainText());
    message me("_system",mess.get_sender(),"恭喜您修改语言资质证明成功",
               QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num2,1,"申请成功");
    Widget::in_pmessages->push_back(me);
    QMessageBox::information(this,"提示","修改成功",QMessageBox::Ok);
    if(Widget::pusers->find(mess.get_sender())->get_can_translate() == 0){
        Widget::pusers->find(mess.get_sender())->change_can();
    }
    this->close();
}
//拒绝
void admin_view_cer::on_pushButton_2_clicked()
{
    QDialog* dlg =new QDialog(this);
    QTextEdit* textt = new QTextEdit(dlg);
    QPushButton* btn = new QPushButton(dlg);
    btn->resize(50,20);
    btn->setText("确定");
    textt->setPlaceholderText("请在此处填写您拒绝的理由");
    dlg->resize(310,210);
    textt->resize(300,200);
    textt->move(5,5);
    connect(btn,&QPushButton::clicked,dlg,[=](){
        if(textt->toPlainText()== ""){
            QMessageBox::information(this,"提示","理由不得为空",QMessageBox::Ok);
        }
        else {
            QString text = textt->toPlainText();

            int num10 = Widget::numof_message;
            int num11 = Widget::in_pmessages->size();
            int num12= num10 +num11 +1000;
            message me("_system",mess.get_sender(),text,
                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"申请失败");
            Widget::in_pmessages->push_back(me);
            QMessageBox::information(this,"提示","发送成功",QMessageBox::Ok);
            dlg->close();
        }
    });
    dlg->show();
}
