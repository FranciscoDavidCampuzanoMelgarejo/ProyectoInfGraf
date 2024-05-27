#include "informacionimagen.h"
#include "ui_informacionimagen.h"

InformacionImagen::InformacionImagen(QVector<QString> informacion, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformacionImagen)
{
    ui->setupUi(this);
    inicializarVectorEtiquetas();

    ui->labelSizeValue->setText(informacion[0] + " x " + informacion[1]); // Ancho x alto
    // Profundidad del pixel
    int prof = informacion[2].toInt();
    ui->labelProfundidadValue->setText(this->profundidades[prof]);

    for(int i = 3, j = 0; i < informacion.size(); i++, j++) {
        // Campo de la memoria total de la imagen
        if(i == 4) {
            deBytesAMega(informacion[i]);
        }
        etiquetas[j]->setText(informacion[i]);
    }
    setColorMedio(informacion[informacion.size() - 1]);

}

InformacionImagen::~InformacionImagen()
{
    delete ui;
}

void InformacionImagen::inicializarVectorEtiquetas() {
    etiquetas.append(ui->labelCanalesValue);
    etiquetas.append(ui->labelMemoriaValue);
    etiquetas.append(ui->labelRGBMediaValue);
}

void InformacionImagen::setColorMedio(QString color) {
    //R G B
    color.replace(" ", ",");
    ui->labelColorMedioValue->setStyleSheet("background-color: rgb(" + color + ")");
}

void InformacionImagen::deBytesAMega(QString &memoria) {
    double bytes = memoria.toDouble();
    const QString arrayUnidades[4] = {"B", "KB", "MB", "GB"};
    QString unidad = arrayUnidades[0];

    int i = 0;
    while(i < 4 && bytes >= 1024) {
        bytes = bytes / 1024.0;
        unidad = arrayUnidades[i++];
    }

    memoria = QString::number(bytes, 'f', 3) + " " + unidad;
    memoria.replace('.', ',');
}
