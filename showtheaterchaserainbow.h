#ifndef SHOWTheaterCHASERainbow_H
#define SHOWTheaterCHASERainbow_H

#include "ilightshow.h"

class ShowTheaterChaseRainBow : public ILightShow
{
    Q_OBJECT
public:
    ShowTheaterChaseRainBow(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:
    ws2811_led_t _color;
    uint16_t _wait;

};

#endif // SHOWTheaterCHASERainbow_H
