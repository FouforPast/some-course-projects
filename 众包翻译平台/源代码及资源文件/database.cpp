#include"user.h"
#include "database.h"
#include<QDebug>
#include"widget.h"
#include"assignment.h"

QSqlDatabase Database::s_database = QSqlDatabase::addDatabase("QSQLITE");

Database::Database()
{
    s_database.setDatabaseName("data.db");
}
//向数据库里输入users数据
void Database::openDataBase(QHash<QString, User> &users){
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name='%1'")
            .arg("users");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt() == 0){
            //该表不存在
            query_str="create table users (phonenum QString primary key,"
                      "username QString, psd QString, age int,"
                      " balance float,certificate QString,"
                      "sex QString,grade int,can_t int,language QString,contacts QString)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建错误"<<query.lastError()<<endl;
            }
        }
        else{//表存在,开始导入数据
            query.prepare("select * from users");//向映射中导入数据
            if(!query.exec()){
                qDebug()<<"导入user数据失败"<<query.lastError()<<endl;
            }
            else {
                while(query.next()){
                    User user(query.value("username").toString(),
                              query.value("psd").toString(),
                              query.value("phonenum").toString(),
                              query.value("age").toInt(),
                              query.value("sex").toString(),
                              query.value("balance").toFloat(),
                              query.value("certificate").toString(),
                              /*query.value("assignmentnum").toInt(),*/
                              //                              query.value("email").toString(),
                              query.value("grade").toInt(),
                              query.value("can_t").toInt(),
                              query.value("language").toString(),
                              query.value("contacts").toString());
                    users.insert(query.value("phonenum").toString(),user);
                }
            }
        }
    }
}
//将user中的数据导入到数据库中
void Database::ingressDataBase(QHash<QString, User> &users){
    QSqlQuery query(s_database);
    QString query_str="delete from users";
    query.prepare(query_str);
    if(!query.exec()){//先执行删除表里的数据

        qDebug()<<"删除表里的user数据失败"<<query.lastError()<<endl;
    }
    else{//开始插入数据
        QHash<QString,User>::iterator i;
        for(i=users.begin();i!=users.end();i++){
            query_str=QString("insert into users(phonenum,username,psd,age,balance,"
                              "sex,grade,can_t,language,certificate,contacts)VALUES('%1','%2','%3','%4','%5','%6',"
                              "'%7','%8','%9','%10','%11')").arg(i->get_phoneNumber())
                    .arg(i->get_username()).arg(i->get_password())
                    .arg(i->get_age()).arg(double(i->get_balance()))
                    .arg(i->get_sex()).arg(i->get_grade())
                    .arg(i->get_can_translate()).arg(i->get_language()).arg(i->get_certificate()).arg(i->get_contacts());
            query.prepare(query_str);
            if(!query.exec()){
                qDebug()<<"插入user失败"<<query.lastError()<<endl;
                return;
            }
        }
    }
}
//从数据库里打开主任务
void Database::openDataBase(QHash<int,R_assignment>& assignments)
{
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name='assignments'");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt()==0){//该表不存在
            query_str="create table assignments(No int primary key,"
                      "date_release QString,ddl QString,title QString,"
                      "releaser QString,"
                      "sub_title QString,o_language QString,language QString,"
                      "state int,salary float,"
                      "translation QString,oringin QString,"
                      "contractor QString, c_salary float,"
                      "info QString, c_ddl QString,reviewer QString, contractors QString,"
                      "translators QString,t_ddl QString,reviewers QString,"
                      "translatorss QString,re_evaluation QString,re_salary float,"
                      "c_evaluation QString,min int,max int)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建错误"<<query.lastError()<<endl;
            }
        }

        else{//表存在,开始导入数据
            query.prepare("select * from assignments");//向映射中导入数据
            if(!query.exec()){
                qDebug()<<"导入assignments数据失败"<<query.lastError()<<endl;
            }
            else {
                while(query.next()){
                    R_assignment assignment1(query.value("date_release").toString(),
                                             query.value("ddl").toString(),
                                             query.value("title").toString(),
                                             query.value("releaser").toString(),
                                             query.value("sub_title").toString(),
                                             query.value("o_language").toString(),
                                             query.value("language").toString(),
                                             query.value("state").toInt(),
                                             query.value("No").toInt(),
                                             query.value("salary").toFloat(),
                                             query.value("translation").toString(),
                                             query.value("oringin").toString(),
                                             query.value("contractor").toString(),
                                             query.value("c_salary").toFloat(),
                                             query.value("info").toString(),
                                             query.value("c_ddl").toString(),
                                             query.value("reviewer").toString(),
                                             query.value("contractors").toString(),
                                             query.value("translators").toString(),
                                             query.value("t_ddl").toString(),
                                             query.value("translatorss").toString(),
                                             query.value("reviewers").toString(),
                                             query.value("re_evaluation").toString(),
                                             query.value("re_salary").toFloat(),
                                             query.value("c_evaluation").toString(),
                                             query.value("min").toInt(),
                                             query.value("max").toInt());
                    assignments.insert(query.value("No").toInt(),assignment1);
                }
            }
        }
    }
}
//输入主任务
void Database::ingressDataBase(QHash<int, R_assignment> & assignments)
{
    QSqlQuery query(s_database);
    QString query_str="delete from assignments";
    query.prepare(query_str);
    if(!query.exec()){//先执行删除表里的数据
        qDebug()<<"删除表里的assignments数据失败"<<query.lastError()<<endl;
    }
    else{//开始插入数据
        QHash<int,R_assignment>::iterator i;
        for(i=assignments.begin();i!=assignments.end();i++){
            query_str=QString("insert into assignments(No,date_release,ddl,title,"
                              "releaser,"
                              "sub_title,o_language,language,"
                              "state,salary,"
                              "translation,oringin,"
                              "contractor,c_salary,"
                              "info,c_ddl,reviewer,contractors,"
                              "t_ddl,translatorss,reviewers,"
                              "re_evaluation,re_salary,translators,"
                              "c_evaluation,min,max)"
                              "VALUES(%1,'%2','%3','%4','%5','%6','%7','%8',%9,%10,'%11',"
                              "'%12','%13',%14,'%15','%16','%17','%18','%19','%20','%21','%22',%23,'%24','%25','%26','%27')")
                    .arg(i->m_snopsis.m_No).arg(i->m_snopsis.m_date_release).arg(i->m_snopsis.m_ddl)
                    .arg(i->m_snopsis.m_title).arg(i->m_snopsis.m_releaser)
                    .arg(i->m_snopsis.m_sub_title)
                    .arg(i->m_snopsis.m_o_language)
                    .arg(i->m_snopsis.m_language).arg(i->m_snopsis.m_state).arg(static_cast<double>(i->m_snopsis.m_salary))
                    .arg(i->m_translation).arg(i->m_oringin).arg(i->m_contractor).arg(static_cast<double>(i->m_c_salary))
                    .arg(i->m_info).arg(i->m_c_ddl).arg(i->m_reviewer)
                    .arg(i->m_contractors).arg(i->m_t_ddl).arg(i->m_translatorss).arg(i->m_reviewers)
                    .arg(i->m_re_evaluation).arg(static_cast<double>(i->m_re_salary)).arg(i->m_translators)
                    .arg(i->m_c_evaluation).arg(i->m_t_salary_min).arg(i->m_t_salary_max);
            query.prepare(query_str);
            if(!query.exec()){
                qDebug()<<"插入assignment失败"<<query.lastError()<< endl;
                return;
            }
        }
    }
}
//从数据库里打开子任务
bool Database::openDataBase(T_assignment& sub_assignment,QString translator,int No)
{
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name='sub_assignments'");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt()==0){//该表不存在
            query_str="create table sub_assignments(t_No int primary key,"
                      "date_release QString,       ddl QString,"
                      "title QString,              sub_title QString,"
                      "o_language QString,         language QString,"
                      "state int,                  No int,                salary float,"
                      "releaser QString,           info QString,"
                      "t_translation QString,      t_oringin QString,"
                      "t_salary float,             translator QString,"
                      "c_evaluation QString,       r_evaluation QString)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建sub_assignment错误"<<query.lastError()<<endl;
            }
        }
        else{//表存在,开始导入数据
            query.prepare(QString("select * from sub_assignments where No = '%1' and translator = '%2'")
                          .arg(No).arg(translator));//向映射中导入数据
            if(!query.exec()){
                qDebug()<<"导入数据失败"<<query.lastError()<<endl;
                return false;
            }
            else {
                while(query.next()){
                    T_assignment sub(query.value("date_release").toString(),
                                     query.value("ddl").toString(),
                                     query.value("title").toString(),
                                     query.value("releaser").toString(),
                                     query.value("sub_title").toString(),
                                     query.value("o_language").toString(),
                                     query.value("language").toString(),
                                     query.value("state").toInt(),
                                     query.value("No").toInt(),
                                     query.value("salary").toFloat(),
                                     query.value("t_translation").toString(),
                                     query.value("t_oringin").toString(),
                                     query.value("t_salary").toFloat(),
                                     query.value("translator").toString(),
                                     query.value("info").toString(),
                                     query.value("c_evaluation").toString(),
                                     query.value("r_evaluation").toString(),
                                     query.value("t_No").toInt());
                    sub_assignment = sub;
                }
            }
        }
    }
    return true;
}
//插入子任务
bool Database::ingressDataBase(const T_assignment &subassignment)
{
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name='sub_assignments'");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt()==0){//该表不存在
            query_str="create table sub_assignments(t_No int primary key,"
                      "date_release QString,       ddl QString,"
                      "title QString,              sub_title QString,"
                      "o_language QString,         language QString,"
                      "state int,                  No int,                salary float,"
                      "releaser QString,           info QString,"
                      "t_translation QString,      t_oringin QString,"
                      "t_salary float,             translator QString,"
                      "c_evaluation QString,       r_evaluation QString)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建错误"<<query.lastError()<<endl;
            }
        }
        query_str= QString("delete from sub_assignments where t_No = %1").arg(subassignment.m_t_No);
        query.prepare(query_str);
        if(!query.exec()){//先执行删除表里的数据
            qDebug()<<"删除表里的sub_assignments数据失败"<<query.lastError()<<endl;
        }

        //开始插入数据
        query_str=QString("insert into sub_assignments"
                          "(t_No,date_release,ddl,"
                          "title,sub_title,"
                          "o_language,language,"
                          "state,No,salary,"
                          "releaser,info,"
                          "t_translation,t_oringin,"
                          "t_salary,translator,"
                          "c_evaluation,r_evaluation)"
                          "VALUES('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11',"
                          "'%12','%13','%14','%15','%16','%17','%18')")
                .arg(subassignment.m_t_No)
                .arg(subassignment.m_snopsis.m_date_release)
                .arg(subassignment.m_snopsis.m_ddl)
                .arg(subassignment.m_snopsis.m_title)
                .arg(subassignment.m_snopsis.m_sub_title)
                .arg(subassignment.m_snopsis.m_o_language)
                .arg(subassignment.m_snopsis.m_language)
                .arg(subassignment.m_snopsis.m_state)
                .arg(subassignment.m_snopsis.m_No)
                .arg(double(subassignment.m_snopsis.m_salary))
                .arg(subassignment.m_snopsis.m_releaser)
                .arg(subassignment.m_info)
                .arg(subassignment.m_translation)
                .arg(subassignment.m_oringin)
                .arg(double(subassignment.m_t_salary))
                .arg(subassignment.m_translator)
                .arg(subassignment.m_c_evaluation)
                .arg(subassignment.m_r_evaluation);
        query.prepare(query_str);
        if(!query.exec()){
            qDebug()<<"插入subassignment失败"<<query.lastError()<<endl;
            return false;
        }
    }
    return true;
}
//获取数据库中已有的子任务数量
int Database::num_of_T_assifnment()
{
    int num =0;
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name='sub_assignments'");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt()==0){//该表不存在
            query_str="create table sub_assignments(t_No int primary key,"
                      "date_release QString,       ddl QString,"
                      "title QString,              sub_title QString,"
                      "o_language QString,         language QString,"
                      "state int,                  No int,                salary float,"
                      "releaser QString,           info QString,"
                      "t_translation QString,      t_oringin QString,"
                      "t_salary float,             translator QString,"
                      "c_evaluation QString,       r_evaluation QString)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建sub_assignment错误"<<query.lastError()<<endl;
            }
        }
        else{//表存在,开始导入数据
            query.prepare(QString("select * from sub_assignments"));
            if(!query.exec()){
                qDebug()<<"导入数据失败"<<query.lastError()<<endl;
                return false;
            }
            else {
                while(query.next()){
                    num ++;
                }
            }
        }
    }
    return num;
}
//获取联系人列表
//void Database::openDataBase(QVector<QString>& contacts,QString target)
//{
//    qDebug()<<"已进入openDataBase(QVector<QString>& contacts)"<<endl;
//    QSqlQuery query(s_database);
//    QString query_str;
//    query_str=QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(target);
//    query.prepare(query_str);
//    qDebug()<<"已执行prepare语句"<<endl;
//    if(query.exec(query_str)){
//        //该表不存在
//        query.next();
//        qDebug()<<"已执行query.next"<<endl;
//        if(query.value(0).toInt()==0){
//            query_str=QString("create table '%1' (phonenum QString primary key)").arg(target);
//            qDebug()<<query_str<<endl;
//            query.prepare(query_str);//创建表
//            qDebug()<<"执行contacts创建"<<endl;
//            if(!query.exec()){
//                qDebug()<<"创建错误"<<query.lastError()<<endl;
//            }
//        }
//        else{//表存在,开始导入数据
//            query.prepare(QString("select * from '%1'").arg(target));//向映射中导入数据
//            qDebug()<<"没有创建contacts"<<endl;
//            if(!query.exec()){
//                qDebug()<<"导入contacts数据失败"<<query.lastError()<<endl;
//            }
//            else {
//                while(query.next()){
//                    contacts.push_back(query.value("phonenum").toString());
//                }
//            }
//        }
//    }
//}
//void Database::ingressDataBase(QVector<QString>& contacts,QString target)
//{
//    QSqlQuery query(s_database);
//    QString query_str=QString("delete from '%1'").arg(target);
//    query.prepare(query_str);
//    if(!query.exec()){//先执行删除表里的数据
//        qDebug()<<"删除contacts数据失败"<<query.lastError()<<endl;
//    }
//    else{//开始插入数据
//        QVector<QString>::iterator i;
//        qDebug()<<"开始插入contacts"<<endl;
//        for(i=contacts.begin();i!=contacts.end();i++){
//            query_str=QString("insert into '%1' (phonenum) VALUES('%2')").arg(target).arg(*i);
//            query.prepare(query_str);
//            if(!query.exec()){
//                qDebug()<<"插入contacts失败"<<endl;
//                return;
//            }

