#ifndef PERSPECTIVA_H
#define PERSPECTIVA_H

#include <QDialog>

namespace Ui {
class perspectiva;
}

class perspectiva : public QDialog
{
    Q_OBJECT

public:
    explicit perspectiva(QWidget *parent = nullptr);
    ~perspectiva();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);

    void on_listWidget_2_currentRowChanged(int currentRow);

    void on_perspectiva_rejected();

    void on_perspectiva_accepted();

private:
    Ui::perspectiva *ui;
};

#endif // PERSPECTIVA_H
