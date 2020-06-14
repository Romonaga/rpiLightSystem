#ifndef SHOWCHASE_H
#define SHOWCHASE_H

#include "ilightshow.h"

class ShowChaser : public ILightShow
{
    Q_OBJECT
public:
    ShowChaser(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();
    void stopShow();


private:

};

#endif // SHOWCHASE_H
