#ifndef DLG_SYSTEM_MESSAGE_H
#define DLG_SYSTEM_MESSAGE_H
#include"message.h"
#include <QDialog>
#include<QTableWidget>
#include<QTableWidgetItem>
#include<QTextBrowser>
#include<database.h>
namespace Ui {
class dlg_system_message;
}

class dlg_system_message : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_system_message(QWidget *parent = nullptr);
    ~dlg_system_message();

private slots:
    void on_tableWidget_contents_itemDoubleClicked(QTableWidgetItem *item);

    void on_btn_delete_clicked();

private:
    Ui::dlg_system_message *ui;
};

#endif // DLG_SYSTEM_MESSAGE_H
