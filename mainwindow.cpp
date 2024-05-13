#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QDebug>

#include <opencv2/opencv.hpp>
using namespace cv;

#include "acercade.h"
#include "brillocontraste.h"
#include "dialognueva.h"
#include "imagenes.h"
#include "mediadevideo.h"
#include "mediaponderada.h"
#include "rotaravideo.h"
#include "suavizados.h"
#include "video.h"
#include "capturarvideo.h"
#include "ajustelineal.h"
#include "bajorrelieve.h"
#include "pinchar_estirar.h"
#include "mat_sat_lum.h"
#include "convolucion.h"
#include "perspectiva.h"
#include "copiar_efectos.h"
#include "informacionimagen.h"
#include "rojo_verde_azul.h"
#include "rotar_reescalar.h"
#include "convertir_color_falso.h"

QString FiltroImagen = "Todos los formatos (*.jpg *.jpeg *.jpe .jp2 *.tif *.tiff *.png *.gif *.bmp *.dib *.webp *.ppm);;Archivos JPG (*.jpg *.jpeg *.jpe);;Archivos TIF (*.tif *.tiff);;Archivos PNG (*.png);;Archivos GIF (*.gif);;Archivos BMP (*.bmp *.dib);;Otros (*.*)";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show()
{
    QMainWindow::show();
    move(x(), 0);
}

void MainWindow::setStatusBarText(QString cadena)
{
    ui->statusBar->showMessage(cadena, 0);
}

