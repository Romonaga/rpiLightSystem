#ifndef MatrixScrollText_H
#define MatrixScrollText_H

#include "ilightshow.h"





class MatrixScrollText : public ILightShow
{
    Q_OBJECT
public:
    MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();
    void shiftRows();

private:


};

#endif // ShowBouncingBalls_H
