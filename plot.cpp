#include "plot.h"


Plot::Plot(QwtPlot *uiplot)
{
    ploter = uiplot;
}

void Plot::setupPloter(void)
{
    //Qwt Plot Widget Init
    //Axis
    ploter->setAxisMaxMajor(QwtPlot::xBottom, 5);
    ploter->setAxisMaxMinor(QwtPlot::xBottom, 9);
    ploter->setAxisMaxMajor(QwtPlot::yRight, 5);
    ploter->setAxisMaxMinor(QwtPlot::yRight, 9);

    ploter->setAxisTitle(QwtPlot::xBottom, "Time");
    ploter->setAxisTitle(QwtPlot::yLeft, "Attitude");

    //Grid
    QwtPlotGrid* plotGrid = new QwtPlotGrid;

    plotGrid->enableXMin(true);
    plotGrid->setMajorPen(Qt::black, 0, Qt::DotLine);
    plotGrid->setMinorPen(Qt::gray, 0, Qt::DotLine);
    plotGrid->attach(ploter);

    //Curve
    d_curve1 = new QwtPlotCurve( "Attitude" );
    d_curve1->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_curve1->setPen( Qt::red);
    d_curve1->setLegendAttribute( QwtPlotCurve::LegendShowLine );
    d_curve1->setYAxis( QwtPlot::yLeft );
    d_curve1->attach(ploter);

    //background
    ploter->setCanvasBackground(QColor("white"));

    ploter->setAutoReplot(true);
}

void Plot::plotPoint(double y, double x)
{
    y_data.append(y);
    x_data.append(x);

    Q_ASSERT(x_data.length() == y_data.length());
    d_curve1->setSamples(x_data, y_data);
}
