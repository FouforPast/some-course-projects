#ifndef MESSAGE_UI_H
#define MESSAGE_UI_H
#include<QListWidget>
#include<QString>
#include"user.h"
#include"widget.h"
#include <QDialog>

namespace Ui {
class message_ui;
}

class message_ui : public QDialog
{
    Q_OBJECT

public:
    explicit message_ui(QWidget *parent = nullptr);
    User* m_target;
    QString m_all_message;
    int m_num;
    ~message_ui();

private slots:
    void on_btn_emit_clicked();

    void on_btn_delete_clicked();

    void on_btn_add_clicked();

    void on_listWidget_contacts_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_contacts_itemClicked(QListWidgetItem *item);

    void on_btn_apply_clicked();

private:
    Ui::message_ui *ui;
};

#endif // MESSAGE_UI_H
