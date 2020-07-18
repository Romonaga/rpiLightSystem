#ifndef ShowFade_H
#define ShowPulse_H

#include "ilightshow.h"

class ShowFade : public ILightShow
{
    Q_OBJECT
public:
    ShowFade(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:




};

#endif // ShowPulse_H
