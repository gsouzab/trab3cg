#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QMessageBox>
#include <qcolordialog.h>
#include <stdlib.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->labelPrincipal->setMouseTracking(true);

    buffer = QImage(600,500, QImage::Format_ARGB32);

    painter = new QPainter(&buffer);

    buffer.fill(corFundo);

    corFundo = Qt::white;
    corCaneta = Qt::black;

    curveIndex = 0;
    pointIndex = 0;

    brushSize = 1;

    movingPoint = false;
    onThePath = false;

    this->interfaceUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

static const int pointSize = 5;

QString MainWindow::getFilePath() const
{
    return filePath;
}

void MainWindow::setFilePath(const QString &value)
{
    filePath = value;
}

void MainWindow::desenhaCurvas()
{
    QVector<Curva>::iterator iterator;

    for(iterator = curvas.begin(); iterator != curvas.end(); iterator++)
    {

        this->desenhaBezier(iterator->pontosControle);
    }
}


void MainWindow::on_fileDialogOpen_clicked()
{

    QString newFilePath = QFileDialog::getOpenFileName(this,
                                                       tr("Abrir o arquivo descritor"),
                                                       QDir::currentPath(),
                                                    tr("TXT Files (*.txt)"));

    if(!newFilePath.isEmpty())
    {
        this->setFilePath(newFilePath);

        this->leArquivoControle();

        this->interfaceUpdate();
    }
}

void MainWindow::interfaceUpdate(void)
{
    buffer.fill(corFundo);

    desenhaCurvas();

    ui->labelPrincipal->setPixmap(QPixmap::fromImage(buffer));
    update();
}

void MainWindow::desenhaBezier(QVector<QPoint> pontos)
{

    //Desenhando os pontos de controle
    for(int i=0;i<4;i++)
    {
        this->desenhaPonto(pontos.at(i));
    }

    //Desenhando as linhas entre os pontos de controle
    this->desenhaLinha(pontos.at(0),pontos.at(1));
    this->desenhaLinha(pontos.at(2),pontos.at(3));

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;

    //Desenhando a bezier cubica
    path.moveTo(pontos.at(0));
    path.cubicTo(pontos.at(1),pontos.at(2),pontos.at(3));

    painter->setPen(QPen(corCaneta, brushSize));
    painter->drawPath(path);

    painter->restore();
}

void MainWindow::desenhaPonto(const QPoint &pos)
{
    painter->save();

    painter->setPen(QColor(50, 100, 120, 200));
    painter->setBrush(QColor(200, 200, 210, 120));
    painter->drawEllipse(QRectF(pos.x() - (pointSize + (brushSize/2)),
                                pos.y() - (pointSize + (brushSize/2)),
                                (pointSize + (brushSize/2))*2, (pointSize + (brushSize/2))*2));
    painter->restore();
}

