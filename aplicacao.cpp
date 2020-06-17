#include <QtWidgets>
#include "aplicacao.h"
#include "ui_aplicacao.h"
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QtGlobal>
#include <QResizeEvent>


aplicacao::aplicacao(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::aplicacao)


{
    ui->setupUi(this);
    scene = new Graphics_Scene(); //criação da cena
    ui->graph_view->setScene(scene);  //colocação da cena dentro da graphics_view inserida

    connect(scene, SIGNAL(Mouse_pos()), this, SLOT(Mouse_corrent_pos())); //envia o sinal do rato para a função pretendida, para devolver x e y
    connect(this, SIGNAL(linha()), scene, SLOT(botao_linha()));
    connect(this, SIGNAL(apagar()), scene, SLOT(botao_apagar()));
    connect(this, SIGNAL(apagar_tudo()), scene, SLOT(botao_apagar_tudo()));
    connect(this, SIGNAL(nova()), scene, SLOT(botao_nova()));
    connect(this, SIGNAL(guardar(QPixmap)), scene, SLOT(botao_guardar(QPixmap)));
    connect(scene, SIGNAL(ver_imagem_final(QPixmap)), this, SLOT(Visualizar_imagem_final(QPixmap)));
}


aplicacao::~aplicacao()
{
    delete ui;
}


void aplicacao::on_actionAbrir_triggered()
{
    QFileDialog dialog(this, tr("Open File"));
    while (dialog.exec()==QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {
    }

    RGB = 1;
}


void aplicacao::on_actionFechar_triggered()
{
    QCoreApplication::quit();
}


bool aplicacao::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    ui->graph_view->setVisible(true); //se existir uma imagem a ser importada então passa a ser visível
    setImage(newImage);
    setWindowFilePath(fileName);

    //quando carregar uma nova imagem "eliminar" as imagens por canais anteriores
    ui->label_red->clear();
    ui->label_green->clear();
    ui->label_blue->clear();

    return true;
}


void aplicacao::setImage(const QImage &newImage)
{
    if  ((newImage.allGray())){ui->RGB->setEnabled(false);} //verificar se todos os pixeis são cinzentos
    else {ui->RGB->setEnabled(true);}                       //se sim, possibilita o botão RGB, dividir a imagem nos 3 canais

    scene->clear(); //quando carregar uma nova imagem "eliminar" a imagem anterior
    //new_scene->clear(); //ESTÁ A DAR ERRO -> "The program has unexpectedly finished. The process was ended forcefully."
    ui->graph_seg->setEnabled(false);
    ui->graph_view->viewport()->update(); //carregar nova imagem

    imagem = newImage;
    item = new QGraphicsPixmapItem(QPixmap::fromImage(imagem));
    scene->addItem(item);
    ok_1 = 1; //criação de uma variavel de estado para redimensionamento

    ui->graph_view->fitInView(item,Qt::KeepAspectRatio); //para que a view apresenta as mesmas dimensões da imagem, mantendo a relação original

    //ativar botões para interação com a imagem
    ui->linha->setEnabled(true);
    ui->apagar->setEnabled(true);
    ui->apagar_tudo->setEnabled(true);
    ui->nova->setEnabled(true);
    ui->guardar->setEnabled(true);
}


void aplicacao::on_RGB_clicked() //conversão da imagem nos seus 3 canais RGB
{
    if (RGB == 1)
    {
        int red = 0, green = 0, blue = 0;
        int altura = imagem.QImage::height();
        int largura = imagem.QImage::width();

        QImage imagem_red = QPixmap(largura, altura).toImage();
        QImage imagem_green = QPixmap(largura, altura).toImage();
        QImage imagem_blue = QPixmap(largura, altura).toImage();

        for(int i = 0; i < altura ; i++)
            {
            QRgb *rowData = (QRgb*)imagem.scanLine(i);
            for(int j = 0; j < largura ; j++)
                {
                QRgb pixelData = rowData[j];
                red = qRed(pixelData);
                green = qGreen(pixelData);
                blue = qBlue(pixelData);
                QColor nova_imagem_red = QColor(red, red, red);
                QColor nova_imagem_green = QColor(green, green, green);
                QColor nova_imagem_blue = QColor(blue, blue, blue);
                imagem_red.setPixelColor(j, i, nova_imagem_red);
                imagem_green.setPixelColor(j, i, nova_imagem_green);
                imagem_blue.setPixelColor(j, i, nova_imagem_blue);
                }
            }

        //inserir os canais RGB nas respetivas labels
        ui->label_red->setPixmap(QPixmap::fromImage(imagem_red));
        ui->label_green->setPixmap(QPixmap::fromImage(imagem_green));
        ui->label_blue->setPixmap(QPixmap::fromImage(imagem_blue));

        RGB = 0; //criação de uma variavel de estado para alternar o botão RGB entre ter imagens e não ter
    }

    else if (RGB == 0)
    {
        //eliminar os canais RGB
        ui->label_red->clear();
        ui->label_green->clear();
        ui->label_blue->clear();

        RGB = 1; //criação de uma variavel de estado para alternar o botão RGB entre ter imagens e não ter
    }
}


void aplicacao::Mouse_corrent_pos()
{
    ui->x_y->setText(QString("X = %1, Y = %2").arg(scene->x).arg(scene->y));
}


void aplicacao::on_linha_clicked()
{
    emit(linha());
}


void aplicacao::on_apagar_clicked()
{
    emit(apagar());
}


void aplicacao::on_apagar_tudo_clicked()
{
    emit(apagar_tudo());
}


void aplicacao::on_nova_clicked()
{
    emit(nova());
}


void aplicacao::on_guardar_clicked()
{

    emit(guardar(QPixmap::fromImage(imagem)));
}

void aplicacao::Visualizar_imagem_final(const QPixmap &imagem_final)
{
    new_scene = new Graphics_Scene();
    ui->graph_seg->setScene(new_scene);

    item_final = new_scene->addPixmap(imagem_final);
    ui->graph_seg->viewport()->update(); //carregar nova imagem
    ui->graph_seg->setEnabled(true); //permissão para a imagem ser visualizada
    ui->graph_seg->fitInView(item_final,Qt::KeepAspectRatio); //para que a view apresenta as mesmas dimensões da imagem, mantendo a relação original
    ok_2 = 1;  //criação de uma variavel de estado para redimensionamento
}


void aplicacao::resizeEvent(QResizeEvent *event) //permite redimensionar as imagens de acordo com a janela da aplicação, nunca perdendo o rácio da imagem original
{
    QWidget::resizeEvent(event);
    if (ok_1)
    {
        ui->graph_view->fitInView(item, Qt::KeepAspectRatio);
    }
    if (ok_2)
    {
        ui->graph_seg->fitInView(item_final, Qt::KeepAspectRatio);
    }
}
