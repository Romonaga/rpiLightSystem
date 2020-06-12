#ifndef SHOWCyclon_H
#define SHOWCyclon_H

#include "ilightshow.h"

class ShowCyclon : public ILightShow
{
    Q_OBJECT
public:
    ShowCyclon(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:
    ws2811_led_t _color;
    uint8_t _speed;
    uint8_t _width;


};

#endif // SHOWCyclon_H
