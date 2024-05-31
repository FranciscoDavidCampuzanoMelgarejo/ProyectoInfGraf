#include "rotar_reescalar.h"
#include "ui_rotar_reescalar.h"
#include "imagenes.h"

rotar_reescalar::rotar_reescalar(int num_foto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rotar_reescalar)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    set_callback_foto(nfoto, false);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);
}

rotar_reescalar::~rotar_reescalar()
{
    set_callback_foto(nfoto, true);
    delete ui;
}

int rotar_reescalar::ajustarAngulo(int angulo) {
    int angulo_ajustado = 180 - angulo;
    if(angulo_ajustado < 0)
        angulo_ajustado += 360;
    return angulo_ajustado;
}

void rotar_reescalar::on_dial_valueChanged(int value)
{
    int angulo = ajustarAngulo(value);
    rotar_y_reescalar(this->nfoto, static_cast<double>(angulo), ui->doubleSpinBox->value());
}

void rotar_reescalar::on_doubleSpinBox_valueChanged(double arg1)
{
    int angulo = ajustarAngulo(ui->dial->value());
    rotar_y_reescalar(this->nfoto, static_cast<double>(angulo), arg1);
}

void rotar_reescalar::on_rotar_reescalar_rejected()
{
    mostrar(nfoto);
}

void rotar_reescalar::on_rotar_reescalar_accepted()
{
    int angulo = ajustarAngulo(ui->dial->value());
    rotar_y_reescalar(nfoto, static_cast<double>(angulo), ui->doubleSpinBox->value(), true);
}
