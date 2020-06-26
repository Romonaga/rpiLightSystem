#ifndef ShowPulse_H
#define ShowPulse_H

#include "ilightshow.h"

class ShowPulse : public ILightShow
{
    Q_OBJECT
public:
    ShowPulse(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:




};

#endif // ShowPulse_H
