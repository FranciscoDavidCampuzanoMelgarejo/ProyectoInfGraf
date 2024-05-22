#ifndef DIALOGO_INPAINT_H
#define DIALOGO_INPAINT_H

#include <QDialog>
#include "opencv2/opencv.hpp"

using namespace cv;

struct ParametrosCallback {
    Mat* imagen;
    Mat* mascara;
    int radio;
};

namespace Ui {
class dialogo_inpaint;
}

class dialogo_inpaint : public QDialog
{
    Q_OBJECT

public:
    explicit dialogo_inpaint(int num_foto, int num_res, QWidget *parent = nullptr);
    ~dialogo_inpaint();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_dialogo_inpaint_rejected();

    void on_dialogo_inpaint_accepted();

private:
    Ui::dialogo_inpaint *ui;

    int nfoto, nres;
    Mat imagen, mascara;
    struct ParametrosCallback paramCallback;
};

#endif // DIALOGO_INPAINT_H
