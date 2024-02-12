#include "copiar_efectos.h"
#include "ui_copiar_efectos.h"
#include <QFileDialog>
#include <QMessageBox>
#include "video.h"

copiar_efectos::copiar_efectos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copiar_efectos)
{
    ui->setupUi(this);
    modo = 0;
}

copiar_efectos::~copiar_efectos()
{
    delete ui;
}

void copiar_efectos::on_pushButton_clicked()
{
    QString nombre = QFileDialog::getOpenFileName(this, "Archivo de video", ".", QString::fromStdString(FiltroVideo));
    if(!cap.open(nombre.toLatin1().data())) {
        QMessageBox::information(this, "Error al abrir video", "No se ha podido abrir el archivo de video");

        ui->pushButton->setChecked(false);
    }
    ui->pushButton_2->setChecked(false);

}

void copiar_efectos::on_pushButton_2_clicked()
{
    if(!cap.open(ui->spinBox->value())) {
        QMessageBox::information(this, "Error al abrir la camara", "No se ha podido abrir la camara");
        ui->pushButton_2->setChecked(false);
    }
    ui->pushButton->setChecked(false);
}

void copiar_efectos::on_copiar_efectos_accepted()
{
    if(!cap.isOpened()) {
        QMessageBox::information(this, "Error de fuente de video", "No hay fuente de video seleccionada");
        return;
    }

    QString nombre = QFileDialog::getSaveFileName(this, "Archivo de video", ".", QString::fromStdString(FiltroVideo));
    if(!nombre.isEmpty()) {
        VideoWriter wri;
        double fps = cap.get(CAP_PROP_FPS);
        if(fps <= 0)
            fps = 25;
        Mat frame;
        if(cap.read(frame)) {
            if(!wri.open(nombre.toLatin1().data(), VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, frame.size())) {
                QMessageBox::information(this, "Error de salida de video", "No se ha podido crear el video" + nombre);
                return;
            }

            int matiz = ui->spinBox_2->value();
            double sat = ui->doubleSpinBox->value();
            double lum = ui->doubleSpinBox_2->value();
            int cx = ui->spinBox_4->value();
            int cy = ui->spinBox_3->value();
            double radio = ui->doubleSpinBox_3->value();
            double grado = ui->doubleSpinBox_4->value();
            int direccion = 1;
            while(waitKey(1) == -1 && cap.read(frame)) {
                Mat res;
                if(modo == 0) {
                    res = op_mat_sat_lum(frame, sat, lum, matiz);
                    if(ui->checkBox->isChecked()) {
                        matiz = (matiz + 4)%256;
                    }
                } else {
                    res = op_pinchar_estirar(frame, cx, cy, radio, grado*radio*radio/1000.0);
                    if(ui->checkBox_2->isChecked()) {
                        grado += direccion;
                        if(abs(grado) > 60) {
                            direccion = -direccion;
                        }
                    }
                }

                imshow("Frame", res);
                wri.write(res);
            }
            wri.release();
            destroyWindow("Frame");
        }

    }
}

void copiar_efectos::on_tabWidget_currentChanged(int index)
{
    modo = index;
}
