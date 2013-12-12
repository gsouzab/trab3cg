#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QPointF>
#include <QVector>

#include "curva.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString getFilePath() const;
    void setFilePath(const QString &value);
    QImage buffer;
    QVector<Curva> curvas;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *e);
private:
    Ui::MainWindow *ui;
    Dialog *fileExplorer;
    QString filePath;
    QPainter *painter;
    float brushSize = 1;
    QColor corFundo = Qt::white;
    QColor corCaneta = Qt::black;

    void desenhaCurvas();
    void desenhaBezier(QVector<QPoint>);
    void interfaceUpdate();
    void desenhaPonto(const QPoint &pt);
    void desenhaLinha(const QPoint &inicio,const QPoint &fim);
    bool markContains(const QPoint &pt, const QPoint &coord) const;
    void leArquivoControle();

    QPoint mouseStart;
    bool movingPoint;
    int curveIndex = 0;
    int pointIndex = 0;
    bool onThePath;


private slots:
    void on_fileDialogOpen_clicked();
    void on_brushSizeSlider_valueChanged(int value);
    void on_corCurvaBtn_clicked();
    void on_corFundoBtn_clicked();
    void on_commandLinkButton_clicked();
    void on_saveFileDialog_clicked();
};
#endif // MAINWINDOW_H
