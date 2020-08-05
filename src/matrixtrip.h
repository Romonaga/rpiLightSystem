#ifndef MatrixTrip_H
#define MatrixTrip_H

#include "ilightshow.h"





class MatrixTrip : public ILightShow
{
    Q_OBJECT
public:
    MatrixTrip(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();

private:
    void midPointCircleDraw(int x_centre, int y_centre, int r) ;

    void circlePoints(int cx, int cy, int x, int y);
    void circleMidpoint(int xCenter, int yCenter, int radius);


};

#endif // MatrixTrip_H
