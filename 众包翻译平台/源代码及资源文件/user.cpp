#include "user.h"
#include<QString>
User::User(QString _username, QString _password, QString _phoneNumber, int _age,
           QString _sex, float _balance/* = 0*/,
           QString _certificate /*="暂无"*/, /*int _assignment = 0,*/
           /*QString _email*//*=暂无*/int grade,int can_translate,QString lan,QString cont)
{
    this->m_username=_username;
    this->m_password=_password;
    this->m_phoneNumber = _phoneNumber;//此处应该修改电话为const属性
    this->m_age=_age;
    this->m_balance=_balance;
    this->m_certificate=_certificate;
//    this->m_assignment_number=_assignment;
//    this->m_email=_email;
    this->m_sex=_sex;
    this->m_grade = grade;
    this->m_can_translate = can_translate;
    this->m_language = lan;
    this->m_contacts = cont;
}
User::User(const User& other){
    this->m_username=other.m_username;
    this->m_password=other.m_password;
    this->m_phoneNumber = other.m_phoneNumber;//此处应该修改电话为const属性
    this->m_age=other.m_age;
    this->m_balance=other.m_balance;
    this->m_certificate=other.m_certificate;
//    this->m_email=other.m_email;
    this->m_sex=other.m_sex;
    this->m_grade = other.m_grade;
    this->m_can_translate = other.m_can_translate;
    this->m_language = other.m_language;
    this->m_contacts = other.m_contacts;
}
void User::change_money(const float& money){
    float ba = this->m_balance;
    this->m_balance  = money + ba;
}
bool User::change_psd(const QString& new_psd){
    this->m_password = new_psd;
    return true;
}
void User::change_age(const int& age){
    m_age = age;
}

void User::change_cer(const QString &new_cer){
    m_certificate = new_cer;
}
void User::change_username(const QString &new_name){
    m_username = new_name;
}

//void User::change_email(const QString &new_email){
//    m_email = new_email;
//}

void User::add_language(QString str)
{
    m_language += QString(str+"#");
}

void User::change_can()
{
    if(m_can_translate == 0){
        m_can_translate = 1;
    }
    else{
        m_can_translate = 0;
    }
}



