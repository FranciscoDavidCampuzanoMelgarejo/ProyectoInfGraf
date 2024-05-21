#include "aplicar_morfologia.h"
#include "ui_aplicar_morfologia.h"
#include "imagenes.h"

aplicar_morfologia::aplicar_morfologia(int num_foto, int num_res, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aplicar_morfologia)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    this->nres = num_res;
    this->imagen = foto[nfoto].img.clone();
    this->operacion = MORPH_DILATE;
    this->tam = 0;
    this->iteraciones = 1;

    set_callback_foto(nfoto, false);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);


}

aplicar_morfologia::~aplicar_morfologia()
{
    set_callback_foto(nfoto, true);
    destroyWindow("Morfologia");
    delete ui;
}

void aplicar_morfologia::on_radioDilatacion_clicked()
{
    this->operacion = MORPH_DILATE;
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);
}

void aplicar_morfologia::on_radioErosion_clicked()
{
    this->operacion = MORPH_ERODE;
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);
}

void aplicar_morfologia::on_radioApertura_clicked()
{
    this->operacion = MORPH_OPEN;
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);
}

void aplicar_morfologia::on_radioCierre_clicked()
{
    this->operacion = MORPH_CLOSE;
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);
}

void aplicar_morfologia::on_horizontalSlider_valueChanged(int value)
{
    this->tam = value;
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);
}

void aplicar_morfologia::on_spinBox_valueChanged(int arg1)
{
    this->iteraciones = arg1;
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);
}

void aplicar_morfologia::on_pushButton_clicked()
{
    Mat salida = aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones);
    salida.copyTo(this->imagen);
}

void aplicar_morfologia::on_aplicar_morfologia_rejected()
{
    mostrar(this->nfoto);
}

void aplicar_morfologia::on_aplicar_morfologia_accepted()
{
    aplicarMorfologia(this->imagen, this->nres, this->operacion, this->tam, this->iteraciones, true);
}
