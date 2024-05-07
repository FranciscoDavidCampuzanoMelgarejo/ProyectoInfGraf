#include "convertir_color_falso.h"
#include "ui_convertir_color_falso.h"

convertir_color_falso::convertir_color_falso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::convertir_color_falso)
{
    ui->setupUi(this);
}

convertir_color_falso::~convertir_color_falso()
{
    delete ui;
}
