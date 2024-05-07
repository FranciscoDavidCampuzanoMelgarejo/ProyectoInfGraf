#include "convertir_color_falso.h"
#include "ui_convertir_color_falso.h"
#include "imagenes.h"

convertir_color_falso::convertir_color_falso(int num_foto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::convertir_color_falso)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    this->paleta = -1;
    this->labelAnterior = nullptr;

    set_callback_foto(nfoto, false);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);

}

convertir_color_falso::~convertir_color_falso()
{
    delete ui;
}

void convertir_color_falso::convertir() {
    convertir_a_falso(this->nfoto, this->paleta);
}

void convertir_color_falso::resaltarLabel(QLabel *label) {
    if(this->labelAnterior != nullptr)
        this->labelAnterior->setStyleSheet("");
    label->setStyleSheet("color:blue;");
    this->labelAnterior = label;
}

void convertir_color_falso::on_pushButton_clicked()
{
    this->paleta = COLORMAP_AUTUMN;
    resaltarLabel(this->ui->lblAutumn);
    convertir();
}

void convertir_color_falso::on_pushButton_2_clicked()
{
    this->paleta = COLORMAP_BONE;
    resaltarLabel(ui->lblBone);
    convertir();
}

void convertir_color_falso::on_pushButton_3_clicked()
{
    this->paleta = COLORMAP_JET;
    resaltarLabel(ui->lblJet);
    convertir();
}

void convertir_color_falso::on_pushButton_4_clicked()
{
    this->paleta = COLORMAP_WINTER;
    resaltarLabel(ui->lblWinter);
    convertir();
}

void convertir_color_falso::on_pushButton_5_clicked()
{
    this->paleta = COLORMAP_RAINBOW;
    resaltarLabel(ui->lblRainbow);
    convertir();
}

void convertir_color_falso::on_pushButton_6_clicked()
{
    this->paleta = COLORMAP_OCEAN;
    resaltarLabel(ui->lblOcean);
    convertir();
}

void convertir_color_falso::on_pushButton_7_clicked()
{
    this->paleta = COLORMAP_SUMMER;
    resaltarLabel(ui->lblSummer);
    convertir();
}

void convertir_color_falso::on_pushButton_8_clicked()
{
    this->paleta = COLORMAP_SPRING;
    resaltarLabel(ui->lblSpring);
    convertir();
}

void convertir_color_falso::on_pushButton_9_clicked()
{
    this->paleta = COLORMAP_COOL;
    resaltarLabel(ui->lblCool);
    convertir();
}

void convertir_color_falso::on_pushButton_10_clicked()
{
    this->paleta = COLORMAP_HSV;
    resaltarLabel(ui->lblHSV);
    convertir();
}

void convertir_color_falso::on_pushButton_11_clicked()
{
    this->paleta = COLORMAP_PINK;
    resaltarLabel(ui->lblPink);
    convertir();
}

void convertir_color_falso::on_pushButton_12_clicked()
{
    this->paleta = COLORMAP_HOT;
    resaltarLabel(ui->lblHot);
    convertir();
}

void convertir_color_falso::on_pushButton_13_clicked()
{
    this->paleta = COLORMAP_PARULA;
    resaltarLabel(ui->lblParula);
    convertir();
}

void convertir_color_falso::on_pushButton_14_clicked()
{
    this->paleta = COLORMAP_MAGMA;
    resaltarLabel(ui->lblMagma);
    convertir();
}

void convertir_color_falso::on_pushButton_15_clicked()
{
    this->paleta = COLORMAP_INFERNO;
    resaltarLabel(ui->lblInferno);
    convertir();
}

void convertir_color_falso::on_pushButton_16_clicked()
{
    this->paleta = COLORMAP_PLASMA;
    resaltarLabel(ui->lblPlasma);
    convertir();
}

void convertir_color_falso::on_pushButton_17_clicked()
{
    this->paleta = COLORMAP_VIRIDIS;
    resaltarLabel(ui->lblViridis);
    convertir();
}

void convertir_color_falso::on_pushButton_18_clicked()
{
    this->paleta = COLORMAP_CIVIDIS;
    resaltarLabel(ui->lblCividis);
    convertir();
}

void convertir_color_falso::on_pushButton_19_clicked()
{
    this->paleta = COLORMAP_TWILIGHT;
    resaltarLabel(ui->lblTwilight);
    convertir();
}

void convertir_color_falso::on_pushButton_20_clicked()
{
    this->paleta = COLORMAP_TWILIGHT_SHIFTED;
    resaltarLabel(ui->lblTwilightShifted);
    convertir();
}

void convertir_color_falso::on_pushButton_21_clicked()
{
    this->paleta = COLORMAP_TURBO;
    resaltarLabel(ui->lblTurbo);
    convertir();
}

void convertir_color_falso::on_pushButton_22_clicked()
{
    this->paleta = COLORMAP_DEEPGREEN;
    resaltarLabel(ui->lblDeepgreen);
    convertir();
}

void convertir_color_falso::on_convertir_color_falso_rejected()
{
    mostrar(nfoto);
}

void convertir_color_falso::on_convertir_color_falso_accepted()
{
    if(this->paleta != -1) {
        convertir_a_falso(this->nfoto, this->paleta, true);
    } else
        mostrar(nfoto);
}
