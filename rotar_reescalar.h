#ifndef ROTAR_REESCALAR_H
#define ROTAR_REESCALAR_H

#include <QDialog>

namespace Ui {
class rotar_reescalar;
}

class rotar_reescalar : public QDialog
{
    Q_OBJECT

public:
    explicit rotar_reescalar(int num_foto, QWidget *parent = nullptr);
    ~rotar_reescalar();

private slots:
    void on_dial_valueChanged(int value);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::rotar_reescalar *ui;

    int nfoto;

    int ajustarAngulo(int angulo);
};

#endif // ROTAR_REESCALAR_H
