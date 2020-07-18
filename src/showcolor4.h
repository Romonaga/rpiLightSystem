#ifndef SHOWTColor4_H
#define SHOWTColor4_H

#include "ilightshow.h"

class ShowColor4 : public ILightShow
{
    Q_OBJECT
public:
    ShowColor4(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // SHOWTColor4_H
