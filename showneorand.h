#ifndef SHOWShowNeoRand_H
#define SHOWShowNeoRand_H

#include "ilightshow.h"

class ShowNeoRand : public ILightShow
{
    Q_OBJECT
public:
    ShowNeoRand(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:
    unsigned int random9();
    uint32_t Bow(int n);


private:
    uint16_t _wait;
    unsigned int _rno;
    unsigned int _cno;
    int _tw;

    ws2811_led_t _blue;
    ws2811_led_t _green;
    ws2811_led_t _red;


};

#endif // SHOWShowNeoRand_H
