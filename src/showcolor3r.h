#ifndef SHOWTColor3R_H
#define SHOWTColor3R_H

#include "ilightshow.h"

class ShowColor3R : public ILightShow
{
    Q_OBJECT
public:
    ShowColor3R(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:

};

#endif // SHOWTColor3R_H
