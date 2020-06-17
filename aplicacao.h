#ifndef APLICACAO_H
#define APLICACAO_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QList>
#include <QTextStream>

#include "graphics_scene.h"

namespace Ui {
class aplicacao;
}

class aplicacao : public QMainWindow
{
    Q_OBJECT

public:
    explicit aplicacao(QWidget *parent = 0);
    ~aplicacao();



private slots:
    void on_actionAbrir_triggered();

    void on_actionFechar_triggered();

    bool loadFile(const QString &fileName);

    void setImage(const QImage &newImage);

    void on_RGB_clicked();

    void Mouse_corrent_pos();

    void on_linha_clicked();

    void on_apagar_clicked();

    void on_apagar_tudo_clicked();

    void on_nova_clicked();

    void on_guardar_clicked();

    void Visualizar_imagem_final(const QPixmap &);

    void resizeEvent(QResizeEvent *event);

signals:
    void linha();

    void apagar();

    void apagar_tudo();

    void nova();

    void guardar(const QPixmap &);

private:
    Ui::aplicacao *ui;
    QImage imagem;
    QGraphicsPixmapItem* item;
    QGraphicsPixmapItem* item_final;
    Graphics_Scene* scene;
    Graphics_Scene* new_scene;
    bool ok_1, ok_2;
    int RGB;

};

#endif // aplicacao_H
