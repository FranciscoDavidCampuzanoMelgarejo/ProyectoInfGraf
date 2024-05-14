#ifndef PERFILADO_H
#define PERFILADO_H

#include <QDialog>

namespace Ui {
class Perfilado;
}

class Perfilado : public QDialog
{
    Q_OBJECT

public:
    explicit Perfilado(int num_foto, QWidget *parent = nullptr);
    ~Perfilado();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_Perfilado_rejected();

    void on_Perfilado_accepted();

private:
    Ui::Perfilado *ui;

    int nfoto;
    int radio;
    double porcentaje;

};

#endif // PERFILADO_H
