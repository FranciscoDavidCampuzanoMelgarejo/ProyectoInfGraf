#ifndef ROJO_VERDE_AZUL_H
#define ROJO_VERDE_AZUL_H

#include <QDialog>

namespace Ui {
class rojo_verde_azul;
}

class rojo_verde_azul : public QDialog
{
    Q_OBJECT

public:
    explicit rojo_verde_azul(int num_foto, QWidget *parent = nullptr);
    ~rojo_verde_azul();

private slots:
    void on_rojoSliderSuma_valueChanged(int value);

    void on_rojoSpinSuma_valueChanged(int arg1);

    void on_rojoSliderMult_valueChanged(int value);

    void on_rojoSpinMult_valueChanged(double arg1);

    void on_verdeSliderSuma_valueChanged(int value);

    void on_verdeSliderMult_valueChanged(int value);

    void on_verdeSpinSuma_valueChanged(int arg1);

    void on_verdeSpinMult_valueChanged(double arg1);

    void on_azulSliderSuma_valueChanged(int value);

    void on_azulSpinSuma_valueChanged(int arg1);

    void on_azulSliderMult_valueChanged(int value);

    void on_azulSpinMult_valueChanged(double arg1);

    void on_rojo_verde_azul_accepted();

    void on_rojo_verde_azul_rejected();

private:
    Ui::rojo_verde_azul *ui;

    int nfoto;
    int sumaCanales[3] = {0, 0, 0}; //BGR
    double productoCanales[3] = {1.0, 1.0, 1.0}; //BGR

    // CONSTANTES
    const int CANAL_ROJO = 2;
    const int CANAL_VERDE = 1;
    const int CANAL_AZUL = 0;

    void actualizarCanal(int canal, int valor);
    void actualizarCanal(int canal, double valor);
};

#endif // ROJO_VERDE_AZUL_H
