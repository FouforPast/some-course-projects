#pragma once
//#ifndef USER_H
//#define USER_H
#include"user_ui.h"
#include<QVector>
#include<QObject>
class User{
private:
    float m_balance;//余额
    QString m_sex;//性别
    QString m_username;//用户名
    QString m_phoneNumber;//手机号码
    QString m_password;//密码
//    QVector<int> m_assignment_number;//任务编号//有改动//将此项删除，任务类里加入用户唯一编号
    QString m_certificate;//语言资质证明
    int m_age;//年龄
    int m_grade;//积分
    QString m_language;//擅长的语言
    int m_can_translate;//是否可以报名参加翻译，1可，0不可
    QString m_contacts;//联系人列表
public:
    User(QString _username = "none", QString _password = "none", QString _phoneNumber="none",
         int _age=0, QString _sex="none", float _balance = 0,
         QString _certificate ="none", /*int _assignment = 0,*/
         /*QString _email ="none",*/int m_grade = 0,int can_translate=0,QString lan = "",QString con ="");//构造函数
    User(const User& other);//拷贝构造函数
    friend class user_ui;
    void change_money(const float& money);//充值&领取酬金
    bool change_psd(const QString& new_psd);//修改密码
    void change_age(const int& age);
    void change_username(const QString& new_name);
    void change_cer(const QString& new_cer);
//    void change_email(const QString& new_email);
    void change_grade(const int& grade){m_grade += grade;}
    void add_language(QString str);
//    void communicate(const QString& message);//通信
    QString get_username(){return m_username;}
//    const QVector<int>& get_assignment_number(){return m_assignment_number;}//返回常引用
    QString get_certificate(){return m_certificate;}
//    QString get_email(){return m_email;}
    QString get_sex(){return m_sex;}
    QString get_password(){return m_password;}
    float get_balance(){return m_balance;}
    int get_age(){return m_age;}
    QString get_phoneNumber(){return m_phoneNumber;}
    int get_grade(){return m_grade;}
    int get_can_translate(){return m_can_translate;}
    QString get_language(){return m_language;}
    QString get_contacts(){return m_contacts;}
    void change_contacts(QString con){m_contacts = con;}
    void change_can();
};
//#endif // USER_H