void MainWindow::desenhaLinha(const QPoint &inicio, const QPoint &fim)
{
    painter->save();
    QPen pen(QColor(255, 0, 255, 127), 1, Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(inicio, fim);
    painter->restore();
}

void MainWindow::leArquivoControle()
{
    QFile file(this->getFilePath());
    QString line;
    QStringList fields;
    Curva curva;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {

        line = in.readLine();

        if(line.compare("B") == 0)
        {
            QPoint ponto;

            curva.setTipoCurva(Curva::BEZIER);

            //lendo os 4 pontos de controle

            for(int i=0;i<4;i++)
            {
                line = in.readLine();

                fields = line.split(" ");
                ponto = QPoint(fields.at(0).toInt(),fields.at(1).toInt());
                curva.pontosControle.append(ponto);
            }

            curvas.append(curva);

            curva.pontosControle.clear();

        }
        else if(line.compare("H") == 0)
        {

        }

    }
}

void MainWindow::on_brushSizeSlider_valueChanged(int value)
{
    brushSize = (value/2);

    buffer.fill(corFundo);

    desenhaCurvas();
    interfaceUpdate();
}


void MainWindow::on_corCurvaBtn_clicked()
{
    corCaneta = QColorDialog::getColor(Qt::white,this);

    if(corCaneta.isValid())
    {
        buffer.fill(corFundo);

        desenhaCurvas();
        interfaceUpdate();
    }


}

void MainWindow::on_corFundoBtn_clicked()
{
    corFundo = QColorDialog::getColor(Qt::white,this);

    if(corFundo.isValid())
    {
        buffer.fill(corFundo);

        desenhaCurvas();
        interfaceUpdate();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{

    ui->labelPrincipal->setCursor(Qt::ClosedHandCursor);

    if (e->button() == Qt::LeftButton)
    {

        QVector<Curva>::iterator iterator;

        int index = 0;

        for(iterator = curvas.begin(); iterator != curvas.end(); iterator++)
        {

            QPoint one(iterator->pontosControle.at(0));
            QPoint two(iterator->pontosControle.at(1));
            QPoint three(iterator->pontosControle.at(2));
            QPoint four(iterator->pontosControle.at(3));

            qDebug() << iterator->pontosControle;

            if (markContains(one, (e->pos()  - ui->labelPrincipal->pos()))) {
                movingPoint = true;
                pointIndex = 0;
            }
            else if (markContains(two, (e->pos()  - ui->labelPrincipal->pos()))) {
                movingPoint = true;
                pointIndex = 1;
            }
            else if (markContains(three, (e->pos()  - ui->labelPrincipal->pos()))) {
                movingPoint = true;
                pointIndex = 2;
            }
            else if (markContains(four, (e->pos()  - ui->labelPrincipal->pos()))) {
                movingPoint = true;
                pointIndex = 3;
            }
            else {
                QPainterPathStroker stroker;
                stroker.setWidth(6);
                QPainterPath path(one);
                path.cubicTo(two, three, four);

                QPainterPath stroked = stroker.createStroke(path);
                if (stroked.contains((e->pos() - ui->labelPrincipal->pos()))) {
                    qDebug()<<"on the path";

                    onThePath = true;

                }
            }

            curveIndex = index;

            index++;


        }

        if (movingPoint || onThePath)
        {
            //qDebug() << "on the path or moving";
            mouseStart = (e->pos() - ui->labelPrincipal->pos());
            //qDebug() << *moving;
        }

        //desenhaCurvas();
        //interfaceUpdate();

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (movingPoint) {
        QPoint diff = (e->pos() - ui->labelPrincipal->pos()) - mouseStart;

        //qDebug() << diff;
        QPoint newPoint = QPoint(diff + curvas.at(curveIndex).pontosControle.at(pointIndex));

        int xMin = 0 + pointSize;
        int xMax = 600 - pointSize;
        int yMin = 0 + pointSize;
        int yMax = 500 - pointSize;

        if(newPoint.x() >= xMin && newPoint.y() >= yMin && newPoint.x() <= xMax && newPoint.y() <= yMax)
            curvas[curveIndex].pontosControle.replace(pointIndex,newPoint);

        interfaceUpdate();

        update();
        mouseStart = e->pos() - ui->labelPrincipal->pos();
    } else if (onThePath) {
        QPoint diff = (e->pos() - ui->labelPrincipal->pos()) - mouseStart;

        int xMin = 0 + pointSize;
        int xMax = 600 - pointSize;
        int yMin = 0 + pointSize;
        int yMax = 500 - pointSize;

        bool podeMover = true;

        for(int i=0;i<4;i++){

            QPoint newPoint = QPoint(diff + curvas[curveIndex].pontosControle.at(i));

            if(newPoint.x() <= xMin ||
                    newPoint.x() >=xMax ||
                    newPoint.y() <=yMin ||
                    newPoint.y() >=yMax)
            {
                podeMover = false;
            }


        }


        if(podeMover)
        {

            curvas[curveIndex].pontosControle[0].operator +=(diff);
            curvas[curveIndex].pontosControle[1].operator +=(diff);
            curvas[curveIndex].pontosControle[2].operator +=(diff);
            curvas[curveIndex].pontosControle[3].operator +=(diff);
        }

        interfaceUpdate();

        update();
        mouseStart = e->pos() - ui->labelPrincipal->pos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    ui->labelPrincipal->setCursor(Qt::OpenHandCursor);
    //qDebug()<<"releasing";
    movingPoint = false;
    onThePath = false;
}

bool MainWindow::markContains(const QPoint &pos, const QPoint &coord) const
{
    QRectF rect(pos.x() - pointSize,
                pos.y() - pointSize,
                (pointSize + (brushSize/2))*2, (pointSize + (brushSize/2))*2);
    QPainterPath path;
    path.addEllipse(rect);
    return path.contains(coord);
}

void MainWindow::on_saveFileDialog_clicked()
{

    QString newFilePath = QFileDialog::getSaveFileName(this,
                                                       tr("Salvar o arquivo descritor"),
                                                       QDir::currentPath(),
                                                    tr("TXT Files (*.txt)"));

    if(!newFilePath.isEmpty())
    {

    }

}

void MainWindow::on_saveImageDialog_clicked()
{

}
