#ifndef CONVERTIR_COLOR_FALSO_H
#define CONVERTIR_COLOR_FALSO_H

#include <QDialog>

namespace Ui {
class convertir_color_falso;
}

class convertir_color_falso : public QDialog
{
    Q_OBJECT

public:
    explicit convertir_color_falso(QWidget *parent = nullptr);
    ~convertir_color_falso();

private:
    Ui::convertir_color_falso *ui;
};

#endif // CONVERTIR_COLOR_FALSO_H
