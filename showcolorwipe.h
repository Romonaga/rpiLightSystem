#ifndef SHOWColorWipe_H
#define SHOWColorWipe_H

#include "ilightshow.h"

class ShowColorWipe : public ILightShow
{
    Q_OBJECT
public:
    ShowColorWipe(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();


private:
    ws2811_led_t _color;
    uint16_t _wait;

};

#endif // SHOWColorWipe_H
