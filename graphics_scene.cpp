#include "graphics_scene.h"
#include "qpainter.h"
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QGraphicsPixmapItem>

Graphics_Scene::Graphics_Scene()
{

}


void Graphics_Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouse_event) //função para detetar movimentos do rato na graphics scene
{
    QGraphicsScene::mousePressEvent(mouse_event);

    if (mouse_event->button() == Qt::LeftButton)
    {
        this->x = mouse_event->pos().toPoint().x(); //quando clica, indica as coordenadas no eixo x
        this->y = mouse_event->pos().toPoint().y(); //quando clica, indica as coordenadas no eixo y
        emit Mouse_pos();

        if (botao == "linha")
        {
            QPoint coordenadas = mouse_event->pos().toPoint();
            if (coordenadas.x() > 0 && coordenadas.y() > 0 ) //segmenta todos os pontos excepto o ponto (0,0)
            {
                if(ultimo.isNull()) {
                    ultimo = coordenadas;
                    lista_pontos << mouse_event->pos(); //adiciona primeiro ponto à lista de pontos
            }
                else
                {
                    QGraphicsLineItem *linha = addLine(QLine(ultimo, coordenadas), QPen(Qt::green, 3)); //definir a linha/reta a ser desenhada
                    lista_linhas << linha; //adiciona linhas à lista de linhas
                    lista_pontos << mouse_event->pos();
                    ultimo = coordenadas;
                }
            }
        }
    }
}


void Graphics_Scene::botao_linha()
{
    botao = "linha";
}


void Graphics_Scene::botao_apagar()
{
    int tamanho_lista = lista_linhas.size();
    if (tamanho_lista > 0 && items().size() > 1) //confirmar se existe alguma linha e se existem outro item na cena
    {
       removeItem(items().first()); //retirar a última linha/reta da lista da scene
       lista_linhas.removeLast(); //retirar a última linha/reta da lista
       lista_pontos.removeLast(); //retirar o último ponto inserido da lista
       ultimo = lista_pontos.last().toPoint(); //voltar a considerar o ultimo ponto da lista, não apagado, como ultimo inserido
    }
    else if (tamanho_lista == 0)
    {
        ultimo = QPoint();
    }
}


void Graphics_Scene::botao_apagar_tudo()
{
    lista_total_linhas << lista_linhas; //guardar todas as linhas num vetor
    lista_total_pontos << lista_pontos; //guardar todas os pontos num vetor

    for(int i = 0; i < lista_total_pontos.size() ; i++) //segmentar o número de segmentações feitas
    {
        int tamanho = lista_total_linhas[i].size();
        for (int j = 0; j < tamanho; j++)
        {
            removeItem(lista_total_linhas[i].last()); //retirar a última linha/reta da lista da scene
            lista_total_linhas[i].removeLast(); //retirar a última linha/reta da lista
        }
    }
    ultimo = QPoint(); //permitir definir um novo ponto inicial
    //apagar tudo, não existe nenhum ponto ou linha guardado
    lista_total_linhas.clear();
    lista_linhas.clear();
    lista_total_pontos.clear();
    lista_pontos.clear();
}


void Graphics_Scene::botao_nova()
{
    lista_total_linhas << lista_linhas; //guardar todas as linhas num vetor
    lista_total_pontos << lista_pontos; //guardar todas os pontos num vetor
    lista_linhas.clear();               //apagar todos os dados atuais nas primeiras listas
    lista_pontos.clear();
    ultimo = QPoint(); //permitir definir um novo ponto inicial
    botao = "linha";
}


void Graphics_Scene::botao_guardar(const QPixmap &imagem_original)
{
    bool ok; //botão ok da caixa de texto
    QString text;
    QString pasta = QInputDialog::getText(0, "Guardar segmentação", "Nome do ficheiro:",QLineEdit::Normal, "/home/mirre/Desktop/LIE II/Trabalho final/Resultados/", &ok);

    if (ok && !pasta.isEmpty()) //se o utilizador pressionar ok e o nome não estiver vazio...
    {
        QPixmap imagem_final = imagem_original; //cria nova imagem com as dimensoes da imagem original
        imagem_final.fill(Qt::black); //preencher com preto

        lista_total_linhas << lista_linhas; //guardar todas as linhas num vetor
        lista_total_pontos << lista_pontos; //guardar todas os pontos num vetor

        for(int i = 0; i < lista_total_pontos.size() ; i++) //segmentar o número de segmentações feitas
        {
            QPolygonF poligono(lista_total_pontos[i]); //cria um polígono fechado através dos pontos existentes na lista
            QPainter paint; //preencher a imagem
            paint.begin(&imagem_final); //associar o painter à imagem_final
            QPainterPath caminho;  //criar caminho
            caminho.addPolygon(poligono); //adicionar poligono
            caminho.closeSubpath();  //fechar caminho
            paint.setPen(QPen(QBrush(Qt::white), 3)); //pintar caminho de cor branco
            paint.drawPath(caminho);
            QBrush brush; //preenche caminho
            brush.setColor(Qt::white);
            brush.setStyle(Qt::SolidPattern);
            paint.fillPath(caminho, brush); //aplicar à imagem
            paint.end(); //terminar

            int tamanho = lista_total_linhas[i].size();
            for (int j = 0; j < tamanho; j++)
            {
                removeItem(lista_total_linhas[i].last()); //retirar a última linha/reta da lista da scene
                lista_total_linhas[i].removeLast(); //retirar a última linha/reta da lista
            }
        }

        ultimo = QPoint();

        QString destino_nome_final;
        QTextStream(&destino_nome_final) << pasta << ".png";

        imagem_final.save(destino_nome_final); //guardar imagem segmentada;
        emit(ver_imagem_final(imagem_final));

        lista_total_linhas.clear();
        lista_linhas.clear();
        lista_total_pontos.clear();
        lista_pontos.clear();

        //notificação
        QMessageBox notificacao;
        notificacao.setText("Segmentação concluída! Ficheiro guardado!");
        notificacao.exec();
    }
    else
    {
        //notificação
        QMessageBox notificacao;
        notificacao.setText("Operação cancelada!");
        notificacao.exec();
    }
}


