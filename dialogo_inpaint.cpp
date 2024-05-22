#include "dialogo_inpaint.h"
#include "ui_dialogo_inpaint.h"
#include "imagenes.h"

Point puntoPrevio(-1, -1);

void callback_inpaint(int event, int x, int y, int flags, void* params) {
    struct ParametrosCallback* parametros = (struct ParametrosCallback*) params;

    if(event == EVENT_LBUTTONUP || !(flags & EVENT_FLAG_LBUTTON))
        puntoPrevio = Point(-1, -1);
    else if(event == EVENT_LBUTTONDOWN)
        puntoPrevio = Point(x, y);
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) {
        Point puntoActual(x, y);
        if(puntoPrevio.x < 0)
            puntoPrevio = puntoActual;
        line(*(parametros->imagen), puntoPrevio, puntoActual, Scalar::all(255), parametros->radio, LINE_AA);
        line(*(parametros->mascara), puntoPrevio, puntoActual, Scalar::all(255), parametros->radio, LINE_AA);
        puntoPrevio = puntoActual;
        imshow("Inpaint", *(parametros->imagen));
    }
}

dialogo_inpaint::dialogo_inpaint(int num_foto, int num_res, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogo_inpaint)
{
    ui->setupUi(this);
    this->nfoto = num_foto;
    this->nres = num_res;
    this->imagen = foto[nfoto].img.clone();
    this->mascara = Mat::zeros(imagen.size(), CV_8U);
    this->paramCallback.imagen = &this->imagen;
    this->paramCallback.mascara = &this->mascara;
    this->paramCallback.radio = 1;

    set_callback_foto(num_foto, false);
    if (parent)
        move(parent->x()+DESP_X_HIJO, parent->y()+DESP_Y_HIJO);
    namedWindow("Inpaint", 0);
    setMouseCallback("Inpaint", callback_inpaint, (void*)&this->paramCallback);
    imshow("Inpaint", this->imagen);
}

dialogo_inpaint::~dialogo_inpaint()
{
    set_callback_foto(this->nfoto, true);
    destroyWindow("Inpaint");
    delete ui;
}

void dialogo_inpaint::on_horizontalSlider_valueChanged(int value)
{
    this->paramCallback.radio = value;
}

void dialogo_inpaint::on_dialogo_inpaint_rejected()
{
    mostrar(nfoto);
}

void dialogo_inpaint::on_dialogo_inpaint_accepted()
{
    aplicarInpaint(nfoto, nres, this->mascara, this->ui->horizontalSlider->value());
}
