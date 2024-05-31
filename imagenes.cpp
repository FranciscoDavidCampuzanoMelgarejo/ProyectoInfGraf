//---------------------------------------------------------------------------

#include "imagenes.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <assert.h>

#include <QDebug>

///////////////////////////////////////////////////////////////////
/////////  VARIABLES GLOBALES                        //////////////
///////////////////////////////////////////////////////////////////

ventana foto[MAX_VENTANAS];

tipo_herramienta herr_actual= HER_PUNTO;

int radio_pincel= 10;

Scalar color_pincel= CV_RGB(255, 255, 255);

int difum_pincel= 10;

bool preguntar_guardar= true;

static int numpos= 0; // Número actual en el orden de posición de las ventanas


// FUNCIONES AUXILIARES
void ajustarROICopia(Rect &roi, int &posx, int &posy, int factual) {
    Mat imagen = foto[factual].img;
    if(roi.x < 0) {
        roi.width = roi.width + roi.x;
        posx = posx + roi.x;
        roi.x = 0;
    }

    if(roi.y < 0) {
        roi.height = roi.height + roi.y;
        posy = posy + roi.y;
        roi.y = 0;
    }

    if(roi.x + roi.width > imagen.cols) {
        roi.width = imagen.cols - roi.x;
    }

    if(roi.y + roi.height > imagen.rows) {
        roi.height = imagen.rows - roi.y;
    }
}

void reestructurarROI(Rect &roi, int ancho, int alto) {
    roi.width = ancho;
    roi.height = alto;
}


///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE MANEJO DE VENTANAS           //////////////
///////////////////////////////////////////////////////////////////

void callback (int event, int x, int y, int flags, void *_nfoto);

//---------------------------------------------------------------------------

void inic_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        foto[i].usada= false;
}

//---------------------------------------------------------------------------

void fin_fotos (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            destroyWindow(foto[i].nombre);
            foto[i].usada= false;
        }
    }
}

//---------------------------------------------------------------------------

int primera_libre (void)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (!foto[i].usada)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

void escribir_barra_estado (void)
{
    int usadas, modificadas;
    num_fotos(usadas, modificadas);
    w->setStatusBarText(QString::number(usadas)+" fotos abiertas, "+
                        QString::number(modificadas)+" modificadas.");
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, int ancho, int alto, Scalar color)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img.create(alto, ancho, CV_8UC3);
    foto[nfoto].img=  color;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, ancho, alto);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, Mat img)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada && !img.empty());
    foto[nfoto].nombre= "nueva-"+to_string(nfoto)+".png";
    foto[nfoto].nombref= foto[nfoto].nombre;
    foto[nfoto].img= img;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= true;
    foto[nfoto].roi= Rect(0, 0, img.cols, img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void crear_nueva (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && !foto[nfoto].usada);
    foto[nfoto].nombre= nombre;
    foto[nfoto].nombref= Lt1(nombre);
    foto[nfoto].img= imread(foto[nfoto].nombref);
    if (foto[nfoto].img.empty())
        return;
    namedWindow(foto[nfoto].nombre, WINDOW_NO_POPUP+WINDOW_MOVE_RIGHT);
    foto[nfoto].orden= numpos++;
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    foto[nfoto].usada= true;
    foto[nfoto].modificada= false;
    foto[nfoto].roi= Rect(0, 0, foto[nfoto].img.cols, foto[nfoto].img.rows);
    setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void guardar_foto (int nfoto, string nombre)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    foto[nfoto].modificada= false;
    // 1. Guardar la imagen con el mismo nombre que tiene
    if (nombre=="")
        imwrite(foto[nfoto].nombref, foto[nfoto].img);
    // 2. Guardar la imagen con un nombre distinto al actual
    else {
        imwrite(Lt1(nombre), foto[nfoto].img);
        setWindowTitle(foto[nfoto].nombre, nombre);
    }
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void cerrar_foto (int nfoto, bool destruir_ventana)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (destruir_ventana)
        destroyWindow(foto[nfoto].nombre);
    foto[nfoto].usada= false;
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

void mostrar (int nfoto)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    imshow(foto[nfoto].nombre, foto[nfoto].img);
    escribir_barra_estado();
}

//---------------------------------------------------------------------------

int nombre_a_numero (string nombre)
{
    for (int i= 0; i<MAX_VENTANAS; i++)
        if (foto[i].usada && foto[i].nombre==nombre)
            return i;
    return -1;
}

//---------------------------------------------------------------------------

int foto_activa (void)
{
    int maxorden= -1, maxpos= -1;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada && foto[i].orden>maxorden) {
            maxorden= foto[i].orden;
            maxpos= i;
        }
    }
    return maxpos;
}

//---------------------------------------------------------------------------

int num_fotos (int &usadas, int &modificadas)
{
    usadas= 0;
    modificadas= 0;
    for (int i= 0; i<MAX_VENTANAS; i++) {
        if (foto[i].usada) {
            usadas++;
            if (foto[i].modificada)
                modificadas++;
        }
    }
    return usadas;
}

//---------------------------------------------------------------------------

void nueva_desde_portapaples(int nfoto, QImage& imagen) {
    Mat img(imagen.height(), imagen.width(), CV_8UC4, imagen.scanLine(0));
    cvtColor(img, img, COLOR_RGBA2RGB);
    crear_nueva(nfoto, img);
}



void set_callback_foto (int nfoto, bool activo)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    if (!activo)
        setMouseCallback(foto[nfoto].nombre, nullptr);
    else
        setMouseCallback(foto[nfoto].nombre, callback, reinterpret_cast<void*>(nfoto));
}

///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DEL CALLBACK PRINCIPAL          //////////////
///////////////////////////////////////////////////////////////////

static int downx, downy;
// Posición inicial del ratón al pinchar sobre la imagen actual

static bool fueraImagen = false;

// Foto 'original' de la que se copian trozos a otras fotos con la herramienta COPIA
static int fotoOrigenCopia = -1;

// Para saber si se ha copiado una foto o todavia no
static bool isCopiado = false;

// Coordenadas (x,y) del click que se hizo en la foto cuyos trozos se van a copiar a otras fotos
static int xOrigen, yOrigen;

//---------------------------------------------------------------------------

