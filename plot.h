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
    void setupPloter(void);
    void plotPoint(double y, double x);

private:
    QwtPlot* ploter;
    QwtPlotCurve* d_curve1;

    QVector<double> x_data;
    QVector<double> y_data;
};

#endif // PLOT_H
