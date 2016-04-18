#ifndef PLOT_H
#define PLOT_H

#include <QObject>

#include <qwt_plot.h>
#include "qwt_plot_grid.h"
#include "qwt_plot_curve.h"

class Plot : QObject
{
    Q_OBJECT

public:
    Plot(QwtPlot* uiplot);
    void setupPloter(int curveCount);
    void plotPoint(double y, double x, int i);
    QVector<double> plotDataValue(int index);
    int plotDataCount(void);

private:
    QwtPlot* ploter;
    QVector<QwtPlotCurve*> curveList;

    QVector<QVector<double>*> x_data;
    QVector<QVector<double>*> y_data;

    int m_plotCurveCount;
};

#endif // PLOT_H
