#include "rojo_verde_azul.h"
#include "ui_rojo_verde_azul.h"
#include "imagenes.h"

rojo_verde_azul::rojo_verde_azul(int num_foto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rojo_verde_azul)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    set_callback_foto(nfoto, false);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);
}

rojo_verde_azul::~rojo_verde_azul()
{
    set_callback_foto(nfoto, true);
    delete ui;
}

void rojo_verde_azul::actualizarCanal(int canal, int valor) {
    this->sumaCanales[canal] = valor;
    ajustar_rojo_verde_azul(this->nfoto, this->productoCanales, this->sumaCanales, 3);
}

void rojo_verde_azul::actualizarCanal(int canal, double valor) {
    this->productoCanales[canal] = valor;
    ajustar_rojo_verde_azul(this->nfoto, this->productoCanales, this->sumaCanales, 3);
}


void rojo_verde_azul::on_rojoSliderSuma_valueChanged(int value)
{
    ui->rojoSpinSuma->setValue(value);
    actualizarCanal(CANAL_ROJO, value);
}

void rojo_verde_azul::on_rojoSpinSuma_valueChanged(int arg1)
{
    ui->rojoSliderSuma->setValue(arg1);
    actualizarCanal(CANAL_ROJO, arg1);
}

void rojo_verde_azul::on_rojoSliderMult_valueChanged(int value)
{
    ui->rojoSpinMult->setValue(static_cast<double>(value));
    actualizarCanal(CANAL_ROJO, static_cast<double>(value));
}

void rojo_verde_azul::on_rojoSpinMult_valueChanged(double arg1)
{
    ui->rojoSliderMult->setValue(static_cast<int>(arg1));
    actualizarCanal(CANAL_ROJO, arg1);
}



void rojo_verde_azul::on_verdeSliderSuma_valueChanged(int value)
{
    ui->verdeSpinSuma->setValue(value);
    actualizarCanal(CANAL_VERDE, value);
}



void rojo_verde_azul::on_verdeSpinSuma_valueChanged(int arg1)
{
    ui->verdeSliderSuma->setValue(arg1);
    actualizarCanal(CANAL_VERDE, arg1);
}

void rojo_verde_azul::on_verdeSliderMult_valueChanged(int value)
{
    ui->verdeSpinMult->setValue(static_cast<double>(value));
    actualizarCanal(CANAL_VERDE, static_cast<double>(value));
}

void rojo_verde_azul::on_verdeSpinMult_valueChanged(double arg1)
{
    ui->verdeSliderMult->setValue(static_cast<int>(arg1));
    actualizarCanal(CANAL_VERDE, arg1);
}

void rojo_verde_azul::on_azulSliderSuma_valueChanged(int value)
{
    ui->azulSpinSuma->setValue(value);
    actualizarCanal(CANAL_AZUL, value);
}

void rojo_verde_azul::on_azulSpinSuma_valueChanged(int arg1)
{
    ui->azulSliderSuma->setValue(arg1);
    actualizarCanal(CANAL_AZUL, arg1);
}

void rojo_verde_azul::on_azulSliderMult_valueChanged(int value)
{
    ui->azulSpinMult->setValue(static_cast<double>(value));
    actualizarCanal(CANAL_AZUL, static_cast<double>(value));
}

void rojo_verde_azul::on_azulSpinMult_valueChanged(double arg1)
{
    ui->azulSliderMult->setValue(static_cast<int>(arg1));
    actualizarCanal(CANAL_AZUL, arg1);
}

void rojo_verde_azul::on_rojo_verde_azul_accepted()
{
    ajustar_rojo_verde_azul(this->nfoto, this->productoCanales, this->sumaCanales, 3, true);
}

void rojo_verde_azul::on_rojo_verde_azul_rejected()
{
    mostrar(this->nfoto);
}
