#ifndef SHOWTHnH_H
#define SHOWTHnH_H

#include "ilightshow.h"

class ShowHnH : public ILightShow
{
    Q_OBJECT
public:
    ShowHnH(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:
    ws2811_led_t _color1;
    ws2811_led_t _color2;

    uint16_t _wait;

};

#endif // SHOWTHnH_H
