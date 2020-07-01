#ifndef ShowScanner_H
#define ShowScanner_H

#include "ilightshow.h"

class ShowScanner : public ILightShow
{
    Q_OBJECT
public:
    ShowScanner(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();



};

#endif // ShowScanner_H
