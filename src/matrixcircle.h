#ifndef MatrixCircle_H
#define MatrixCircle_H

#include "ilightshow.h"





class MatrixCircle : public ILightShow
{
    Q_OBJECT
public:
    MatrixCircle (Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // MatrixCircle_H
