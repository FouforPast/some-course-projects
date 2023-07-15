#ifndef REALEASE_TASK_H
#define REALEASE_TASK_H

#include <QDialog>

namespace Ui {
class realease_task;
}

class realease_task : public QDialog
{
    Q_OBJECT

public:
    explicit realease_task(QWidget *parent = nullptr);
    ~realease_task();

private slots:
    void on_pushButton_clicked();

private:
    Ui::realease_task *ui;
};

#endif // REALEASE_TASK_H
