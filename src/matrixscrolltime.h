#ifndef MatrixScrollTime_H
#define MatrixScrollTime_H

#include "ilightshow.h"



class MatrixScrollTime : public ILightShow
{
    Q_OBJECT
public:
    MatrixScrollTime(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:
    void scrollTime(QString msg, int maxRows, int maxCols, int rowStart, int drawCol, ws2811_led_t color,  ws2811_led_t *snapShotBuffer, int delay);

private:

};

#endif // MatrixScrollTime_H
