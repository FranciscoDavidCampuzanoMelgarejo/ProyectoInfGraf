#ifndef APLICAR_MORFOLOGIA_H
#define APLICAR_MORFOLOGIA_H

#include <QDialog>
#include "opencv2/opencv.hpp"

using namespace cv;

namespace Ui {
class aplicar_morfologia;
}

class aplicar_morfologia : public QDialog
{
    Q_OBJECT

public:
    explicit aplicar_morfologia(int num_foto, int num_res, QWidget *parent = nullptr);
    ~aplicar_morfologia();

private slots:
    void on_radioDilatacion_clicked();

    void on_radioErosion_clicked();

    void on_radioApertura_clicked();

    void on_radioCierre_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_aplicar_morfologia_rejected();

    void on_aplicar_morfologia_accepted();

private:
    Ui::aplicar_morfologia *ui;

    int nfoto;
    int nres;
    Mat imagen;
    int operacion;

    int tam, iteraciones;

};

#endif // APLICAR_MORFOLOGIA_H
