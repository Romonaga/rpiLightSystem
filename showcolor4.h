#ifndef SHOWTColor4_H
#define SHOWTColor4_H

#include "ilightshow.h"

class ShowColor4 : public ILightShow
{
    Q_OBJECT
public:
    ShowColor4(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:
    ws2811_led_t _color1;
    ws2811_led_t _color2;
    ws2811_led_t _color3;
    ws2811_led_t _color4;

    uint16_t _wait;

};

#endif // SHOWTColor4_H
