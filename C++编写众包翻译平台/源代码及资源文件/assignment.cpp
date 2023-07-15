#include "assignment.h"
#include<map>
#include<QMessageBox>
#include<utility>
#include"dlg_r_alllco.h"
#include"dlg_c_view.h"
#include "dlg_allco_c.h"
#include"user_ui.h"
#include"dlg_update.h"
Assignment::Assignment(QString date_release,QString ddl, QString title,QString releaser,
                       QString sub_title,QString o_language,QString language,
                       int state,int No,float salary, const QString tran, const QString ori)
    :m_snopsis(date_release,ddl,title,releaser,sub_title,o_language,language,state,No,salary)
{
    m_translation=tran;
    m_oringin=ori;
}
Assignment::Assignment(const Assignment& other)
{
    m_snopsis=other.m_snopsis;
    m_translation=other.m_translation;
    m_oringin= other.m_oringin;
}
//float Assignment::get_salary() const{
//    return m_snopsis.m_salary;
//}
//void Assignment::change_salary(float salary){
//    m_snopsis.m_salary = salary;
//}
//float C_assignment::get_salary() const{
//    return m_c_salary;
//}
//void C_assignment::change_salary(float salary){
//    m_c_salary = salary;
//}
//float R_assignment::get_salary() const
//{
//    return this->C_assignment::Assignment::get_salary();
//}
//void R_assignment::change_salary(float salary){
//    this->Assignment::change_salary(salary);
//}
//float T_assignment::get_salary() const
//{
//    return m_t_salary;
//}
//void T_assignment::change_salary(float salary)
//{
//    this->m_t_salary = salary;
//}
//void Assignment::change_ddl(QString time)
//{
//    this->m_snopsis.m_ddl = time;
//}
//void C_assignment::change_ddl(QString time)
//{
//    this->m_c_ddl = time;
//}

//void R_assignment::change_ddl(QString str)
//{
//    this->m_c_ddl = str;
//}
Snopsis::Snopsis(QString date_release/*="00-00-00 00:00:00"*/,
                 QString ddl/*="00-00-00 00:00:00"*/, QString title/*=none"*/,
                 QString releaser /*=none*/, QString sub_title/*="none"*/,
                 QString o_language,QString language /*= "English"*/,
                 int state /*= recruit_c*/,int No /*= 0*/,float salary/*=0*/){
    m_date_release=date_release;
    m_ddl=ddl;
    m_title=title;
    m_language = language;
    m_No = No;
    m_state =static_cast<enum State>(state);
    m_sub_title = sub_title;
    m_releaser = releaser;
    m_salary = salary;
    m_o_language = o_language;
}
Snopsis::Snopsis(const Snopsis& other){
    m_date_release=other.m_date_release;
    m_ddl=other.m_ddl;
    m_title=other.m_title;
    m_language = other.m_language;
    m_No = other.m_No;
    m_state = other.m_state;
    m_sub_title = other.m_sub_title;
    m_releaser = other.m_releaser;
    m_salary = other.m_salary;
}
C_assignment::C_assignment(QString date_release,QString ddl, QString title,QString releaser,
                           QString sub_title,QString o_language,QString language,
                           int state,int No,float salary,QString translation,QString oringin,
                           QString contractor,float c_salary,QString info,QString c_ddl,QString translatorss ,
                           QString translators,QString c_evaluation,int min, int max)
:Assignment (date_release,ddl,title,releaser,sub_title,o_language,language,
                     state,No,salary,translation,oringin)
{
    m_contractor = contractor;
    m_c_salary = c_salary;
    m_info = info;
    m_c_ddl = c_ddl;
    m_translatorss = translatorss;
    m_translators = translators;
    m_c_evaluation = c_evaluation;
    m_t_salary_max = max;
    m_t_salary_min = min;
}
R_assignment::R_assignment(QString date_release,QString ddl, QString title,QString releaser,
                           QString sub_title,QString o_language,QString language,int state,
                           int No,float salary,QString translation,QString oringin,
                           QString contractor,float c_salary,QString info,QString c_ddl,
                           QString reviewer,QString contractors,QString translators,QString t_ddl,
                           QString translatorss,QString reviewers,QString re_evaluation,float re_salary,
                           QString c_evaluation,int min, int max)
    :C_assignment (date_release,ddl, title,releaser,sub_title,o_language,language,state,No,salary,
                   translation,oringin,contractor,c_salary,info,c_ddl,translatorss,translators,
                   c_evaluation,min,max){
    m_reviewer = reviewer;
    m_contractors = contractors;
    m_t_ddl = t_ddl;
    m_reviewers = reviewers;
    m_re_salary = re_salary;
    m_re_evaluation = re_evaluation;
}
T_assignment::T_assignment(QString date_release,QString ddl, QString title,
                           QString releaser, QString sub_title,QString o_language,
                           QString language ,int state ,int No ,float salary,
                           QString t_translation,QString t_oringin,
                           float t_salary,QString translator,
                           QString info,QString c_evaluation,QString r_evaluation,int t_No)
    :Assignment (date_release,ddl,title,releaser,sub_title,o_language,
                         language,state,No,salary,t_translation,t_oringin)
{
    m_t_salary = t_salary;
    m_c_evaluation = c_evaluation;
    m_info =info;
    m_translator = translator;
    m_r_evaluation =r_evaluation;
    m_t_No = t_No;
}
//复制构造函数
T_assignment::T_assignment(const T_assignment& other):Assignment(other)
{
    this->m_info = other.m_info;
    this->m_t_No = other.m_t_No;
    this->m_t_salary = other.m_t_salary;
    this->m_translator = other.m_translator;
    this->m_c_evaluation = other.m_c_evaluation;
    this->m_r_evaluation = other.m_r_evaluation;
}
