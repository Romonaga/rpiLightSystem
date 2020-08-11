#ifndef MatrixTriangle_H
#define MatrixTriangle_H

#include "ilightshow.h"





class MatrixTriangle : public ILightShow
{
    Q_OBJECT
public:
    MatrixTriangle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // MatrixTriangle_H
