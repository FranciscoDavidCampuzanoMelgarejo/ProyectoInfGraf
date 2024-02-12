#ifndef COPIAR_EFECTOS_H
#define COPIAR_EFECTOS_H

#include <QDialog>
#include "imagenes.h"

namespace Ui {
class copiar_efectos;
}

class copiar_efectos : public QDialog
{
    Q_OBJECT

public:
    explicit copiar_efectos(QWidget *parent = nullptr);
    ~copiar_efectos();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_copiar_efectos_accepted();

    void on_tabWidget_currentChanged(int index);

private:
    VideoCapture cap;
    int modo;
    Ui::copiar_efectos *ui;
};

#endif // COPIAR_EFECTOS_H
