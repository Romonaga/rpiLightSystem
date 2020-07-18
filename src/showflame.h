#ifndef SHOWFIRE_H
#define SHOWFIRE_H

#include "ilightshow.h"

class ShowFlame : public ILightShow
{
    Q_OBJECT
public:
    ShowFlame(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();



};

#endif // SHOWFIRE_H
