#include "perspectiva.h"
#include "ui_perspectiva.h"
#include "imagenes.h"

int corresp[MAX_VENTANAS];
int nfoto1, nfoto2;
Point2f puntos[2][4];

void pintar_esquinas() {
    Mat copia = foto[nfoto1].img.clone();
    for(int i = 0; i < 4; i++) {
        line(copia, puntos[0][i], puntos[0][(i+1)%4], CV_RGB(0, 0, 0), 2);
    }
    for(int i = 0; i < 4; i++) {
        circle(copia, puntos[0][i], 10, CV_RGB(255, 0, 0), -1);
    }
    imshow("Origen", copia);
}

void callback_pers(int evento, int x, int y, int flags, void * pt) {
    int npos = (int)pt;
    if(flags == EVENT_FLAG_LBUTTON) {
        int dmin = 200;
        int posmin = 0;
        for(int i = 0; i < 4; i++) {
            int dact = abs(x - puntos[npos][i].x) + abs(y - puntos[npos][i].y);
            if(dact < dmin) {
                dmin = dact;
                posmin = i;
            }
        }

        if(dmin < 200) {
            puntos[npos][posmin] = Point2f(x, y);
            if(npos == 0)
                pintar_esquinas();
            ver_perspectiva(nfoto1, nfoto2, puntos[0], puntos[1]);
        }

    }
}

perspectiva::perspectiva(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::perspectiva)
{
    ui->setupUi(this);
    int pos = 0;
    for(int i = 0; i < MAX_VENTANAS; i++) {
        if(foto[i].usada) {
            ui->listWidget->addItem(QString::fromStdString(foto[i].nombre));
            ui->listWidget_2->addItem(QString::fromStdString(foto[i].nombre));
            corresp[pos++] = i;
        }
    }
    nfoto2 = foto_activa();
    nfoto1 = corresp[0];

    int W = foto[nfoto1].img.cols;
    int H = foto[nfoto1].img.rows;
    puntos[0][0] = Point2f(0, 0);
    puntos[0][1] = Point2f(W, 0);
    puntos[0][2] = Point2f(W, H);
    puntos[0][3] = Point2f(0, H);

    W = foto[nfoto2].img.cols;
    H = foto[nfoto2].img.rows;
    puntos[1][0] = Point2f(0, 0);
    puntos[1][1] = Point2f(W, 0);
    puntos[1][2] = Point2f(W, H);
    puntos[1][3] = Point2f(0, H);

    pintar_esquinas();
    ver_perspectiva(nfoto1, nfoto2, puntos[0], puntos[1]);
    setMouseCallback("Origen", callback_pers, 0);
    setMouseCallback("Perspectiva", callback_pers, (void*)1);
}

perspectiva::~perspectiva()
{
    delete ui;
}

void perspectiva::on_listWidget_currentRowChanged(int currentRow)
{
    double propX = double(foto[corresp[currentRow]].img.cols)/foto[nfoto1].img.cols;
    double propY = double(foto[corresp[currentRow]].img.rows)/foto[nfoto1].img.rows;
    for(int i = 0; i < 4; i++) {
        puntos[0][i].x *= propX;
        puntos[0][i].y *= propY;
    }

    nfoto1 = corresp[currentRow];
    pintar_esquinas();
    ver_perspectiva(nfoto1, nfoto2, puntos[0], puntos[1]);
}

void perspectiva::on_listWidget_2_currentRowChanged(int currentRow)
{
    double propX = double(foto[corresp[currentRow]].img.cols)/foto[nfoto2].img.cols;
    double propY = double(foto[corresp[currentRow]].img.rows)/foto[nfoto2].img.rows;
    for(int i = 0; i < 4; i++) {
        puntos[1][i].x *= propX;
        puntos[1][i].y *= propY;
    }
    nfoto2 = corresp[currentRow];
    ver_perspectiva(nfoto1, nfoto2, puntos[0], puntos[1]);
}

void perspectiva::on_perspectiva_rejected()
{
    destroyWindow("Origen");
    destroyWindow("Perspectiva");
}

void perspectiva::on_perspectiva_accepted()
{
    ver_perspectiva(nfoto1, nfoto2, puntos[0], puntos[1], true);
    mostrar(nfoto2);
    destroyWindow("Origen");
    destroyWindow("Perspectiva");
}
