#ifndef SHOWFIRE_H
#define SHOWFIRE_H

#include "ilightshow.h"

class ShowFire : public ILightShow
{
    Q_OBJECT
public:
    ShowFire(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();



};

#endif // SHOWFIRE_H
