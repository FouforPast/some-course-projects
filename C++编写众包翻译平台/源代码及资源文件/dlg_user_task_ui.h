#ifndef DLG_USER_TASK_UI_H
#define DLG_USER_TASK_UI_H
#include"user_ui_task.h"
#include <QDialog>

namespace Ui {
class dlg_user_task_ui;
}

class dlg_user_task_ui : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_user_task_ui(int _role,int _z_No,QWidget *parent = nullptr);
    user_ui_task* dlg;
    ~dlg_user_task_ui();

private:
    Ui::dlg_user_task_ui *ui;
    int role;
    int z_No;
};

#endif // DLG_USER_TASK_UI_H
