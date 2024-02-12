#ifndef BAJORRELIEVE_H
#define BAJORRELIEVE_H

#include <QDialog>

namespace Ui {
class bajorrelieve;
}

class bajorrelieve : public QDialog
{
    Q_OBJECT

public:
    explicit bajorrelieve(int numfoto, int numres, QWidget *parent = nullptr);
    ~bajorrelieve();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_dial_valueChanged(int value);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_bajorrelieve_accepted();

    void on_bajorrelieve_rejected();

private:
    int nfoto;
    int nres;
    double angulo;
    double grado;
    int nfondo;
    Ui::bajorrelieve *ui;
};

#endif // BAJORRELIEVE_H
