#ifndef PONER_TEXTO_H
#define PONER_TEXTO_H

#include <QDialog>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


namespace Ui {
class poner_texto;
}


class poner_texto : public QDialog
{
    Q_OBJECT

public:
    explicit poner_texto(int num_foto, QWidget *parent = nullptr);
    ~poner_texto();
    int getFoto();
    void publicActualizar();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_poner_texto_rejected();

    void on_poner_texto_accepted();

private:
    Ui::poner_texto *ui;

    int nfoto;
    string texto;
    int tam;
    Scalar color;

    void actualizar(bool guardar);
};

#endif // PONER_TEXTO_H
