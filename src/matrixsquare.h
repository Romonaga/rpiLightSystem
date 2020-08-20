#ifndef MatrixSquare_H
#define MatrixSquare_H

#include "ilightshow.h"





class MatrixSquare : public ILightShow
{
    Q_OBJECT
public:
    MatrixSquare(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // MatrixSquare_H
