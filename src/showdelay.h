#ifndef ShowDelay_H
#define ShowDelay_H

#include "ilightshow.h"

class ShowDelay : public ILightShow
{
    Q_OBJECT
public:
    ShowDelay(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:




};

#endif // ShowDelay_H
