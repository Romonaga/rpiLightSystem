#ifndef SHOWTColor3R_H
#define SHOWTColor3R_H

#include "ilightshow.h"

class ShowColor3R : public ILightShow
{
    Q_OBJECT
public:
    ShowColor3R(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:
    ws2811_led_t _color1;
    ws2811_led_t _color2;
    ws2811_led_t _color3;

    uint16_t _wait;

};

#endif // SHOWTColor3R_H
