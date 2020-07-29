#ifndef MatrixArt_H
#define MatrixArt_H

#include "ilightshow.h"

class MatrixArt : public ILightShow
{
    Q_OBJECT
public:
    MatrixArt(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:




};

#endif // MatrixArt_H
