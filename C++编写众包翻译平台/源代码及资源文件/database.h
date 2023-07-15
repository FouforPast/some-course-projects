#ifndef DATABASE_H
#define DATABASE_H
#include<QtSql/QSql>
#include<QHash>
#include <QStringListIterator>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include<QtSql/QSqlDatabase>
#include"assignment.h"
#include"message.h"
class Database
{
private:

public:
    Database();
    static QSqlDatabase s_database;

    void openDataBase(QHash<QString,User>& users);
    void ingressDataBase(QHash<QString,User>& users);
//存储主任务
    bool ingressDataBase(const T_assignment& subassignment);
    bool openDataBase(T_assignment& subassignment,QString translator,int No);
//存储子任务
    void ingressDataBase(QHash<int,R_assignment>& assignments);
    void openDataBase(QHash<int,R_assignment>& assignments);
    void openDataBase(QVector<QString>& contacts,QString target);
    void ingressDataBase(QVector<QString>& contacts,QString target);
    void openDataBase(QVector<message>& out_messages);
    void ingressDataBase(QVector<message>& in_messages);
    void openDataBase(QVector<Validation>& validation);
    void ingressDataBase(QVector<Validation>& validation);
    int num_of_messages();
    int num_of_T_assifnment();
    bool delete_messages(message& d_message);
};

#endif // DATABASE_H
