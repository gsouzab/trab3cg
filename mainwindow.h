#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QTime>
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
    QImage bufferExport;
    QVector<Curva> curvas;
    bool criarBezierManual;
    Curva curvamanual;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;
    QString filePath;
    QPainter *painter;
    QPainter *painterExport;
    float brushSize;
    int LOD;
    QColor corFundo;
    QColor corCaneta;

    void desenhaCurvas();
    void desenhaBezier(QVector<QPoint>);
    void desenhaHermite(QVector<QPoint> pontos);
    void interfaceUpdate();
    void desenhaPonto(const QPoint &pt);
    void desenhaLinha(const QPoint &inicio,const QPoint &fim);
    bool markContains(const QPoint &pt, const QPoint &coord) const;
    void leArquivoControle();
    void CriarCurvaManual(QPoint pos);

    QPoint mouseStart;
    bool movingPoint;
    int curveIndex;
    int pointIndex;
    bool onThePath;


private slots:
    void on_brushSizeSlider_valueChanged(int value);
    void on_corCurvaBtn_clicked();
    void on_corFundoBtn_clicked();
    void on_fileDialogOpen_clicked();
    void on_saveFileDialog_clicked();
    void on_saveImageDialog_clicked();

    void on_criarBezier_clicked();
    void on_checkboxCriarBezierManual_stateChanged(int arg1);

};
#endif // MAINWINDOW_H
