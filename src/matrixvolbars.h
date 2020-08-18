#ifndef MAtrixVolBars_H
#define MAtrixVolBars_H

#include "ilightshow.h"
#include <math.h>


class MatrixVolBars : public ILightShow
{
    Q_OBJECT
public:
    MatrixVolBars(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void drawBarRow(int bar, int y,  ws2811_led_t color);

    // ILightShow interface
public:
    void startShow();

private:



    int* _barHeights;
    int _barWidth;
    int _height;



};

#endif // ShowBouncingBalls_H
