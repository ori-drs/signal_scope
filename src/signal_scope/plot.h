#ifndef _PLOT_H_
#define _PLOT_H_

#include "fpscounter.h"

#include <qwt_plot.h>
#include <qwt_interval.h>
#include <qwt_system_clock.h>

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotDirectPainter;

class SignalData;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot(QWidget * = NULL);
    virtual ~Plot();

    void start();
    void stop();

    virtual void replot();

    void addSignal(SignalData* signalData, QColor color);
    void removeSignal(SignalData* signalData);
    void setSignalVisible(SignalData* signalData, bool visible);
    void setSignalColor(SignalData* signalData, QColor color);

    double timeWindow();

public Q_SLOTS:
    void setTimeWindow(double);
    void setYScale(double);

protected:
    virtual void timerEvent(QTimerEvent *);

private:
    void initGradient();

    QwtPlotMarker *d_origin;

    QwtInterval d_interval;
    int d_timerId;

    int mColorMode;

    QMap<SignalData*, QwtPlotCurve*> mSignals;

    FPSCounter fpsCounter;

};

#endif