int MainWindow::comprobar_primera_libre (void)
{
    int pl= primera_libre();
    if (pl == -1)
        QMessageBox::warning(this, "Error al crear imagen",
                             "Lo siento. No se pueden crear más ventanas.");
    return pl;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionSalir_triggered()
{
    close();
}

void MainWindow::on_actionNueva_imagen_triggered()
{
    int pl= comprobar_primera_libre();
    if (pl != -1) {
        DialogNueva nueva(pl, this);
        nueva.exec();
    }
}

void MainWindow::on_actionAbrir_imagen_triggered()
{
    int pl= comprobar_primera_libre();
    if (pl != -1) {
        QString nombre= QFileDialog::getOpenFileName(this, "Abrir imagen", ".", FiltroImagen);
        if (!nombre.isEmpty())
            crear_nueva(pl, nombre.toUtf8().data());
    }
}

void MainWindow::on_actionPunto_triggered()
{
    herr_actual= HER_PUNTO;
    ui->toolButton_4->setChecked(true);
    liberar_copia();
}

void MainWindow::on_actionLinea_triggered()
{
    herr_actual= HER_LINEA;
    ui->toolButton_5->setChecked(true);
    liberar_copia();
}

void MainWindow::on_actionSeleccionar_triggered()
{
    herr_actual= HER_SELECCION;
    ui->toolButton_6->setChecked(true);
    liberar_copia();
}

void MainWindow::on_toolButton_2_clicked()
{
    on_actionNueva_imagen_triggered();
}

void MainWindow::on_pushButton_clicked()
{
    QColor color= QColorDialog::getColor();
    if (color.isValid()) {
        QString estilo= "background-color: rgb(";
        estilo+= QString::number(color.red())+",";
        estilo+= QString::number(color.green())+",";
        estilo+= QString::number(color.blue())+")";
        ui->pushButton->setStyleSheet(estilo);
        color_pincel= CV_RGB(color.red(), color.green(), color.blue());
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    radio_pincel= value;
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    difum_pincel= value;
}

void MainWindow::on_toolButton_clicked()
{
    on_actionAbrir_imagen_triggered();
}

void MainWindow::on_actionGuardar_triggered()
{
    int fa= foto_activa();
    if (fa != -1)
        guardar_foto(fa);
}

void MainWindow::on_actionGuardar_como_triggered()
{
    int fa= foto_activa();
    if (fa != -1) {
        QString nombre= QFileDialog::getSaveFileName(this, "Guardar imagen", QString::fromStdString(foto[fa].nombre), FiltroImagen);
        if (!nombre.isEmpty())
            guardar_foto(fa, nombre.toUtf8().data());
    }
}

void MainWindow::on_actionCerrar_triggered()
{
    int fa= foto_activa();
    if (fa != -1) {
        if (foto[fa].modificada && preguntar_guardar) {
            QString nombre= QString::fromStdString(foto[fa].nombre);
            nombre= "El archivo " + nombre + " ha sido modificado.\n¿Desea guardarlo?";
            int resp= QMessageBox::question(this, "Archivo modificado", nombre,
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (resp==QMessageBox::Yes)
                guardar_foto(fa);
            else if (resp==QMessageBox::Cancel)
                return;
            else
                foto[fa].modificada= false;
        }
        cerrar_foto(fa);
    }
}

void MainWindow::on_actionPreguntar_si_guardar_triggered()
{
    preguntar_guardar= ui->actionPreguntar_si_guardar->isChecked();
}

void MainWindow::on_actionColor_del_pincel_triggered()
{
    on_pushButton_clicked();
}

void MainWindow::on_actionSeleccionar_todo_triggered()
{
    int fa= foto_activa();
    if (fa != -1) {
        foto[fa].roi= Rect(0, 0, foto[fa].img.cols, foto[fa].img.rows);
        mostrar(fa);
    }
}

void MainWindow::on_toolButton_3_clicked()
{
    on_actionGuardar_triggered();
}

void MainWindow::on_toolButton_4_clicked()
{
    herr_actual= HER_PUNTO;
    liberar_copia();
}

void MainWindow::on_toolButton_5_clicked()
{
    herr_actual= HER_LINEA;
    liberar_copia();
}

void MainWindow::on_toolButton_6_clicked()
{
    herr_actual= HER_SELECCION;
    liberar_copia();
}

void MainWindow::on_actionInvertir_triggered()
{
    int fa= foto_activa();
    if (fa != -1) {
        int pl= comprobar_primera_libre();
        if (pl != -1)
            invertir(fa, pl);
    }
}

void MainWindow::on_actionRotar_90_triggered()
{
    int fa= foto_activa();
    if (fa != -1) {
        int pl= comprobar_primera_libre();
        if (pl != -1)
            rotar_exacto(fa, pl, 1);
    }
}

void MainWindow::on_actionRotar_180_triggered()
{
    int fa= foto_activa();
    if (fa != -1) {
        int pl= comprobar_primera_libre();
        if (pl != -1)
            rotar_exacto(fa, pl, 2);
    }
}

void MainWindow::on_actionRotar_91_triggered()
{
    int fa= foto_activa();
    if (fa != -1) {
        int pl= comprobar_primera_libre();
        if (pl != -1)
            rotar_exacto(fa, pl, 3);
    }
}

void MainWindow::on_actionBrillo_contraste_triggered()
{
    if (foto_activa() != -1) {
        brillocontraste bc(foto_activa(), this);
        bc.exec();
    }
}

void MainWindow::on_actionGausiano_triggered()
{
    if (foto_activa() !=- 1) {
        suavizados sg(foto_activa(), 1, this);
        sg.exec();
    }
}

void MainWindow::on_actionMedia_triggered()
{
    if (foto_activa() != -1) {
        suavizados sg(foto_activa(), 2, this);
        sg.exec();
    }
}

void MainWindow::on_actionMedia_ponderada_triggered()
{
    if (foto_activa() != -1) {
        int pl= comprobar_primera_libre();
        if (pl != -1) {
            mediaponderada mp(this);
            mp.exec();
        }
    }
}

void MainWindow::on_actionRotar_imagen_triggered()
{
    if (foto_activa() != -1) {
        rotaravideo rv;
        rv.exec();
    }
}

void MainWindow::on_actionAcerca_de_triggered()
{
    acercade ad;
    ad.exec();
}

void MainWindow::on_actionImagen_media_triggered()
{
    mediadevideo mv(this);
    mv.exec();
}

void MainWindow::on_toolButton_7_clicked()
{
    herr_actual = HER_RECTANGULO;
    liberar_copia();
}

void MainWindow::on_actionRect_ngulo_triggered()
{
    herr_actual = HER_RECTANGULO;
    ui->toolButton_7->setChecked(true);
    liberar_copia();
}

void MainWindow::on_toolButton_8_clicked()
{
    herr_actual = HER_ELIPSE;
    liberar_copia();
}

void MainWindow::on_actionElipse_triggered()
{
    herr_actual = HER_ELIPSE;
    ui->toolButton_8->setChecked(true);
    liberar_copia();
}

void MainWindow::on_actionCopiar_a_nueva_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        copiar_a_nueva(foto_activa(), primera_libre());
    }
}

void MainWindow::on_actionGris_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        ver_histograma(foto_activa(), 3, primera_libre());
    }
}

void MainWindow::on_actionVerde_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        ver_histograma(foto_activa(), 2, primera_libre());
    }
}

void MainWindow::on_actionVerde_2_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        ver_histograma(foto_activa(), 1, primera_libre());
    }
}

void MainWindow::on_actionAzul_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        ver_histograma(foto_activa(), 0, primera_libre());
    }
}

void MainWindow::on_actionMediana_triggered()
{
    if(foto_activa() != -1) {
        suavizados sg(foto_activa(), 3, this);
        sg.exec();
    }
}

void MainWindow::on_actionCapturar_de_c_mara_triggered()
{
    capturar_de_camara();
}

void MainWindow::on_actionCapturar_de_v_deo_triggered()
{
    QString nombre = QFileDialog::getOpenFileName();
    if(!nombre.isEmpty()) {
        Capturarvideo cap(nombre.toLatin1().data(), this);
        if(cap.isOpened()) {
            cap.exec();
        }
    }
}

void MainWindow::on_toolButton_9_clicked()
{
    herr_actual = HER_ARCO_IRIS;
    liberar_copia();
}

void MainWindow::on_actionArco_Iris_triggered()
{
    herr_actual = HER_ARCO_IRIS;
    ui->toolButton_9->setChecked(true);
    liberar_copia();
}

