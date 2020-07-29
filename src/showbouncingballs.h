#ifndef ShowBouncingBalls_H
#define ShowBouncingBalls_H

#include "ilightshow.h"





class ShowBouncingBalls : public ILightShow
{
    Q_OBJECT
public:
    ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();

private:


};

#endif // ShowBouncingBalls_H
