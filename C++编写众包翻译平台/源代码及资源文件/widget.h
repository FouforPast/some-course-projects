#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include<QHash>
#include<QCloseEvent>
#include<QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include<QtSql/QSqlDatabase>
#include"assignment.h"
#include"dlglogin.h"
#include"database.h"
#include"user.h"
#include"message.h"
class message_ui;
namespace Ui {
class Widget;
}

//外部变量
extern int user_flag ;
class Widget : public QWidget
{
    Q_OBJECT
private:

    Database _database;
public:
    int is_A;
    explicit Widget(QWidget *parent = nullptr);
    static QHash<int,R_assignment>* r_passignments;//主任务
    static QHash<QString,User>* pusers;
    static QString key;//用于判断是哪一位用户登录的
    static bool exit_flag;
    static QVector<QString>* pcontacts;
    static QVector<message>* in_pmessages;
    static QVector<message>* out_pmessages;
    static QVector<Validation>* pvalidation;
    static bool no_login_flag;
    static int numof_message;
    static bool close_flag2;
    user_ui* w_user_ui;
    assignment_ui* w_assignment_ui;
    dlgLogin* w_login;
    message_ui* w_message_ui;
    ~Widget();

private slots:
    void on_btn_mine_clicked();
    void closeEvent(QCloseEvent* ev);
    void on_btn_assignment_clicked();
    void on_btn_communication_clicked();

    void on_pushButton_message_system_clicked();

    void on_pushButton_admin_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
