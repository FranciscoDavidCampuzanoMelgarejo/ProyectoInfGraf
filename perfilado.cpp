#include "perfilado.h"
#include "ui_perfilado.h"
#include "imagenes.h"

Perfilado::Perfilado(int num_foto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Perfilado)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    this->radio = 1;
    this->porcentaje = 0.0;

    set_callback_foto(nfoto, false);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);
}

Perfilado::~Perfilado()
{
    set_callback_foto(nfoto, true);
    delete ui;
}

void Perfilado::on_horizontalSlider_valueChanged(int value)
{
    radio = (value % 2 == 0) ? value + 1 : value;
    ver_perfilado(nfoto, radio, porcentaje);
}

void Perfilado::on_horizontalSlider_2_valueChanged(int value)
{
    porcentaje = value / 100.0;
    ver_perfilado(nfoto, radio, porcentaje);
}

void Perfilado::on_Perfilado_rejected()
{
    mostrar(nfoto);
}

void Perfilado::on_Perfilado_accepted()
{
    ver_perfilado(nfoto, radio, porcentaje, true);
}
