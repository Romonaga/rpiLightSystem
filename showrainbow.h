#ifndef SHOWShowRainbow_H
#define SHOWShowRainbow_H

#include "ilightshow.h"

class ShowRainbow : public ILightShow
{
    Q_OBJECT
public:
    ShowRainbow(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();


private:
    uint16_t _wait;

};

#endif // SHOWCHASE_H
