#include "convolucion.h"
#include "ui_convolucion.h"

convolucion::convolucion(int numfoto, int numres, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::convolucion)
{
    ui->setupUi(this);
    nfoto = numfoto;
    nres = numres;
    M.create(5, 5, CV_64FC1);
    actualizar();
}

convolucion::~convolucion()
{
    delete ui;
}

void convolucion::actualizar() {
    suma = ui->doubleSpinBox_2->value();
    mult = ui->doubleSpinBox->value();

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            double val = ui->tableWidget->item(i, j)->text().toDouble();
            M.at<double>(i, j) = val;
            ui->tableWidget->item(i, j)->setText(QString::number(val));
        }
    }
}

void convolucion::on_pushButton_clicked()
{
    actualizar();
    ver_convolucion(nfoto, nres, M, mult, suma);
}

void convolucion::on_convolucion_accepted()
{
    actualizar();
    ver_convolucion(nfoto, nres, M, mult, suma, true);
    destroyWindow("Convolucion");
}

void convolucion::on_convolucion_rejected()
{
    destroyWindow("Convolucion");
}

void convolucion::on_pushButton_2_clicked()
{
    double T[5][5] = {
        {0, 0, 0, 0, 0},
        {0, -1, 0, 1, 0},
        {0, -1, 0, 1, 0},
        {0, -1, 0, 1, 0},
        {0, 0, 0, 0, 0},
    };
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            ui->tableWidget->item(i, j)->setText(QString::number(T[i][j]));
        }
    }
    ui->doubleSpinBox->setValue(1);
    ui->doubleSpinBox_2->setValue(128);
}

void convolucion::on_pushButton_3_clicked()
{
    double T[5][5] = {
        {0, 0, 0, 0, 0},
        {0, -1, -1, -1, 0},
        {0, -1, 8, -1, 0},
        {0, -1, -1, -1, 0},
        {0, 0, 0, 0, 0},
    };
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            ui->tableWidget->item(i, j)->setText(QString::number(T[i][j]));
        }
    }
    ui->doubleSpinBox->setValue(1);
    ui->doubleSpinBox_2->setValue(128);
}

void convolucion::on_pushButton_4_clicked()
{
    double T[5][5] = {
        {0, 0, 0, 0, 0},
        {0, -1, -1, -1, 0},
        {0, -1, 9, -1, 0},
        {0, -1, -1, -1, 0},
        {0, 0, 0, 0, 0},
    };
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            ui->tableWidget->item(i, j)->setText(QString::number(T[i][j]));
        }
    }
    ui->doubleSpinBox->setValue(1);
    ui->doubleSpinBox_2->setValue(0);
}

void convolucion::on_pushButton_5_clicked()
{

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            ui->tableWidget->item(i, j)->setText("1");
        }
    }
    ui->doubleSpinBox->setValue(1.0/25);
    ui->doubleSpinBox_2->setValue(0);
}
