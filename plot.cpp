#include "plot.h"


Plot::Plot(QwtPlot *uiplot)
{
    ploter = uiplot;
}

void Plot::setupPloter(int curveCount)
{
    int i;

    m_plotCurveCount = curveCount;

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

    curveList.clear();

    //Curve
    for(i = 0; i < curveCount; i++)
    {
        curveList.append(new QwtPlotCurve( "Attitude" ));
        curveList[i]->setRenderHint( QwtPlotItem::RenderAntialiased );

        if( i == 0)
        {
            curveList[i]->setPen(Qt::red);
        }
        else if(i == 1)
        {
            curveList[i]->setPen(Qt::black);
        }
        else
        {
            curveList[i]->setPen(Qt::blue);
        }

        curveList[i]->setLegendAttribute( QwtPlotCurve::LegendShowLine );
        curveList[i]->setYAxis( QwtPlot::yLeft );
        curveList[i]->attach(ploter);

        y_data.append(new QVector<double>);
        x_data.append(new QVector<double>);
    }

    //background
    ploter->setCanvasBackground(QColor("white"));

    ploter->setAutoReplot(true);

}

void Plot::plotPoint(double y, double x, int i)
{
    y_data[i]->append(y);
    x_data[i]->append(x);

    Q_ASSERT(x_data[i]->length() == y_data[i]->length());

    curveList[i]->setSamples(*x_data[i], *y_data[i]);
}

QVector<double> Plot::plotDataValue(int index)
{
    int i;
    double* dataItem;

    QVector<double> data = QVector<double>();

    for(i = 0; i < m_plotCurveCount; i++)
    {
        dataItem = y_data[i]->data();
        data.append(*(dataItem + index));

    }
    return data;
}

int Plot::plotDataCount(void)
{
    int i;
    for(i = 0; i < m_plotCurveCount; i++)
    {
        Q_ASSERT(x_data[i]->length() == y_data[i]->length());
    }

    return y_data[0]->length();
}
