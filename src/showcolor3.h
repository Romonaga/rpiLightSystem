#ifndef SHOWTColor3_H
#define SHOWTColor3_H

#include "ilightshow.h"

class ShowColor3 : public ILightShow
{
    Q_OBJECT
public:
    ShowColor3(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();


private:


};

#endif // SHOWTColor3_H
