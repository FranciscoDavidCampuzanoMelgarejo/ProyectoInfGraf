#include "dialogo_ecualizacion_local.h"
#include "ui_dialogo_ecualizacion_local.h"
#include "imagenes.h"

dialogo_ecualizacion_local::dialogo_ecualizacion_local(int num_foto, int num_res, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogo_ecualizacion_local)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    this->nres = num_res;
    this->ec_estandar = true;

    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);

    int valor_maximo_spinBox = std::min(foto[nfoto].img.cols, foto[nfoto].img.rows)/2;
    this->ui->spinBox->setMaximum(valor_maximo_spinBox);
    ecualizar_histograma_local(nfoto, nres, this->ui->spinBox->value());

}

dialogo_ecualizacion_local::~dialogo_ecualizacion_local()
{
    destroyWindow("Ecualizacion Local");
    delete ui;
}

void dialogo_ecualizacion_local::on_radioButton_clicked()
{
    if(!this->ec_estandar) {
        this->ec_estandar = true;
        ecualizar_histograma_local(nfoto, nres, this->ui->spinBox->value());
    }
}

void dialogo_ecualizacion_local::on_radioButton_2_clicked()
{
    if(this->ec_estandar) {
        this->ec_estandar = false;
        ecualizar_histograma_local_CLAHE(nfoto, nres, this->ui->spinBox->value());
    }
}

void dialogo_ecualizacion_local::on_pushButton_clicked()
{
    if(this->ec_estandar)
        ecualizar_histograma_local(nfoto, nres, this->ui->spinBox->value());
    else
        ecualizar_histograma_local_CLAHE(nfoto, nres, this->ui->spinBox->value());
}

void dialogo_ecualizacion_local::on_dialogo_ecualizacion_local_rejected()
{
    mostrar(nfoto);
}

void dialogo_ecualizacion_local::on_dialogo_ecualizacion_local_accepted()
{
    if(this->ec_estandar)
        ecualizar_histograma_local(nfoto, nres, this->ui->spinBox->value(), true);
    else
        ecualizar_histograma_local_CLAHE(nfoto, nres, this->ui->spinBox->value(), true);
}
