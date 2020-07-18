#ifndef SHOWTheaterCHASERainbow_H
#define SHOWTheaterCHASERainbow_H

#include "ilightshow.h"

class ShowTheaterChaseRainBow : public ILightShow
{
    Q_OBJECT
public:
    ShowTheaterChaseRainBow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // SHOWTheaterCHASERainbow_H