//        }
//        qDebug()<<"插入contacts完毕"<<endl;
//    }
//}
//读入数据库中有关自己的所有消息
void Database::openDataBase(QVector<message> &out_messages)
{
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name = 'messages'");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt()==0){//该表不存在
            query_str="create table messages(No int primary key,date QString,"
                      "receiver QString, text QString, sender QString,state int,title QString)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建错误"<<query.lastError()<<endl;
            }
        }
        else{//表存在,开始导入数据
            query.prepare(QString("select * from messages where receiver = '%1' OR sender = '%2' OR receiver ='admin'")
                          .arg(Widget::key).arg(Widget::key));//向映射中导入数据
            if(!query.exec()){
                qDebug()<<"导入messages数据失败"<<query.lastError()<<endl;
            }
            else {
                while(query.next()){
                    message message0(query.value("sender").toString(),
                                     query.value("receiver").toString(),
                                     query.value("text").toString(),query.value("date").toString(),query.value("No").toInt(),
                                     query.value("state").toInt(),query.value("title").toString());
                    out_messages.push_back(message0);
                }
            }
        }
    }
}
//向数据库中插入要发送的消息
void Database::ingressDataBase(QVector<message> &in_messages)
{
    QSqlQuery query(s_database);
    QString query_str;
    QVector<message>::iterator i;
    for(i=in_messages.begin();i!=in_messages.end();i++){
        query_str=QString("insert into messages(sender,receiver,text,date,state,title,No) "
                          "VALUES('%1','%2','%3','%4','%5','%6','%7')")
                .arg(i->m_sender).arg(i->m_receiver).arg(i->m_text)
                .arg(i->m_datetime).arg(int(i->m_state)).arg(i->m_title).arg(i->m_No);
        query.prepare(query_str);
        if(!query.exec()){
            qDebug()<<"插入失败"<<query.lastError()<<endl;
            return;
        }
    }
}
//获取数据库中已有的信息数量
int Database::num_of_messages()
{
    int num = 0;
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name = 'messages'");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt()==0){//该表不存在
            query_str="create table messages(No int primary key,date QString,"
                      "receiver QString, text QString, sender QString,state int,title QString)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建错误"<<query.lastError()<<endl;
            }
        }
        else{//表存在,开始导入数据
            query.prepare(QString("select * from messages"));//向映射中导入数据
            if(!query.exec()){
                qDebug()<<"导入messages数据失败"<<query.lastError()<<endl;
            }
            else {
                while(query.next()){
                    num ++;
                }
            }
        }
    }
    return num;
}
//删除消息
bool Database::delete_messages(message& d_message)
{
    QSqlQuery query(Database::s_database);
    QString query_str;
    query_str = QString("delete from messages where No = '%1'")
            .arg(d_message.get_No());
    query.prepare(query_str);
    if(!query.exec(query_str))
    {
        qDebug()<<"删除消息失败"<<query.lastError()<<endl;
        return false;
    }
    return true;
}
//读入验证
void Database::openDataBase(QVector<Validation> &validation)
{
    QSqlQuery query(s_database);
    QString query_str;
    query_str=QString("select count(*) from sqlite_master where type='table' and name= 'validations'");
    query.prepare(query_str);
    if(query.exec(query_str)){
        query.next();
        if(query.value(0).toInt()==0){
            query_str="create table validations (date QString primary key, sender QString,"
                      "receiver QString,state int)";
            query.prepare(query_str);//创建表
            if(!query.exec()){
                qDebug()<<"创建错误"<<query.lastError()<<endl;
            }
        }
        else{//表存在,开始导入数据
            query.prepare("select * from validations");
            if(!query.exec()){
                qDebug()<<"导入数据validation失败"<<query.lastError()<<endl;
            }
            else {
                qDebug()<<"123"<<endl;
                while(query.next()){
                    qDebug()<<"akhjs"<<endl;
                    Validation v(query.value("date").toString(), query.value("sender").toString(),
                                 query.value("receiver").toString(),query.value("state").toInt());
                    validation.push_back(v);
                }
            }
            qDebug()<<validation.size();
        }
    }
}
//输出验证
void Database::ingressDataBase(QVector<Validation> &validation)
{
    QSqlQuery query(s_database);
    QString query_str=QString("delete from validations");
    query.prepare(query_str);
    if(!query.exec()){
        qDebug()<<"删除表里的validation数据失败"<<query.lastError()<<endl;
    }
    else{//开始插入数据
        QVector<Validation>::iterator i;
        for(i=validation.begin();i!=validation.end();i++){
            query_str=QString("insert into validations(date,sender,receiver,state)"
                              "VALUES('%1','%2','%3','%4')").arg(i->m_date.unicode())
                    .arg(i->m_sender.unicode()).arg(i->m_receiver.unicode()).arg((int)i->m_state);
            query.prepare(query_str);
            if(!query.exec()){
                qDebug()<<"插入validation失败" << query.lastError() << endl;
                return;
            }
        }
    }
}
