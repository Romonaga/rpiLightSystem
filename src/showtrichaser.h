#ifndef SHOWShowTriChaser_H
#define SHOWShowTriChaser_H

#include "ilightshow.h"

class ShowTriChaser : public ILightShow
{
    Q_OBJECT
public:
    ShowTriChaser(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // SHOWShowTriChaser_H
