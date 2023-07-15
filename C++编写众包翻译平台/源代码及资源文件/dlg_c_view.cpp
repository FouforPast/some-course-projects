#include "dlg_c_view.h"
#include "ui_dlg_c_view.h"
#include"database.h"
#include"widget.h"
#include"assignment.h"
dlg_c_view::dlg_c_view(int _cate,R_assignment* _r1,int _role,QWidget *parent) :
    QDialog(parent),cate(_cate),r1(_r1),role(_role),
    ui(new Ui::dlg_c_view)
{
    ui->setupUi(this);
    ui->comboBox_translator->addItem("翻译者");
    ui->comboBox_translator->setCurrentIndex(0);

    QStringList tran = r1->m_translators.split("$");
    for(int i = 0;i <tran.size() -1;i++){
        ui->comboBox_translator->addItem(tran.at(i));
    }
    if(cate == 0){
        ui->textEdit_evaluation->setPlaceholderText("请于此处填写评价");
        ui->pushButton_submit->setDisabled(true);
        ui->pushButton_submit->hide();
    }
    if(cate == 1){
        ui->textEdit_evaluation->setPlaceholderText("请于此处整合译文");
        ui->pushButton_emit->setDisabled(true);
        ui->pushButton_emit->hide();
    }
}

dlg_c_view::~dlg_c_view()
{
    delete ui;
}
//查看翻译者的译文
void dlg_c_view::on_comboBox_translator_currentIndexChanged(const QString &arg1)
{
    T_assignment t_ass;
    Database here;
    here.openDataBase(t_ass,arg1,r1->m_snopsis.m_No);
    ui->textBrowser_origin->setText(t_ass.m_oringin);
    ui->textBrowser_translation->setText(t_ass.m_translation);
}
//发布评价
void dlg_c_view::on_pushButton_emit_clicked()
{
    if(ui->comboBox_translator->currentText() == "翻译者"){
        QMessageBox::information(this,"提示","未选中对象",QMessageBox::Ok);
        return;
    }
    T_assignment t_ass;
    Database here;
    here.openDataBase(t_ass,ui->comboBox_translator->currentText(),r1->m_snopsis.m_No);
    if(role == 2){
        int num10 = Widget::numof_message;
        int num11 = Widget::in_pmessages->size();
        int num12= num10 +num11 +1000;
    t_ass.m_c_evaluation += ui->textEdit_evaluation->toPlainText();
    message* me = new message("_system",ui->comboBox_translator->currentText(),
                              QString("您主任务编号为%1的子任务得到了负责人的新的评价，快去看看吧").arg(r1->m_snopsis.m_No),
                              QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"评价更新");
    Widget::in_pmessages->push_back(*me);
    delete me;
    }
    if(role == 4){
        int num10 = Widget::numof_message;
        int num11 = Widget::in_pmessages->size();
        int num12= num10 +num11 +1000;
        t_ass.m_r_evaluation += ui->textEdit_evaluation->toPlainText();
        message* me = new message("_system",ui->comboBox_translator->currentText(),
                                  QString("您主任务编号为%1的子任务得到了审核人的新的评价，快去看看吧").arg(r1->m_snopsis.m_No),
                                  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"评价更新");
        Widget::in_pmessages->push_back(*me);
        delete me;
    }
    here.ingressDataBase(t_ass);
}
void dlg_c_view::on_pushButton_submit_clicked()
{
    T_assignment t_ass;
    Database here;
    here.openDataBase(t_ass,ui->comboBox_translator->currentText(),r1->m_snopsis.m_No);
    if(QMessageBox::Yes == QMessageBox::question(this,"提示","您确认要提交吗",QMessageBox::Yes|QMessageBox::No)){
        QMessageBox::information(this,"提示","您已经成功提交",QMessageBox::Ok);
      int num10 = Widget::numof_message;
      int num11 = Widget::in_pmessages->size();
        int num12= num10 +num11 +1000;
        message me("_system",t_ass.m_snopsis.m_releaser,"您的编号为%1的任务负责人已经提交译文",
                   QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),num12,1,"负责人提交译文");
        Widget::in_pmessages->push_back(me);
    }
}
