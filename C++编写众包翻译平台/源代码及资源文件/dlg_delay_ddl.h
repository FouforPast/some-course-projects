#ifndef DLG_DELAY_DDL_H
#define DLG_DELAY_DDL_H
#include <QDialog>
#include<QButtonGroup>
class R_assignment;
namespace Ui {
class dlg_delay_ddl;
}

class dlg_delay_ddl : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_delay_ddl(R_assignment* r,int role,QWidget *parent = nullptr);
    ~dlg_delay_ddl();
    int sel;
    QButtonGroup *g_;

private slots:
    void on_pushButton_clicked();

private:
    Ui::dlg_delay_ddl *ui;
    int role;
    R_assignment* r;
};

#endif // DLG_DELAY_DDL_H