void MainWindow::on_actionAjuste_lineal_triggered()
{
    if(foto_activa() != -1) {
        ajustelineal al(foto_activa());
        al.exec();
    }
}

void MainWindow::on_actionBajorrelieve_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        bajorrelieve br(foto_activa(), primera_libre());
        br.exec();
    }
}

void MainWindow::on_actionEscala_de_color_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        escala_color(foto_activa(), primera_libre());
    }
}

void MainWindow::on_actionPinchar_Estirar_triggered()
{
    if(foto_activa() != -1) {
        pinchar_estirar pe(foto_activa());
        pe.exec();
    }
}

void MainWindow::on_actionMatiz_Saturacion_Luminosidad_triggered()
{
    if(foto_activa() != -1) {
        mat_sat_lum msl(foto_activa());
        msl.exec();
    }
}

void MainWindow::on_actionConvolucion_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        convolucion co(foto_activa(), primera_libre());
        co.exec();
    }
}

void MainWindow::on_actionPerspectiva_triggered()
{
    if(foto_activa() != -1) {
        perspectiva pe;
        pe.exec();
    }
}

void MainWindow::on_actionCopiar_con_efectos_triggered()
{
    copiar_efectos ce;
    ce.exec();
}

void MainWindow::on_actionAbrir_imagen_del_portapapeles_triggered()
{
    if(primera_libre() != -1) {
        QClipboard *clipboard = QApplication::clipboard();
        QImage imagen = clipboard->image();

        if(imagen.isNull()) {
            QMessageBox::warning(nullptr, "Portapeles sin imagen", "No hay imagen en el portapapeles");
            return;
        }


        nueva_desde_portapaples(primera_libre(), imagen);

    }
}

void MainWindow::on_actionCopiar_al_portapapeles_triggered()
{
    if(foto_activa() == -1) {
        QMessageBox::warning(nullptr, "SIN FOTOS", "No se ha creado ninguna foto");
        return;
    }

    QImage imagen = copiar_al_portapapeles(foto_activa());
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(imagen);
}

void MainWindow::on_actionVer_informacion_triggered()
{
    if(foto_activa() == -1) {
        QMessageBox::warning(nullptr, "SIN FOTOS", "No se ha creado ninguna foto");
        return;
    }

    QVector<QString> informacion = ver_informacion(foto_activa());
    if(!informacion.empty()) {
        InformacionImagen infoImage(informacion);
        infoImage.exec();
    }

}

void MainWindow::on_toolButton_10_clicked()
{
    herr_actual = HER_TRAZO;
    liberar_copia();
}

void MainWindow::on_actionTrazo_triggered()
{
    herr_actual = HER_TRAZO;
    ui->toolButton_10->setChecked(true);
    liberar_copia();
}

void MainWindow::on_toolButton_11_clicked()
{
    herr_actual = HER_RELLENADO;
    liberar_copia();
}

void MainWindow::on_actionRellenar_triggered()
{
    herr_actual = HER_RELLENADO;
    ui->toolButton_11->setChecked(true);
    liberar_copia();
}

void MainWindow::on_toolButton_12_clicked()
{
    herr_actual = HER_SUAVIZADO;
    liberar_copia();
}

void MainWindow::on_actionSuavizado_triggered()
{
    herr_actual = HER_SUAVIZADO;
    ui->toolButton_12->setChecked(true);
    liberar_copia();
}

void MainWindow::on_toolButton_13_clicked()
{
    herr_actual = HER_COPIA;
}

void MainWindow::on_actionCopiar_triggered()
{
    herr_actual = HER_COPIA;
    ui->toolButton_13->setChecked(true);
}

void MainWindow::on_actionAjuste_Rojo_Verde_Azul_triggered()
{
    if(foto_activa() != -1) {
        rojo_verde_azul aj_rgb(foto_activa(), this);
        aj_rgb.exec();
    }
}

void MainWindow::on_actionRotar_y_Reescalar_triggered()
{
    if(foto_activa() != -1) {
        rotar_reescalar rot_res(foto_activa(), this);
        rot_res.exec();
    }
}

void MainWindow::on_actionConvertir_a_color_falso_triggered()
{
    if(foto_activa() != -1) {
        convertir_color_falso convAFalso(foto_activa(), this);
        convAFalso.exec();
    }
}

void MainWindow::on_actionRGB_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2RGB);
    }
}

void MainWindow::on_actionHLS_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2HLS);
    }
}

void MainWindow::on_actionHSV_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2HSV);
    }
}

void MainWindow::on_actionXYZ_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2XYZ);
    }
}

void MainWindow::on_actionYUV_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2YUV);
    }
}

void MainWindow::on_actionGris_2_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2GRAY);
    }
}

void MainWindow::on_actionLab_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2Lab);
    }
}

void MainWindow::on_actionLuv_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2Luv);
    }
}

void MainWindow::on_actionYCrCb_triggered()
{
    if(foto_activa() != -1 && primera_libre() != -1) {
        cambiar_modelo_color(foto_activa(), primera_libre(), COLOR_BGR2YCrCb);
    }
}
