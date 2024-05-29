#ifndef DIALOGO_HISTOGRAMA2D_H
#define DIALOGO_HISTOGRAMA2D_H

#include "imagenes.h"
#include <QDialog>

namespace Ui {
class dialogo_histograma2D;
}

class dialogo_histograma2D : public QDialog
{
    Q_OBJECT

public:
    explicit dialogo_histograma2D(int num_foto, int num_res, QWidget *parent = nullptr);
    ~dialogo_histograma2D();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_pushButton_clicked();

    void on_dialogo_histograma2D_rejected();

    void on_dialogo_histograma2D_accepted();

private:
    Ui::dialogo_histograma2D *ui;

    int nfoto, nres;
    canales_histograma2D canales;
};

#endif // DIALOGO_HISTOGRAMA2D_H
