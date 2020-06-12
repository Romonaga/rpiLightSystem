#ifndef ShowBlink_H
#define ShowBlink_H

#include "ilightshow.h"

class ShowBlink : public ILightShow
{
    Q_OBJECT
public:
    ShowBlink(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:
    uint8_t _numBlinks;
    uint16_t _wait;
    uint8_t _brightness;




};

#endif // ShowBlink_H
