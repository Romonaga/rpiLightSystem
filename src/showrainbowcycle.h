#ifndef ShowRainbowCycle_H
#define ShowRainbowCycle_H

#include "ilightshow.h"

class ShowRainbowCycle : public ILightShow
{
    Q_OBJECT
public:
    ShowRainbowCycle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // ShowRainbowCycle_H
