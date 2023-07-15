#ifndef ADMIN_VIEW_CER_H
#define ADMIN_VIEW_CER_H
#include"widget.h"
#include"message.h"
#include"user.h"
#include <QDialog>

namespace Ui {
class admin_view_cer;
}

class admin_view_cer : public QDialog
{
    Q_OBJECT

public:
    explicit admin_view_cer(const message& me, QWidget *parent = nullptr);
    ~admin_view_cer();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    message mess;
    Ui::admin_view_cer *ui;
};

#endif // ADMIN_VIEW_CER_H
