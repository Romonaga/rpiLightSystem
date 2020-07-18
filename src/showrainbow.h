#ifndef SHOWShowRainbow_H
#define SHOWShowRainbow_H

#include "ilightshow.h"

class ShowRainbow : public ILightShow
{
    Q_OBJECT
public:
    ShowRainbow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();


private:

};

#endif // SHOWCHASE_H
