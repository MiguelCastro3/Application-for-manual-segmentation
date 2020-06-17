#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QString>


class Graphics_Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Graphics_Scene();

    void mousePressEvent(QGraphicsSceneMouseEvent *mouse_event);

    int x, y;

public slots:
    void botao_linha();

    void botao_apagar();

    void botao_apagar_tudo();

    void botao_nova();

    void botao_guardar(const QPixmap &);

private:
    QString botao;
    QPoint ultimo;
    QVector<QGraphicsLineItem *> lista_linhas;
    QVector<QVector<QGraphicsLineItem *>> lista_total_linhas, uma_segmentacao;
    QVector<QPointF> lista_pontos;
    QVector<QVector<QPointF>> lista_total_pontos;

signals:
    void Mouse_pos();

    void ver_imagem_final(const QPixmap &);

};

#endif // GRAPHICS_SCENE_H
