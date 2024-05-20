#include "poner_texto.h"
#include "ui_poner_texto.h"
#include "imagenes.h"
#include <QColorDialog>
#include <QDebug>

int xPunto = -1, yPunto = -1;


void callback_texto(int event, int x, int y, int flags, void *params) {
    poner_texto* instancia = (poner_texto*)params;
    if(event == EVENT_LBUTTONDOWN) {
        Mat clonada = foto[instancia->getFoto()].img.clone();
       if(x >= 0 && x < clonada.cols)
           xPunto = x;
       if(y >= 0 && y < clonada.rows)
           yPunto = y;

       if(x < 0)
           xPunto = 0;
       if(x >= clonada.cols)
           xPunto = clonada.cols - 1;

       if(y < 0)
           yPunto = 0;
       if(y >= clonada.rows)
           yPunto = clonada.rows - 1;

       instancia->publicActualizar();

    }

}

poner_texto::poner_texto(int num_foto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::poner_texto)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    this->texto = "";
    this->color = Scalar::all(0); // Color negro por defecto
    this->tam = 8;
    qDebug() << "X: " << xPunto << "-----" << "Y: " << yPunto;

    set_callback_foto(nfoto, false);
    setMouseCallback(foto[this->nfoto].nombre, callback_texto, (void*)this);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);
}

poner_texto::~poner_texto()
{
    set_callback_foto(nfoto, true);
    delete ui;
}

void poner_texto::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor();
    if(color.isValid()) {
        QString estilo = "color: rgb(";
        estilo += QString::number(color.red()) + ",";
        estilo += QString::number(color.green()) + ",";
        estilo += QString::number(color.blue()) + ")";
        this->ui->lineEdit->setStyleSheet(estilo);
        this->color = CV_RGB(color.red(), color.green(), color.blue());
        actualizar(false);
    }
}

void poner_texto::on_comboBox_currentTextChanged(const QString &arg1)
{
    bool ok;
    int size = arg1.toInt(&ok);
    if(ok) {
        this->tam = size;
        actualizar(false);
    }
}

void poner_texto::actualizar(bool guardar) {
    if(xPunto != -1 && yPunto != -1) {
        if(!this->texto.empty() && this->texto != "") {
            ponerTextoEnImagen(this->nfoto, this->texto, this->tam, this->color, xPunto, yPunto, guardar);
        } else {
            Mat clonada = foto[nfoto].img.clone();
            circle(clonada, Point(xPunto, yPunto), 5, Scalar::all(0), -1, LINE_AA);
            imshow(foto[nfoto].nombre, clonada);
        }
    } else {
        mostrar(this->nfoto);
    }
}

void poner_texto::on_pushButton_2_clicked()
{
    this->texto = this->ui->lineEdit->text().toStdString();
    actualizar(false);
}

int poner_texto::getFoto() {
    return this->nfoto;
}

void poner_texto::publicActualizar() {
    this->actualizar(false);
}

void poner_texto::on_poner_texto_rejected()
{
    xPunto = yPunto = -1;
    mostrar(this->nfoto);
}

void poner_texto::on_poner_texto_accepted()
{
    this->actualizar(true);
    xPunto = yPunto = -1;
}
