#ifndef DLG_ALLCO_C_H
#define DLG_ALLCO_C_H
#include <QDialog>
class R_assignment;
namespace Ui {
class dlg_allco_c;
}

class dlg_allco_c : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_allco_c(R_assignment* r, QWidget *parent = nullptr);
    ~dlg_allco_c();
    static int num;
private slots:
    void on_pushButton_allco_clicked();

    void on_pushButton_detail_clicked();

private:
    R_assignment* r;
    Ui::dlg_allco_c *ui;
};

#endif // DLG_ALLCO_C_H
