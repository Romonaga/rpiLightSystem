#ifndef ShowTriChaser_H
#define ShowColorEvery_H

#include "ilightshow.h"

class ShowColorEvery : public ILightShow
{
    Q_OBJECT
public:
    ShowColorEvery(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // ShowColorEvery_H
