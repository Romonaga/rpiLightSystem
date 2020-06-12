#ifndef SHOWTheaterCHASE_H
#define SHOWTheaterCHASE_H

#include "ilightshow.h"

class ShowTheaterChaser : public ILightShow
{
    Q_OBJECT
public:
    ShowTheaterChaser(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:
    ws2811_led_t _color;
    uint16_t _wait;

};

#endif // SHOWTheaterCHASE_H
