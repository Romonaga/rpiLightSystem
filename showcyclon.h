#ifndef SHOWCyclon_H
#define SHOWCyclon_H

#include "ilightshow.h"

class ShowCyclon : public ILightShow
{
    Q_OBJECT
public:
    ShowCyclon(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:


};

#endif // SHOWCyclon_H
