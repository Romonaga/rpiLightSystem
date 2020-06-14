#ifndef ShowBlink_H
#define ShowBlink_H

#include "ilightshow.h"

class ShowBlink : public ILightShow
{
    Q_OBJECT
public:
    ShowBlink(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:




};

#endif // ShowBlink_H
