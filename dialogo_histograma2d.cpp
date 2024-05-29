#include "dialogo_histograma2d.h"
#include "ui_dialogo_histograma2d.h"

dialogo_histograma2D::dialogo_histograma2D(int num_foto, int num_res, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogo_histograma2D)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    this->nres = num_res;
    this->canales = R_G;

    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);

    ver_histograma2D(nfoto, nres, this->ui->spinBox->value(), canales);
}

dialogo_histograma2D::~dialogo_histograma2D()
{
    destroyWindow("Histograma_2D");
    delete ui;
}

void dialogo_histograma2D::on_radioButton_clicked()
{
    canales_histograma2D nuevosCanales = R_G;
    if(nuevosCanales != canales) {
        this->canales = nuevosCanales;
        ver_histograma2D(nfoto, nres, this->ui->spinBox->value(), canales);
    }
}

void dialogo_histograma2D::on_radioButton_2_clicked()
{
    canales_histograma2D nuevosCanales = R_B;
    if(nuevosCanales != canales) {
        this->canales = nuevosCanales;
        ver_histograma2D(nfoto, nres, this->ui->spinBox->value(), canales);
    }
}

void dialogo_histograma2D::on_radioButton_3_clicked()
{
    canales_histograma2D nuevosCanales = G_B;
    if(nuevosCanales != canales) {
        this->canales = nuevosCanales;
        ver_histograma2D(nfoto, nres, this->ui->spinBox->value(), canales);
    }
}

void dialogo_histograma2D::on_pushButton_clicked()
{
    ver_histograma2D(nfoto, nres, this->ui->spinBox->value(), canales);
}

void dialogo_histograma2D::on_dialogo_histograma2D_rejected()
{
    mostrar(nfoto);
}

void dialogo_histograma2D::on_dialogo_histograma2D_accepted()
{
    ver_histograma2D(nfoto, nres, this->ui->spinBox->value(), canales, true);
}
