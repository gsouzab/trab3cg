#ifndef CURVA_H
#define CURVA_H

#include <QPoint>
#include <QVector>

class Curva
{
public:
    Curva();

    int getTipoCurva() const;
    void setTipoCurva(int value);
    QVector<QPoint> pontosControle;

    enum {BEZIER,HERMIT};

private:
    int tipoCurva;
};

#endif // CURVA_H
