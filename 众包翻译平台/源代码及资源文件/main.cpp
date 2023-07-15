#include<QTextCodec>
#include <qtextcodec.h>
#include "widget.h"
#include <QApplication>
#include<QDebug>
#include<QTextCodec>
#include<QHash>
#include <QStringListIterator>
#include<QObject>
#include"dlg_register.h"
#include"user.h"
#include"assignment.h"

using namespace std;

/*
 * 在用户类里加入翻译的语言
 * 登陆成功后提示语句
 * 通讯界面右键单击联系人出现菜单，里面应该有删除聊天记录的选项
 * 通讯界面自己和对方的标题可以分别在界面两侧
 * 注册时各项的合法性检查
 * 通讯列表的显示应设为用户名
 * 为用户加入评分属性
 * 严格来说，以发送的时间按区分消息不太严谨
 * 还未分配薪酬
 * 表格显示列宽不合适
 * ddl应该提前规划好
 * 设置金钱范围
*/


int main(int argc, char *argv[])
{
    //置于最开头，勿动
    if(QT_VERSION>=QT_VERSION_CHECK(5,6,0)){
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    qDebug()<<"创建Widget对象"<<endl;
    Widget w;
    if(Widget::exit_flag){//实现主界面关闭后程序也关闭
        a.quit();
        return 0;
    }
    else {
        qDebug()<<"主界面将要显示"<<endl;
        w.show();
        return a.exec();
    }
}

