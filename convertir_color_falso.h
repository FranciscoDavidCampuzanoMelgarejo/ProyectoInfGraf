#ifndef CONVERTIR_COLOR_FALSO_H
#define CONVERTIR_COLOR_FALSO_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class convertir_color_falso;
}

class convertir_color_falso : public QDialog
{
    Q_OBJECT

public:
    explicit convertir_color_falso(int num_foto, QWidget *parent = nullptr);
    ~convertir_color_falso();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_convertir_color_falso_rejected();

    void on_convertir_color_falso_accepted();

private:
    Ui::convertir_color_falso *ui;

    int nfoto;
    int paleta;
    QLabel *labelAnterior;

    void convertir();
    void resaltarLabel(QLabel* label);
};

#endif // CONVERTIR_COLOR_FALSO_H
