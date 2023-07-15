//#pragma once
#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#include"assignment_ui.h"
#include"user.h"
#include"dlg_delay_ddl.h"
#include<QString>
#include<QDateTime>
#include<iostream>
class user_ui;
class dlg_r_alllco;
class dlg_c_view;
class assignment_ui;
class dlg_allco_c;
enum State{
    recruit_c = 1,//招募负责人
    recruit_t,//招募译者
    translating,//正在翻译中
    completed//已完成
};
//任务概要
class Snopsis
{
private:
    QString m_date_release;//发布日期
    QString m_ddl;//截止日期
    QString m_title;//标题
    QString m_releaser;//发布者
    QString m_sub_title;//发布任务简介
    QString m_o_language;//原语言
    QString m_language;//翻译的语言
    enum State m_state;//任务状态
    int m_No;//任务唯一标识
    float m_salary;//报酬
public:
    Snopsis(QString date_release="2000-01-01 00:00:00",QString ddl ="2000-01-01 00:00:00",
            QString title = "none",QString releaser = "nobody",QString sub_title = "none",
            QString o_language = "none",QString language = "none",
            int state = recruit_c,int No = 0,float salary = 0);
    Snopsis(const Snopsis& other);
    friend class Assignment;
    friend class R_assignment;
    friend class assignment_ui;
    friend class T_assignment;
    friend class Database;
    friend class user_ui_task;
    friend class dlg_r_alllco;
    friend class dlg_c_view;
    friend class dlg_allco_c;
    friend class user_ui;
    friend class dlg_delay_ddl;
    friend class dlg_update;
    ~Snopsis(){}
};

//任务类
class Assignment
{
protected:
    Snopsis m_snopsis;//简介
    QString m_translation;//译文
    QString m_oringin;//原文

public:
    Assignment(QString date_release="2000-01-01 00:00:00",QString ddl ="2000-01-01 00:00:00",
               QString title = "none",QString releaser = "nobody",QString sub_title = "none",
               QString o_language = "none",QString language = "none",
               int state = recruit_c,int No = 0,float salary = 0,
               QString translation = "none",QString oringin = "none");
    Assignment(const Assignment& other);//拷贝构造函数
    virtual ~Assignment(){}
    const QString& get_translation(){return m_translation;}//得到译文,常引用
    const QString& get_oringin(){return m_oringin;}//得到原文，常引用
    void change_translation(QString str){m_translation = str;}
    void change_oringin(QString str){m_oringin = str;}
    friend class assignment_ui;
    friend class R_assignment;
    friend class Database;
    friend class user_ui_task;
    friend class dlg_r_alllco;
    friend class dlg_c_view;
    friend class dlg_allco_c;
    friend class user_ui;
    friend class dlg_delay_ddl;
    friend class dlg_update;
};
//负责的任务 charge_asignment
class C_assignment: public Assignment
{
protected:
    QString m_contractor;//此翻译任务唯一的负责人
    float m_c_salary;//负责人的报酬
    QString m_translatorss;//此任务相关的翻译者
    QString m_translators;//和此任务最终的翻译者
    QString m_info; //此负责任务的简介
    QString m_c_ddl;//翻译者报名的ddl
    int m_t_salary_min;//翻译者报酬下限
    int m_t_salary_max;//翻译者报酬上限
    QString m_c_evaluation;//发布者的评价
public:
    C_assignment(QString date_release,QString ddl,QString title,
                 QString releaser,QString sub_title,QString o_language,
                 QString language,int state,
                 int No,float salary,QString translation,QString oringin,
                 QString contractor="nobody",float c_salary=0,QString info="none",
                 QString c_ddl="2000-01-01 00:00:00",QString translatorss = "",
                 QString translators = "",QString c_evaluation ="",int min = 0, int max = 0);
     ~C_assignment(){}//析构函数变为虚函数
    friend class assignment_ui;
    friend class Database;
    friend class user_ui_task;
    friend class dlg_r_alllco;
    friend class dlg_c_view;
    friend class dlg_allco_c;
    friend class user_ui;
    friend class dlg_delay_ddl;
    friend class dlg_update;
    friend class dlg_update;
    float get_salary();
};
//发布的任务 release_assignment最详细
class R_assignment: public C_assignment
{
private:
    QString m_reviewer;//审核人
    QString m_reviewers;//和此任务相关的审核人
    QString m_contractors;//和此任务相关的负责人
    QString m_t_ddl;//此任务完成的最终期限
    QString m_re_evaluation;//对审核人的评价
    float m_re_salary;//审核人的报酬
public:
    friend class assignment_ui;
    R_assignment(QString date_release="2000-01-01 00:00:00",QString ddl ="2000-01-01 00:00:00",
                 QString title = "none",QString releaser = "nobody",QString sub_title = "none",
                 QString o_language = "none",QString language = "none",
                 int state = recruit_c,int No = 0,float salary = 0,
                 QString translation = "",QString oringin = "",
                 QString contractor = "nobody",float c_salary=0,QString info = "none",
                 QString c_ddl="2000-01-01 00:00:00",QString reviewer="nobody",
                 QString contractors="",
                 QString translators="",QString t_ddl="2000-01-01 00:00:00",
                 QString translatorss="",QString reviewers="",QString re_evauetion="",
                 float re_salary=0,QString c_evaluation  = "",
                 int min = 0,int max = 0);
    ~R_assignment(){}
    friend class Database;
    friend class user_ui_task;
    friend class dlg_r_alllco;
    friend class dlg_c_view;
    friend class dlg_allco_c;
    friend class user_ui;
    friend class dlg_delay_ddl;
    friend class dlg_update;
};
//翻译的任务 translation_assignment
class T_assignment: public Assignment
{
private:
    float m_t_salary;//翻译者的报酬
    QString m_translator;//此翻译任务唯一的翻译人
    QString m_info; //此翻译任务的简介
    QString m_c_evaluation;//负责人的评价
    QString m_r_evaluation;//审核人的评价
    int m_t_No;//该子任务在数据库中存储的唯一标识
public:
    friend class assignment_ui;
    T_assignment(QString date_release = "00-00-00 00:00:00",QString ddl = "00-00-00 00:00:00",
                 QString title ="none",
                 QString releaser = "nobody", QString sub_title = "none",QString o_language = "none",
                 QString language = "none",int state = 1 ,int No = 0 ,float salary = 0,
                 QString t_tran = "none", QString t_ori = "none",
                 float t_salary = 0,QString translator = "nobody",
                 QString info ="none",QString c_evaluation ="",
                 QString r_evaluation = "",int t_No = 0);
    T_assignment(const T_assignment& other);
    ~T_assignment(){}
    friend class Database;
    friend class user_ui_task;
    friend class dlg_r_alllco;
    friend class dlg_c_view;
    friend class dlg_allco_c;
    friend class user_ui;
    friend class dlg_delay_ddl;
    friend class dlg_update;
};
#endif // ASSIGNMENT_H
