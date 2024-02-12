#ifndef CAPTURARVIDEO_H
#define CAPTURARVIDEO_H

#include <QDialog>
#include "imagenes.h"

namespace Ui {
class Capturarvideo;
}

class Capturarvideo : public QDialog
{
    Q_OBJECT

public:
    explicit Capturarvideo(string nombre, QWidget *parent = nullptr);
    ~Capturarvideo();

    bool isOpened();

private slots:
    void on_horizontalSlider_3_valueChanged(int value);

    void on_spinBox_3_valueChanged(int arg1);

    void on_Capturarvideo_rejected();

    void on_Capturarvideo_accepted();

private:
    VideoCapture cap;
    Ui::Capturarvideo *ui;
};

#endif // CAPTURARVIDEO_H
