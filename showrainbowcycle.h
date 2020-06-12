#ifndef ShowRainbowCycle_H
#define ShowRainbowCycle_H

#include "ilightshow.h"

class ShowRainbowCycle : public ILightShow
{
    Q_OBJECT
public:
    ShowRainbowCycle(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();


private:
    uint16_t _wait;

};

#endif // ShowRainbowCycle_H
