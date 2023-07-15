#ifndef ASSIGNMENT_UI_H
#define ASSIGNMENT_UI_H
#include"realease_task.h"
#include <QMainWindow>
#include<QTableWidget>
#include<QTableWidgetItem>
namespace Ui {
class assignment_ui;
}

class assignment_ui : public QMainWindow
{
    Q_OBJECT

public:
    explicit assignment_ui(QWidget *parent = nullptr);
    ~assignment_ui();

private slots:
    void on_btn_001_clicked();

    void on_btn_002_clicked();

    void on_bt_003_clicked();

    void on_btn_004_clicked();

    void on_btn_s_contractor_clicked();

    void on_btn_s_interpreter_clicked();

    void on_pushButton_r_clicked();

    void on_pushButton_detail1_clicked();

    void on_pushButton_detail2_clicked();

    void on_btn_joinin_clicked();

    void on_btn_sign_up_reviewer_clicked();

    void on_pushButton_sort_clicked();

    void on_pushButton_sort_2_clicked();

    void on_tableWidget_0_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::assignment_ui *ui;
};

#endif // ASSIGNMENT_UI_H
