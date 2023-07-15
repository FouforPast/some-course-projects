#ifndef DLG_C_VIEW_H
#define DLG_C_VIEW_H
#include <QDialog>
class R_assignment;
namespace Ui {
class dlg_c_view;
}

class dlg_c_view : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_c_view(int cate,R_assignment* r1,int role ,QWidget *parent = nullptr);
    ~dlg_c_view();

private slots:

    void on_pushButton_emit_clicked();

    void on_comboBox_translator_currentIndexChanged(const QString &arg1);

    void on_pushButton_submit_clicked();

private:
    int cate;
    R_assignment* r1;
    int role;
    Ui::dlg_c_view *ui;
};

#endif // DLG_C_VIEW_H
