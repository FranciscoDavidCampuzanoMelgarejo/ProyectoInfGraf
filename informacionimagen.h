#ifndef INFORMACIONIMAGEN_H
#define INFORMACIONIMAGEN_H

#include <QDialog>
#include <QLabel>
#include <QDebug>
#include <iostream>

using namespace std;

namespace Ui {
class InformacionImagen;
}

class InformacionImagen : public QDialog
{
    Q_OBJECT

public:
    // El array 'informacion' tiene que seguir un orden concreto -> ancho, alto, profundidad, canales
    // memoria, media RGB
    explicit InformacionImagen(QVector<QString> informacion, QWidget *parent = nullptr);
    ~InformacionImagen();

private:
    Ui::InformacionImagen *ui;
    QVector<QLabel*> etiquetas;

    const QString profundidades[8] = {"8u", "8s", "16u", "16s", "32s", "32f", "64f", "16f"};

    // METODOS PRIVADOS
    void inicializarVectorEtiquetas();
    void setColorMedio(QString color);
    void deBytesAMega(QString& memoria);

};

#endif // INFORMACIONIMAGEN_H
