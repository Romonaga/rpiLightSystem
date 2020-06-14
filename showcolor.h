#ifndef ShowColor_H
#define ShowColor_H

#include "ilightshow.h"

class ShowColor : public ILightShow
{
    Q_OBJECT
public:
    ShowColor(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();



};

#endif // ShowColor_H
