#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H
#include"message.h"
#include <QDialog>

namespace Ui {
class administrator;
}

class administrator : public QDialog
{
    Q_OBJECT

public:
    explicit administrator(QWidget *parent = nullptr);
    ~administrator();
    QVector<message> h_me;
    int num;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::administrator *ui;
};

#endif // ADMINISTRATOR_H
