#include "bajorrelieve.h"
#include "ui_bajorrelieve.h"
#include "imagenes.h"

bajorrelieve::bajorrelieve(int numfoto, int numres, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bajorrelieve)
{
    ui->setupUi(this);
    nfoto = numfoto;
    nres = numres;
    angulo = 0;
    grado = 1;
    nfondo = 3;

    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo);
}

bajorrelieve::~bajorrelieve()
{
    delete ui;
}

void bajorrelieve::on_horizontalSlider_valueChanged(int value)
{
    grado = value/100.0;
    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo);
}

void bajorrelieve::on_dial_valueChanged(int value)
{
    angulo = value;
    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo);
}

void bajorrelieve::on_radioButton_clicked()
{
    nfondo = 0;
    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo);
}

void bajorrelieve::on_radioButton_2_clicked()
{
    nfondo = 1;
    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo);
}

void bajorrelieve::on_radioButton_3_clicked()
{
    nfondo = 2;
    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo);
}

void bajorrelieve::on_radioButton_4_clicked()
{
    nfondo = 3;
    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo);
}

void bajorrelieve::on_bajorrelieve_accepted()
{
    ver_bajorrelieve(nfoto, nres, angulo, grado, nfondo, true);
    destroyWindow("Bajorrelieve");
}

void bajorrelieve::on_bajorrelieve_rejected()
{
    destroyWindow("Bajorrelieve");
}
