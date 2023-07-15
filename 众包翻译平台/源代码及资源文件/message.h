#ifndef MESSAGE_H
#define MESSAGE_H
#include<QString>
#include<QHash>
class message_ui;
class Database;
enum message_state{
    to_be_read = 1,//未读
    read//已读
};
//验证消息
class Validation
{
private:
    QString m_date;
    QString m_sender;
    QString m_receiver;
    enum message_state m_state;
public:
    Validation(const QString date, const QString sender,const QString receiver,const int state)
        :m_date(date), m_sender(sender),m_receiver(receiver),m_state(static_cast<enum message_state>(state)){}
    friend class Database;
    friend class message_ui;
};
//消息类
class message
{
private:
    QString m_sender;//发送方
    QString m_receiver;//接受方
    QString m_text;//消息内容
    QString m_datetime;//日期时间
    int m_No;
    enum message_state m_state;//消息状态
    QString m_title;//消息的标题
public:
    message(QString sender, QString receiver, QString text, QString datetime,int No,int state= 1,QString title="none")
        :m_sender(sender),m_receiver(receiver),m_text(text),m_datetime(datetime),m_No(No),
         m_state(static_cast<enum message_state>(state)),m_title(title){}
    ~message();
    friend class Database;
    friend class message_ui;
    QString get_sender(){return m_sender;}
    QString get_receiver(){return m_receiver;}
    enum message_state get_state(){return m_state;}
    QString get_date(){return  m_datetime;}
    QString get_title(){return m_title;}
    QString get_text(){return m_text;}
    int get_No(){return m_No;}
    void change_state();
};
#endif // MESSAGE_H
