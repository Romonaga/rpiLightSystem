#ifndef MatrixScrollText_H
#define MatrixScrollText_H

#include "ilightshow.h"





class MatrixScrollText : public ILightShow
{
    Q_OBJECT
public:
    MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~MatrixScrollText();

    // ILightShow interface
public:
    void startShow();


private:


    
private:





};

#endif // ShowBouncingBalls_H
