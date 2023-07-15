#ifndef DLG_UPDATE_H
#define DLG_UPDATE_H
#include"assignment.h"
#include <QDialog>

namespace Ui {
class dlg_update;
}

class dlg_update : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_update(R_assignment* r, QWidget *parent = nullptr);
    ~dlg_update();

private slots:
    void on_pushButton_clicked();

private:
    R_assignment* r;
    Ui::dlg_update *ui;
};

#endif // DLG_UPDATE_H