void ninguna_accion (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    circle(res, Point(x, y), radio_pincel, CV_RGB(255,255,255), 2, LINE_AA);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_close (int factual)
{
    if (foto[factual].usada && !foto[factual].img.empty()) {
        if (foto[factual].modificada && preguntar_guardar) {
            QString cadena= "El archivo " + QString::fromStdString(foto[factual].nombre) +
                    " ha sido modificado.\n¿Desea guardarlo?";
            int resp= QMessageBox::question(w, "Archivo modificado", cadena,
                                            QMessageBox::Yes | QMessageBox::No );
            if (resp==QMessageBox::Yes)
                guardar_foto(factual);
        }
        cerrar_foto(factual, false);
    }
}

//---------------------------------------------------------------------------

void cb_punto (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, color_pincel, -1, LINE_AA);
    else {
        int tam = radio_pincel + difum_pincel;
        int posx = tam, posy = tam;
        Rect roi(x-tam, y-tam, 2*tam+1, 2*tam+1);
        if(roi.x < 0) {
            roi.width = roi.width + x;
            posx += roi.x;
            roi.x = 0;
        }
        if(roi.y < 0) {
            roi.height = roi.height + y;
            posy += roi.y;
            roi.y = 0;
        }

        if(roi.x + roi.width > im.cols) {
            roi.width = im.cols - roi.x;
        }
        if(roi.y + roi.height > im.rows) {
            roi.height = im.rows - roi.y;
        }
        Mat trozo = im(roi);
        Mat res(roi.size(), im.type(), color_pincel);
        Mat cop(roi.size(), im.type(), CV_RGB(0,0,0));
        circle(cop, Point(posx, posy), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(trozo, cop, trozo, 1.0/255.0);
        trozo= res + trozo;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

Scalar color_arco_iris() {
    static int estado = 0;
    static Scalar color_actual = CV_RGB(255, 0, 0);
    const int salto = 10;

    switch (estado) {
        case 0:
            color_actual.val[1] += salto;
            if(color_actual.val[1] >= 255) {
                estado++;
            }
            break;
        case 1:
            color_actual.val[2] -= salto;
            if(color_actual.val[2] <= 0) {
                estado++;
            }
            break;
        case 2:
            color_actual.val[0] += salto;
            if(color_actual.val[0] >= 255) estado++;
            break;
        case 3:
            color_actual.val[1] -= salto;
            if(color_actual.val[1] <= 0) estado++;
            break;
        case 4:
            color_actual.val[2] += salto;
            if(color_actual.val[2] >= 255) estado++;
            break;
        case 5:
            color_actual.val[0] -= salto;
            if(color_actual.val[0] <= 0) estado = 0;
            break;
    }
    return color_actual;
}

//---------------------------------------------------------------------------

void cb_arco_iris (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        circle(im, Point(x, y), radio_pincel, color_arco_iris(), -1, LINE_AA);
    else {
        int tam = radio_pincel + difum_pincel;
        int posx = tam, posy = tam;
        Rect roi(x-tam, y-tam, 2*tam+1, 2*tam+1);
        if(roi.x < 0) {
            roi.width = roi.width + x;
            posx += roi.x;
            roi.x = 0;
        }
        if(roi.y < 0) {
            roi.height = roi.height + y;
            posy += roi.y;
            roi.y = 0;
        }

        if(roi.x + roi.width > im.cols) {
            roi.width = im.cols - roi.x;
        }
        if(roi.y + roi.height > im.rows) {
            roi.height = im.rows - roi.y;
        }
        Mat trozo = im(roi);
        Mat res(roi.size(), im.type(), color_arco_iris());
        Mat cop(roi.size(), im.type(), CV_RGB(0,0,0));
        circle(cop, Point(posx, posy), radio_pincel, CV_RGB(255,255,255), -1, LINE_AA);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(trozo, cop, trozo, 1.0/255.0);
        trozo= res + trozo;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}


//---------------------------------------------------------------------------

void cb_linea (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        line(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2+1);
    else {
        int tam = radio_pincel + difum_pincel;
        int anchoRoi = abs(downx - x) + 2*tam+1;
        int altoRoi = abs(downy - y) + 2*tam+1;

        int coordenadaX, coordenadaY;
        if(downx <= x)
            coordenadaX = downx;
        else
            coordenadaX = x;

        if(downy <= y)
            coordenadaY = downy;
        else
            coordenadaY = y;
        Rect roi(coordenadaX - tam, coordenadaY - tam, anchoRoi, altoRoi);

        if(roi.x < 0) {
            roi.width = roi.width + coordenadaX;
            roi.x = 0;
        }

        if(roi.y < 0) {
            roi.height = roi.height + coordenadaY;
            roi.y = 0;
        }

        if(roi.x + roi.width >= im.cols) {
            roi.width = im.cols - roi.x;
        }

        if(roi.y + roi.height >= im.rows) {
            roi.height = im.rows - roi.y;
        }

        Point new_down(downx - roi.x, downy - roi.y);
        Point new_end(x - roi.x, y - roi.y);

        Mat trozo = im(roi);
        Mat res(trozo.size(), im.type(), color_pincel);
        Mat cop(trozo.size(), im.type(), CV_RGB(0,0,0));
        line(cop, new_down, new_end, CV_RGB(255,255,255), radio_pincel*2+1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(trozo, cop, trozo, 1.0/255.0);
        trozo= res + trozo;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_rectangulo (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        rectangle(im, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        rectangle(cop, Point(downx, downy), Point(x,y), CV_RGB(255,255,255), radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_elipse (int factual, int x, int y)
{
    Mat im= foto[factual].img;  // Ojo: esto no es una copia, sino a la misma imagen
    if (difum_pincel==0)
        ellipse(im, Point(downx, downy), Size(abs(x - downx), abs(y - downy)), 0, 0, 360, color_pincel, radio_pincel*2-1);
    else {
        Mat res(im.size(), im.type(), color_pincel);
        Mat cop(im.size(), im.type(), CV_RGB(0,0,0));
        ellipse(cop, Point(downx, downy), Size(abs(x - downx), abs(y - downy)), 0, 0, 360, CV_RGB(255,255,255), radio_pincel*2-1);
        blur(cop, cop, Size(difum_pincel*2+1, difum_pincel*2+1));
        multiply(res, cop, res, 1.0/255.0);
        bitwise_not(cop, cop);
        multiply(im, cop, im, 1.0/255.0);
        im= res + im;
    }
    imshow(foto[factual].nombre, im);
    foto[factual].modificada= true;
}

//---------------------------------------------------------------------------

void cb_ver_linea (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    line(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2+1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_ver_rectangulo (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    rectangle(res, Point(downx, downy), Point(x,y), color_pincel, radio_pincel*2-1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_ver_elipse (int factual, int x, int y)
{
    Mat res= foto[factual].img.clone();
    ellipse(res, Point(downx, downy), Size(abs(x - downx), abs(y - downy)), 0, 0, 360, color_pincel, radio_pincel*2-1);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

void cb_seleccionar (int factual, int x, int y)
{
    Mat im= foto[factual].img;
    Rect nuevo= Rect(min(downx, x), min(downy, y),
                     max(downx, x)-min(downx, x)+1, max(downy, y)-min(downy, y)+1);
    if (nuevo.x<0)
        nuevo.x= 0;
    if (nuevo.y<0)
        nuevo.y= 0;
    if (nuevo.x+nuevo.width>im.size().width)
        nuevo.width= im.size().width-nuevo.x;
    if (nuevo.y+nuevo.height>im.size().height)
        nuevo.height= im.size().height-nuevo.y;
    foto[factual].roi= nuevo;
}

//---------------------------------------------------------------------------

void cb_ver_seleccion (int factual, int x, int y, bool foto_roi)
{
    Mat res= foto[factual].img.clone();
    Point p1, p2;
    if (foto_roi) {
        p1.x= foto[factual].roi.x;
        p1.y= foto[factual].roi.y;
        p2.x= foto[factual].roi.x+foto[factual].roi.width-1;
        p2.y= foto[factual].roi.y+foto[factual].roi.height-1;
    }
    else {
        p1= Point(downx, downy);
        p2= Point(x, y);
    }
    rectangle(res, p1, p2, CV_RGB(255,foto_roi?0:255,0),2);
    imshow(foto[factual].nombre, res);
}

//---------------------------------------------------------------------------

// Funcion que se encarga de aplicar el rellenado (floodFill) sobre una imagen
// Por defecto se aplica sobre la copia (visualizacion)
void cb_rellenado(int factual, int x, int y, bool isAplicado = false) {
    double factorTolerancia = radio_pincel / 120.0;
    double factorOpacidad = difum_pincel / 120.0;

    Scalar lowDiff = Scalar::all(factorTolerancia * 90);
    Scalar upDiff = Scalar::all(factorTolerancia * 90);

    Mat imagen = foto[factual].img;
    Mat imagenRelleno = imagen.clone();
    if(!isAplicado) {
        floodFill(imagenRelleno, Point(x, y), color_pincel, nullptr, lowDiff, upDiff, FLOODFILL_FIXED_RANGE);
        addWeighted(imagen, factorOpacidad, imagenRelleno, (1.0 - factorOpacidad), 0, imagenRelleno);
        imshow(foto[factual].nombre, imagenRelleno);
    } else {
        floodFill(imagenRelleno, Point(x, y), color_pincel, nullptr, lowDiff, upDiff, FLOODFILL_FIXED_RANGE);
        addWeighted(imagen, factorOpacidad, imagenRelleno, (1.0 - factorOpacidad), 0, imagen);
        imshow(foto[factual].nombre, imagen);
        foto[factual].modificada = true;
    }
}

//---------------------------------------------------------------------------
void cb_suavizado(int factual, int x, int y) {
    Mat imagen = foto[factual].img;

    int suavizado = std::min(31, difum_pincel*2+1);
    qDebug() << "Suavizado: " << suavizado;

    Rect roi(x - radio_pincel, y - radio_pincel, 2*radio_pincel+1, 2*radio_pincel+1);

    int posx = radio_pincel;
    int posy = radio_pincel;

    if(roi.x < 0) {
        roi.width = roi.width + x;
        posx = posx + roi.x;
        roi.x = 0;
    }

    if(roi.y < 0) {
        roi.height = roi.height + y;
        posy = posy + roi.y;
        roi.y = 0;
    }

    if(roi.x + roi.width > imagen.cols) {
        roi.width = imagen.cols - roi.x;
    }

    if(roi.y + roi.height > imagen.rows) {
        roi.height = imagen.rows - roi.y;
    }

    Mat trozo = imagen(roi);
    Mat copiaTrozo = trozo.clone();
    Mat mascara(roi.size(), imagen.type(), CV_RGB(0, 0, 0));

    circle(mascara, Point(posx, posy), radio_pincel, CV_RGB(255, 255, 255), -1, LINE_AA);
    GaussianBlur(copiaTrozo, copiaTrozo, Size(suavizado, suavizado), 0, 0, BORDER_DEFAULT);

    multiply(copiaTrozo, mascara, copiaTrozo, 1.0/255.0);
    bitwise_not(mascara, mascara);
    multiply(trozo, mascara, trozo, 1.0/255.0);
    trozo = trozo + copiaTrozo;

    imshow(foto[factual].nombre, imagen);
    foto[factual].modificada = true;
}

//---------------------------------------------------------------------------

void cb_copia(int factual, int x, int y) {
    if(!isCopiado) {
        xOrigen = x;
        yOrigen = y;
        isCopiado = true;
        fotoOrigenCopia = factual;
    } else {
        int suavizado = std::min(31, difum_pincel*2+1);
        int posxOrigen, posyOrigen;
        int posxCopia, posyCopia;
        posxOrigen = posyOrigen = posxCopia = posyCopia = radio_pincel;
        Rect roiOrigen(xOrigen - radio_pincel, yOrigen - radio_pincel, 2*radio_pincel+1, 2*radio_pincel+1);
        Rect roiCopia(x - radio_pincel, y - radio_pincel, 2*radio_pincel+1, 2*radio_pincel+1);

        // Ajustar los ROI
        ajustarROICopia(roiOrigen, posxOrigen, posyOrigen, fotoOrigenCopia);
        ajustarROICopia(roiCopia, posxCopia, posyCopia, factual);

        int anchoMinimo = std::min(roiOrigen.size().width, roiCopia.size().width);
        int altoMinimo = std::min(roiOrigen.size().height, roiCopia.size().height);

        // Reestructurar los roi
        if(roiOrigen.size().width > anchoMinimo || roiOrigen.size().height > altoMinimo) {
            reestructurarROI(roiOrigen, anchoMinimo, altoMinimo);
        }
        if(roiCopia.size().width > anchoMinimo || roiCopia.size().height > altoMinimo) {
            reestructurarROI(roiCopia, anchoMinimo, altoMinimo);
        }

        Mat origen = foto[fotoOrigenCopia].img(roiOrigen).clone();
        blur(origen, origen, Size(suavizado, suavizado));

        Mat destino = foto[factual].img(roiCopia);
        Mat mascara(roiCopia.size(), destino.type(), CV_RGB(0, 0, 0));
        circle(mascara, Point(posxCopia, posyCopia), radio_pincel, CV_RGB(255, 255, 255), -1, LINE_AA);

        multiply(origen, mascara, origen, 1.0/255.0);
        bitwise_not(mascara, mascara);

        multiply(destino, mascara, destino, 1.0/255.0);
        destino = destino + origen;
        foto[factual].modificada = true;

    }
    imshow(foto[factual].nombre, foto[factual].img);
}
//---------------------------------------------------------------------------

void callback (int event, int x, int y, int flags, void *_nfoto)
{
    int factual= reinterpret_cast<int>(_nfoto);

    // 1. Eventos y casos especiales
    // 1.1. Evento cerrar ventana
    if (event==EVENT_CLOSE) {
        cb_close(factual);
        return;
    }
    // 1.2. Evento obtiene el foco
    if (event==EVENT_FOCUS) {
        foto[factual].orden= numpos++;
    }
    // 1.3. El ratón se sale de la ventana
    if (x<0 || x>=foto[factual].img.size().width || y<0 || y>=foto[factual].img.size().height) {
        fueraImagen = true;
        return;
    }

    // 1.4. Se inicia la pulsación del ratón
    if (event==EVENT_LBUTTONDOWN) {
        downx= x;
        downy= y;
    }

    if(fotoOrigenCopia != -1 && herr_actual != HER_COPIA) {
        isCopiado = false;
    }

    // 2. Según la herramienta actual
    switch (herr_actual) {

    // 2.1. Herramienta PUNTO
    case HER_PUNTO:
        if (flags==EVENT_FLAG_LBUTTON)
            cb_punto(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.2. Herramienta LINEA
    case HER_LINEA:
        if (event==EVENT_LBUTTONUP)
            cb_linea(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_linea(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

    case HER_TRAZO:
        if(fueraImagen) {
            downx = x;
            downy = y;
            fueraImagen = false;
        }
        if(event == EVENT_LBUTTONUP)
            cb_linea(factual, x, y);
        else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON) {
            cb_linea(factual, x, y);
            downx = x;
            downy = y;
        } else
            ninguna_accion(factual, x, y);
        break;

        // 2.3. Herramienta SELECCION
    case HER_SELECCION:
        if (event==EVENT_LBUTTONUP)
            cb_seleccionar(factual, x, y);
        else if (event==EVENT_MOUSEMOVE)
            cb_ver_seleccion(factual, x, y, flags!=EVENT_FLAG_LBUTTON);
        break;

        // 2.4. Herramienta RECTANGULO
    case HER_RECTANGULO:
        if (event==EVENT_LBUTTONUP)
            cb_rectangulo(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_rectangulo(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.5. Herramienta ELIPSE
    case HER_ELIPSE:
        if (event==EVENT_LBUTTONUP)
            cb_elipse(factual, x, y);
        else if (event==EVENT_MOUSEMOVE && flags==EVENT_FLAG_LBUTTON)
            cb_ver_elipse(factual, x, y);
        else
            ninguna_accion(factual, x, y);
        break;

        // 2.6. Herramienta ARCO IRIS
        case HER_ARCO_IRIS:
            if (flags==EVENT_FLAG_LBUTTON)
                cb_arco_iris(factual, x, y);
            else
                ninguna_accion(factual, x, y);
            break;

        case HER_RELLENADO:
            if(event == EVENT_MOUSEMOVE) {
                cb_rellenado(factual, x, y);
            } else if (event == EVENT_LBUTTONDOWN) {
                cb_rellenado(factual, x, y, true);
            } else {
                ninguna_accion(factual, x, y);
            }
            break;

        case HER_SUAVIZADO:
        if(event == EVENT_LBUTTONDOWN) {
            cb_suavizado(factual, x, y);
        } else {
            ninguna_accion(factual, x, y);
        }
        break;

        case HER_COPIA:
            if(event == EVENT_LBUTTONDOWN) {
                cb_copia(factual, x, y);
            } else {
                ninguna_accion(factual, x, y);
            }
    }
    escribir_barra_estado();
}


///////////////////////////////////////////////////////////////////
/////////  FUNCIONES DE PROCESAMIENTO DE IMAGENES    //////////////
///////////////////////////////////////////////////////////////////

void invertir (int nfoto, int nres)
{
    Mat img(foto[nfoto].img.size(), foto[nfoto].img.type());
    bitwise_not(foto[nfoto].img, img);
    crear_nueva(nres, img);
}

//---------------------------------------------------------------------------

void rotar_angulo (Mat imagen, Mat &imgRes, double angulo, double escala, int modo)
{
    double w= imagen.size().width, h= imagen.size().height;
    Size sres;
    if (modo==0) {     // Reescalar con proporción al original
        sres.width= int(w*escala);
        sres.height= int(h*escala);
    }
    else if (modo==1)  // Reescalar y ampliar para caber entera
        sres.width= sres.height= int(sqrt(w*w + h*h)*escala);
    else               // Reescalar y recortar para no mostrar borde
        sres.width= sres.height= int(min(w, h)*escala/sqrt(2.0));
    imgRes.create(sres, imagen.type());
    double sa= sin(angulo*M_PI/180), ca= cos(angulo*M_PI/180);
    double cx= -w/2*ca-h/2*sa, cy= w/2*sa-h/2*ca;
    Mat M= getRotationMatrix2D(Point2f(0,0), angulo, escala);
    M.at<double>(0,2)= sres.width/2+cx*escala;
    M.at<double>(1,2)= sres.height/2+cy*escala;
    imgRes= color_pincel;
    warpAffine(imagen, imgRes, M, sres, INTER_CUBIC);
}

//---------------------------------------------------------------------------

void rotar_exacto (int nfoto, int nres, int grado)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(nres>=0 && nres<MAX_VENTANAS && !foto[nres].usada);
    Mat entrada= foto[nfoto].img;
    Mat salida;
    if (grado == 0)
        salida= entrada.clone();
    else if (grado == 1) {
        transpose(entrada, salida);
        flip(salida, salida, 1);
    }
    else if (grado == 2)
        flip(entrada, salida, -1);
    else if (grado == 3) {
        transpose(entrada, salida);
        flip(salida, salida, 0);
    }
    crear_nueva(nres, salida);
}

//---------------------------------------------------------------------------

void ver_brillo_contraste_gama (int nfoto, double suma, double prod, double gama, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    Mat img;
    foto[nfoto].img.convertTo(img, CV_8UC3, prod, suma);

    Mat img32f;
    img.convertTo(img32f, CV_32FC3, 1.0/255);
    pow(img32f, gama, img32f);
    img32f.convertTo(img, CV_8UC3, 255);
    imshow(foto[nfoto].nombre, img);

    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}

//---------------------------------------------------------------------------

void escala_color(int nfoto, int nres) {
    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    cvtColor(gris, gris, COLOR_GRAY2BGR);

    Mat lut(256, 1, CV_8UC3);
    int vr = color_pincel.val[2]; // Color rojo
    int vg = color_pincel.val[1]; // Color verde
    int vb = color_pincel.val[0]; // Color azul
    int vgris = (vr+vg+vb)/3; // Gris aproximado
    for(int A = 0; A < 256; A++) {
        if(A < vgris) {
            lut.at<Vec3b>(A) = Vec3b(vb*A/vgris, vg*A/vgris, vr*A/vgris);
        } else {
            lut.at<Vec3b>(A) = Vec3b(vb+(255-vb) * (A-vgris)/(256 - vgris),
                                     vg+(255-vg) * (A-vgris)/(256 - vgris),
                                     vr+(255-vr) * (A-vgris)/(256 - vgris));
        }
    }

    Mat res;
    LUT(gris, lut, res);
    crear_nueva(nres, res);
}

//---------------------------------------------------------------------------

Mat op_pinchar_estirar(Mat img, int cx, int cy, double radio, double grado) {

    Mat S(img.rows, img.cols, CV_32FC1);
    for(int y = 0; y < S.rows; y++) {
        for(int x = 0; x < S.cols; x++) {
            S.at<float>(y, x) = exp(-((x-cx)*(x-cx) + (y-cy)*(y-cy))/(radio*radio));
        }
    }

    Mat Gx, Gy;
    Sobel(S, Gx, CV_32F, 1, 0, 3, grado);
    Sobel(S, Gy, CV_32F, 0, 1, 3, grado);

    multiply(S, Gx, Gx);
    multiply(S, Gy, Gy);

    for(int y = 0; y < S.rows; y++) {
        for(int x = 0; x < S.cols; x++) {
            Gx.at<float>(y,x) += x;
            Gy.at<float>(y,x) += y;
        }
    }

    Mat res;
    remap(img, res, Gx, Gy, INTER_CUBIC, BORDER_REFLECT);
    return res;
}

//---------------------------------------------------------------------------

void ver_pinchar_estirar(int nfoto, int cx, int cy, double radio, double grado, bool guardar) {

    Mat res = op_pinchar_estirar(foto[nfoto].img, cx, cy, radio, grado);

    if(guardar) {
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    } else {
        imshow("Pinchar/Estirar", res);
    }

}

//---------------------------------------------------------------------------

void ver_suavizado (int nfoto, int ntipo, int tamx, int tamy, bool guardar)
{
    assert(nfoto>=0 && nfoto<MAX_VENTANAS && foto[nfoto].usada);
    assert(tamx>0 && tamx&1 && tamy>0 && tamy&1);
    Mat img= foto[nfoto].img.clone();
    Mat trozo = img(foto[nfoto].roi);
    if (ntipo == 1)
        GaussianBlur(trozo, trozo, Size(tamx, tamy), 0);
    else if (ntipo == 2)
        blur(trozo, trozo, Size(tamx, tamy));
    else if (ntipo == 3) {
        medianBlur(trozo, trozo, tamx);
    }
    imshow(foto[nfoto].nombre, img);
    if (guardar) {
        img.copyTo(foto[nfoto].img);
        foto[nfoto].modificada= true;
    }
}
//---------------------------------------------------------------------------

void ver_perspectiva(int nfoto_origen, int nfoto_destino, Point2f pt_origen[], Point2f pt_destino[], bool guardar) {

    Mat M = getPerspectiveTransform(pt_origen, pt_destino);
    Mat res = foto[nfoto_destino].img.clone();
    warpPerspective(foto[nfoto_origen].img, res, M, res.size(), INTER_LINEAR, BORDER_TRANSPARENT);

    if(guardar) {
        res.copyTo(foto[nfoto_destino].img);
        foto[nfoto_destino].modificada = true;
    } else {
        for(int i = 0; i < 4; i++) {
            line(res, pt_destino[i], pt_destino[(i+1)%4], CV_RGB(0, 0, 0), 2);
        }
        for(int i = 0; i < 4; i++) {
            circle(res, pt_destino[i], 10, CV_RGB(0, 255, 0), -1);
        }
        imshow("Perspectiva", res);
    }
}

//---------------------------------------------------------------------------
void ver_bajorrelieve(int nfoto, int nres, double angulo, double grado, int nfondo, bool guardar) {

    QString nombres[4] = {":/imagenes/arena.jpg", ":/imagenes/cielo.jpg", ":/imagenes/gris.png", ":/imagenes/madera.jpg"};
    QImage qimg = QImage(nombres[nfondo]);
    Mat img(qimg.height(), qimg.width(), CV_8UC4, qimg.scanLine(0));
    cvtColor(img, img, COLOR_RGBA2RGB);
    resize(img, img, foto[nfoto].img.size());

    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);

    Mat rotada;
    rotar_angulo(gris, rotada, angulo, 1.0, 1);

    Mat sobel;
    Sobel(rotada, sobel, CV_8U, 1, 0, 3, grado, 128, BORDER_REFLECT);

    rotar_angulo(sobel, rotada, -angulo, 1.0, 0);

    Rect roi(rotada.cols/2 - gris.cols/2, rotada.rows/2 - gris.rows/2, gris.cols, gris.rows);
    sobel = rotada(roi);
    cvtColor(sobel, gris, COLOR_GRAY2RGB);
    addWeighted(img, 1.0, gris, 1.0, -128, img); // Suma ponderada

    if(guardar){
        crear_nueva(nres, img);
    } else {
        imshow("Bajorrelieve", img);
    }
}


//---------------------------------------------------------------------------
void ver_histograma(int nfoto, int canal, int nres) {
    QImage imq = QImage(":/imagenes/histbase.png");
    Mat img(imq.height(), imq.width(), CV_8UC4, imq.scanLine(0));
    cvtColor(img, img, COLOR_RGBA2RGB);

    Mat gris;

    if(canal == 3) {
        cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    }

    int canales[1] = {0};
    int bins[1] = {256};
    float rango[2] = {0, 256};
    const float *rangos[] = {rango};
    Mat hist;
    if(canal == 3) {
        calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);
    } else {
        calcHist(&foto[nfoto].img, 1, &canal, noArray(), hist, 1, bins, rangos);
    }

    double vmin, vmax;
    minMaxLoc(hist, &vmin, &vmax);
    double escala = 182/vmax;
    for(int i = 0; i < 256; i++) {
        rectangle(img, Point(3+i*391/256.0, 185), Point(3+(i+1)*391/256.0, 185 - hist.at<float>(i) * escala), CV_RGB(canal == 2 ? 255: 0, canal == 1 ? 255: 0, canal == 0 ? 255: 0), -1);
    }
    crear_nueva(nres, img);
}

//---------------------------------------------------------------------------
void copiar_a_nueva(int nfoto, int nres) {
    Mat trozo = foto[nfoto].img(foto[nfoto].roi).clone();
    crear_nueva(nres, trozo);

}

//---------------------------------------------------------------------------

QImage copiar_al_portapapeles(int nfoto) {
    Mat trozo = foto[nfoto].img(foto[nfoto].roi).clone();
    cvtColor(trozo, trozo, COLOR_BGR2RGB); // Mat -> BGR; QImage -> RGB
    QImage imagen(trozo.data, trozo.cols, trozo.rows, trozo.step, QImage::Format_RGB888);
    return imagen.copy();
}

//--------------------------------------------------------------------------

QVector<QString> ver_informacion(int nfoto) {
    Mat imagen = foto[nfoto].img;
    QString ancho = QString::number(imagen.cols);
    QString alto = QString::number(imagen.rows);
    QString profundidad = QString::number(imagen.depth());
    QString canales = QString::number(imagen.channels());
    QString memoria = QString::number(imagen.total() * imagen.elemSize());

    Scalar colorMedio = mean(imagen);
    QString RGBMedio = QString::number((int)colorMedio[2]) + " " + QString::number((int)colorMedio[1]) + " " + QString::number((int)colorMedio[0]); // BGR -> RGB

    return {ancho, alto, profundidad, canales, memoria, RGBMedio};
}

//---------------------------------------------------------------------------

void ver_ajuste_lineal(int nfoto, double pmin, double pmax, bool guardar) {
    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    int canales[1] = {0};
    int bins[1] = {256};
    float rango[2] = {0, 256};
    const float *rangos[] = {rango};
    Mat hist;
    calcHist(&gris, 1, canales, noArray(), hist , 1, bins, rangos);

    normalize(hist, hist, 100, 0, NORM_L1);
    int m = 0, M = 255;
    for(double acum = 0; m < 255 && acum < pmin; m++) {
        acum += hist.at<float>(m);
    }
    for(double acum = 0; M > 0 && acum < pmax; M--) {
        acum += hist.at<float>(M);
    }

    if(m >= M)
        M = m + 1;

    double a = 255.0/(M - m);
    double b = -m * a;
    Mat res;
    foto[nfoto].img.convertTo(res, CV_8UC3, a, b);
    imshow(foto[nfoto].nombre, res);

    if(guardar) {
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
}

//---------------------------------------------------------------------------

Mat op_mat_sat_lum(Mat img, double sat, double lum, int matiz) {

    Mat hls;
    cvtColor(img, hls, COLOR_BGR2HLS_FULL);
    Mat canales[3];
    split(hls, canales);

    Mat im16;
    canales[0].convertTo(im16, CV_16S, 1, matiz);
    bitwise_and(im16, Scalar(255), im16);
    im16.convertTo(canales[0], CV_8U);
    canales[1] *= lum;
    canales[2] *= sat;
    merge(canales, 3, hls);

    Mat res;
    cvtColor(hls, res, COLOR_HLS2BGR_FULL);
    return res;
}

//---------------------------------------------------------------------------

void ver_mat_sat_lum(int nfoto, double sat, double lum, int matiz, bool guardar) {

    Mat res = op_mat_sat_lum(foto[nfoto].img, sat, lum, matiz);

    if(guardar) {
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    } else {
        imshow(foto[nfoto].nombre, res);
    }

}

//---------------------------------------------------------------------------

void ajustar_rojo_verde_azul(int nfoto, double* producto, int* suma, int num_canales, bool guardar) {
    Mat imagen = foto[nfoto].img.clone();
    Mat canales[num_canales];

    split(imagen, canales);
    for(int i = 0; i < num_canales; i++) {
        canales[i] = canales[i].mul(producto[i]) + suma[i];
    }
    merge(canales, num_canales, imagen);

    if(guardar) {
        imagen.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
    imshow(foto[nfoto].nombre, imagen);
}

//---------------------------------------------------------------------------

void rotar_y_reescalar(int nfoto, double angulo, double escala, bool guardar) {
    Mat imagen = foto[nfoto].img.clone();

    int anchoImagen = imagen.cols;
    int altoImagen = imagen.rows;

    // Calcular el seno y el coseno del angulo (pasar a radianes)
    double coseno = cos(angulo * CV_PI / 180);
    double seno = sin(angulo * CV_PI / 180);

    // Calcular el centro de la imagen despues de la rotacion
    double cx = -anchoImagen/2 * coseno - altoImagen/2 * seno;
    double cy = anchoImagen/2 * seno - altoImagen/2 * coseno;

    coseno = fabs(coseno);
    seno = fabs(seno);

    // Calcular el tamaño de la imagen rotada
    Size tam((anchoImagen * coseno + altoImagen * seno) * escala, (altoImagen * coseno + anchoImagen * seno) * escala);

    Mat M = getRotationMatrix2D(Point2f(0, 0), angulo, escala);
    M.at<double>(0, 2) = tam.width/2 + cx*escala;
    M.at<double>(1, 2) = tam.height/2 + cy*escala;

    warpAffine(imagen, imagen, M, tam);

    if(guardar) {
        imagen.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }

    imshow(foto[nfoto].nombre, imagen);

}

//---------------------------------------------------------------------------

void convertir_a_falso(int nfoto, int colorMap, bool guardar) {
    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);
    applyColorMap(gris, gris, colorMap);
    if(guardar) {
        gris.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
    imshow(foto[nfoto].nombre, gris);
}

//---------------------------------------------------------------------------

void cambiar_modelo_color(int nfoto, int nres, int modelo) {
    Mat res;
    cvtColor(foto[nfoto].img, res, modelo);
    crear_nueva(nres, res);
}

//---------------------------------------------------------------------------

void ver_espectro(int nfoto, int nres) {
    // Convertir la imagen a gris
    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);

    Mat escala;
    gris.convertTo(escala, CV_32FC1, 1.0/255);

    // Obtener la transformada de fourier
    Mat fourier;
    dft(escala, fourier, DFT_COMPLEX_OUTPUT);

    // Centrar la transformada
    centrarFourier(fourier);

    // Calcular la magnitud de la transformada de Fourier
    vector<Mat> canales;
    split(fourier, canales);
    magnitude(canales[0], canales[1], fourier);

    Mat res;
    fourier.convertTo(res, CV_8UC1, -1, 255);
    crear_nueva(nres, res);

}

//---------------------------------------------------------------------------

void ver_perfilado(int nfoto, int radio, double porcentaje, bool guardar) {

    Mat imagen = foto[nfoto].img.clone();
    Mat laplaciana;
    Laplacian(imagen, laplaciana, CV_16S, radio);
    imagen.convertTo(imagen, CV_16S);

    Mat res;
    addWeighted(imagen, 1.0, laplaciana, -porcentaje, 0, res);
    res.convertTo(res, CV_8U);

    if(guardar) {
        res.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }
    imshow(foto[nfoto].nombre, res);

}

//---------------------------------------------------------------------------

void ponerTextoEnImagen(int nfoto, string texto, int tam, Scalar color, int x, int y, bool guardar) {
    Mat imagen = foto[nfoto].img.clone();

    // Creamos una mascara del tamaño de la imagen, obtenemos la escala del texto y añadimos el texto en blanco
    Mat mascara(imagen.size(), CV_8UC3, Scalar::all(0));
    double escala = getEscalaTexto(texto, tam);
    putText(mascara, texto, Point(x + 5, y + (tam/2)), FONT_HERSHEY_PLAIN, escala, Scalar::all(255), 2);

    // Aplicamos un suavizado Gaussiano a la mascara
    Mat suavizado;
    GaussianBlur(mascara, suavizado, Size(3, 3), 0.0);

    // Aplicamos un desplazamiento (transformacion afín)
    Mat matrizDesplazamiento = (Mat_<float>(2, 3) << 1, 0, 4, 0, 1, 4);
    warpAffine(suavizado, suavizado, matrizDesplazamiento, mascara.size());

    Mat resultado = suavizado + mascara; // Mascara que contiene el texto y la sombra
    imagen = imagen + resultado; // Añade el texto y su sombra a la imagen

    Mat resultadoInversa;
    bitwise_not(resultado, resultadoInversa);

    multiply(imagen, resultadoInversa, imagen, 1.0/255);
    putText(imagen, texto, Point(x + 5, y + (tam/2)), FONT_HERSHEY_PLAIN, escala, color, 2);
    addWeighted(imagen, 0.5, foto[nfoto].img, 0.5, 0.0, imagen);

    if(!guardar) {
        circle(imagen, Point(x, y), 5, Scalar(0, 0, 0), -1, LINE_AA);
    } else {
        qDebug() << "Imagen guardada";
        imagen.copyTo(foto[nfoto].img);
        foto[nfoto].modificada = true;
    }

    qDebug() << "Mostrar foto";
    imshow(foto[nfoto].nombre, imagen);
}

//---------------------------------------------------------------------------

void ver_convolucion(int nfoto, int nres, Mat M, double mult, double suma, bool guardar) {

    Mat res;
    M *= mult;
    filter2D(foto[nfoto].img, res, -1, M, Point(-1, -1), suma, BORDER_REFLECT);

    if(guardar) {
        crear_nueva(nres, res);
    } else {
        imshow("Convolucion", res);
    }

}

//---------------------------------------------------------------------------

Mat aplicarMorfologia(Mat &entrada, int nres, int operacion, int tam, int iteraciones, bool guardar) {
    Mat imagen = entrada.clone();
    Mat elemento = getStructuringElement(MORPH_RECT, Size(2*tam+1, 2*tam+1), Point(tam, tam));
    morphologyEx(imagen, imagen, operacion, elemento, Point(-1, -1), iteraciones);

    if(guardar) {
        crear_nueva(nres, imagen);
    } else {
        imshow("Morfologia", imagen);
    }
    return imagen;
}

//---------------------------------------------------------------------------

void balanceBlancos(int nfoto, int nres) {
    Mat imagen = foto[nfoto].img.clone();
    imagen.convertTo(imagen, CV_32F, 1.0/255.0);
    cvtColor(imagen, imagen, COLOR_BGR2Lab); // Trabajamos en el espacio de color Lab

    double media_canal_a, media_canal_b;
    calcularMediaLAB(imagen, media_canal_a, media_canal_b);
    desplazarCanalesLAB(imagen, -media_canal_a, -media_canal_b);

    // Convertimos de Lab a BGR y de 32F a 8U
    cvtColor(imagen, imagen, COLOR_Lab2BGR);
    imagen.convertTo(imagen, CV_8U, 255.0);

    crear_nueva(nres, imagen);

}

//---------------------------------------------------------------------------

void aplicarInpaint(int nfoto, int nres, Mat &mascara, int radio) {
    Mat imagen = foto[nfoto].img;
    Mat res;
    inpaint(imagen, mascara, res, radio, INPAINT_TELEA);
    crear_nueva(nres, res);
}

//---------------------------------------------------------------------------

void ver_histograma2D(int nfoto, int nres, int celdas, canales_histograma2D canales, bool guardar) {

    // Obtener la imagen en la que se va a dibujar el histograma
    QString rutaImagnes[3] = {":/imagenes/Histograma_R-G.png", ":/imagenes/Histograma_R-B.png", ":/imagenes/Histograma_G-B.png"};
    QString ruta = rutaImagnes[canales];
    QImage imq = QImage(ruta);
    Mat img(imq.height(), imq.width(), CV_8UC4, imq.scanLine(0));
    cvtColor(img, img, COLOR_RGBA2RGB);

    // Cambiar de el espacio de color de la imagen activa de BGR a RGB (Realmente no es necesario)
    Mat imagen = foto[nfoto].img.clone();
    cvtColor(imagen, imagen, COLOR_BGR2RGB);

    // Calculo del histograma
    Mat hist;
    int bins[] = {celdas, celdas};
    float rango[] = {0, 256};
    const float *rangos[] = {rango, rango};
    int canalesHistograma[2];
    switch (canales) {
        case R_G:
            canalesHistograma[0] = 0;
            canalesHistograma[1] = 1;
        break;

        case R_B:
            canalesHistograma[0] = 0;
            canalesHistograma[1] = 2;
        break;

        case G_B:
            canalesHistograma[0] = 1;
            canalesHistograma[1] = 2;
        break;
    }

    calcHist(&imagen, 1, canalesHistograma, noArray(), hist, 2, bins, rangos);

    // Dibujar el histograma
    Point esquinaInfIzq(55, 429);
    Point esquinaSupDer(473, 11);

    int anchoHistograma = esquinaSupDer.x - esquinaInfIzq.x;
    int altoHistograma = esquinaInfIzq.y - esquinaSupDer.y;

    float anchoCelda = static_cast<float>(anchoHistograma)/celdas;
    float altoCelda = static_cast<float>(altoHistograma)/celdas;

    double min, max;
    minMaxLoc(hist, &min, &max);

    for(int x = 0; x < celdas; x++) {
        for(int y = 0; y < celdas; y++) {
            float valorHistograma = hist.at<float>(x, y);
            int color = cvRound(255 - (valorHistograma*255/max));
            rectangle(img, Point(esquinaInfIzq.x + x * anchoCelda, esquinaInfIzq.y - (y + 1) * altoCelda),
                                          Point(esquinaInfIzq.x + (x + 1) * anchoCelda - 1, esquinaInfIzq.y - y * altoCelda - 1),
                                          Scalar(color, color, color), FILLED);
        }
    }

    if(guardar) {
        crear_nueva(nres, img);
    } else {
        imshow("Histograma_2D", img);
    }
}

//---------------------------------------------------------------------------

void ecualizar_histograma_independiente(int nfoto, int nres) {
    Mat imagen = foto[nfoto].img.clone();
    vector<Mat> canales;
    split(imagen, canales);
    for(int i = 0; i < canales.size(); i++) {
        equalizeHist(canales[i], canales[i]);
    }
    merge(canales, imagen);
    crear_nueva(nres, imagen);
}

//---------------------------------------------------------------------------

void ecualizar_histograma_conjunta(int nfoto, int nres) {
    Mat gris;
    cvtColor(foto[nfoto].img, gris, COLOR_BGR2GRAY);

    // Calcular el histograma de la foto en grises
    int canales[] = {0};
    int bins[] = {256};
    float rango[] = {0 ,256};
    const float* rangos[] = {rango, rango};
    Mat hist;
    calcHist(&gris, 1, canales, noArray(), hist, 1, bins, rangos);

    // Normalizar el histograma
    hist *= 255.0/norm(hist, NORM_L1);

    // Crear la tabla LUT e inicializarla
    Mat lut(1, 256, CV_8UC1);
    float acum = 0.0;
    for(int i = 0; i < 256; i++) {
        lut.at<uchar>(0, i) = acum;
        acum += hist.at<float>(i);
    }

    Mat res;
    LUT(foto[nfoto].img, lut, res);
    crear_nueva(nres, res);
}

//---------------------------------------------------------------------------

void media_ponderada (int nf1, int nf2, int nueva, double peso)
{
    assert(nf1>=0 && nf1<MAX_VENTANAS && foto[nf1].usada);
    assert(nf2>=0 && nf2<MAX_VENTANAS && foto[nf2].usada);
    assert(nueva>=0 && nueva<MAX_VENTANAS && !foto[nueva].usada);
    Mat img= foto[nf1].img.clone();
    Mat cop;
    resize(foto[nf2].img, cop, img.size());
    addWeighted(img, peso, cop, 1.0-peso, 0, img);
    crear_nueva(nueva, img);
}

//---------------------------------------------------------------------------

void ecualizar_histograma_local(int nfoto, int nres, int radio, bool guardar) {
    // Convertir de BGR a Lab
    Mat imagen = foto[nfoto].img;
    Mat imagen_Lab;
    cvtColor(imagen, imagen_Lab, COLOR_BGR2Lab);

    // Obtener el canal L (luminosidad)
    vector<Mat> canales_Lab;
    split(imagen_Lab, canales_Lab);
    Mat canal_L = canales_Lab[0];
    Mat clon_canal_L = canal_L.clone(); //Guardar los las modificaciones

    for(int y = 0; y < imagen.rows; y++) {
        for(int x = 0; x < imagen.cols; x++) {
            int x_inicio = std::max(0, x-radio);
            int x_final = std::min(imagen.cols - 1, x+radio);
            int y_inicio = std::max(0, y-radio);
            int y_final = std::min(imagen.rows - 1, y + radio);

            Rect region_rect(Point(x_inicio, y_inicio), Point(x_final + 1, y_final + 1)); // Asegurarse de incluir el pixel final
            Mat region = canal_L(region_rect);

            // Calcular el histograma
            int canales[] = {0}; // Canal L
            int bins[] = {256};
            float rango[] = {0, 256};
            const float* rangos[] = {rango};
            Mat hist;
            calcHist(&region, 1, canales, noArray(), hist, 1, bins, rangos);

            hist /= sum(hist)[0]; // Normalizar el histograma

            float percentil = 0.0;
            int valor_pixel = canal_L.at<uchar>(y, x);
            for(int i = 0; i <= valor_pixel; i++) {
                percentil += hist.at<float>(i);
            }

            clon_canal_L.at<uchar>(y, x) = saturate_cast<uchar>(255*percentil);
        }

    }

    clon_canal_L.copyTo(canal_L);
    merge(canales_Lab, imagen_Lab);

    Mat res;
    cvtColor(imagen_Lab, res, COLOR_Lab2BGR);
    if(guardar)
        crear_nueva(nres, res);
    else
        imshow("Ecualizacion Local", res);
}

//---------------------------------------------------------------------------

void ecualizar_histograma_local_CLAHE(int nfoto, int nres, int radio, bool guardar) {

    Mat imagen_Lab;
    cvtColor(foto[nfoto].img, imagen_Lab, COLOR_BGR2Lab);
    vector<Mat> canales;
    split(imagen_Lab, canales);

    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2.0);
    clahe->setTilesGridSize(Size(2*radio+1, 2*radio+1));
    clahe->apply(canales[0], canales[0]);

    merge(canales, imagen_Lab);
    Mat res;
    cvtColor(imagen_Lab, res, COLOR_Lab2BGR);
    if(guardar)
        crear_nueva(nres, res);
    else
        imshow("Ecualizacion Local", res);
}

//---------------------------------------------------------------------------

string Lt1(string cadena)
{
    QString temp= QString::fromUtf8(cadena.c_str());
    return temp.toLatin1().data();
}

//---------------------------------------------------------------------------

void centrarFourier(Mat &fourier) {

    // Calcular el centro de la imagen
    int cx = fourier.cols / 2;
    int cy = fourier.rows / 2;

    Mat q0(fourier, Rect(0, 0, cx, cy));   // Cuadrante sup-izq
    Mat q1(fourier, Rect(cx, 0, cx, cy));  // Cuadrante sup-der
    Mat q2(fourier, Rect(0, cy, cx, cy));  // Cuadrante inf-izq
    Mat q3(fourier, Rect(cx, cy, cx, cy)); // Cuadrante inf-der

    // Intercambiar cuadrantre sup-izq por inf-der
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    // Intermcambiar cuadrante sup-der por inf-izq
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

//---------------------------------------------------------------------------

double getEscalaTexto(string texto, int tam) {

    Size tamTexto = getTextSize(texto, FONT_HERSHEY_PLAIN, 1.0, 2, nullptr);
    double escala = static_cast<double>(tam) / min(tamTexto.width, tamTexto.height);
    return escala;
}

//---------------------------------------------------------------------------

void calcularMediaLAB(Mat &imagen, double &media_a, double &media_b) {
    vector<Mat> canales;
    split(imagen, canales);

    media_a = mean(canales[1])[0];
    media_b = mean(canales[2])[0];

}

//---------------------------------------------------------------------------

void desplazarCanalesLAB(Mat &imagen, double des_a, double des_b) {
    for (int y = 0; y < imagen.rows; y++) {
        for (int x = 0; x < imagen.cols; x++) {
            Vec3f& lab = imagen.at<Vec3f>(y, x);
            double l = lab[0];
            double a_delta = des_a * (l / 100.0) * 1.1;
            double b_delta = des_b * (l / 100.0) * 1.1;
            lab[1] += a_delta;
            lab[2] += b_delta;
        }
    }
}
