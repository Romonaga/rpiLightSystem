#ifndef SHOWColorWipe_H
#define SHOWColorWipe_H

#include "ilightshow.h"

class ShowColorWipe : public ILightShow
{
    Q_OBJECT
public:
    ShowColorWipe(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();


private:

};

#endif // SHOWColorWipe_H
