#ifndef DIALOGO_ECUALIZACION_LOCAL_H
#define DIALOGO_ECUALIZACION_LOCAL_H

#include <QDialog>

namespace Ui {
class dialogo_ecualizacion_local;
}

class dialogo_ecualizacion_local : public QDialog
{
    Q_OBJECT

public:
    explicit dialogo_ecualizacion_local(int num_foto, int num_res, QWidget *parent = nullptr);
    ~dialogo_ecualizacion_local();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_clicked();

    void on_dialogo_ecualizacion_local_rejected();

    void on_dialogo_ecualizacion_local_accepted();

private:
    Ui::dialogo_ecualizacion_local *ui;

    int nfoto, nres;
    bool ec_estandar; // TRUE -> Ec. Estandar; FALSE -> Ec. CLAHE
};

#endif // DIALOGO_ECUALIZACION_LOCAL_H
