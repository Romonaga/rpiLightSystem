#ifndef SHOWTHnH_H
#define SHOWTHnH_H

#include "ilightshow.h"

class ShowHnH : public ILightShow
{
    Q_OBJECT
public:
    ShowHnH(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:

};

#endif // SHOWTHnH_H
