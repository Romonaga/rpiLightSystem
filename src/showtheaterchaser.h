#ifndef SHOWTheaterCHASE_H
#define SHOWTheaterCHASE_H

#include "ilightshow.h"

class ShowTheaterChaser : public ILightShow
{
    Q_OBJECT
public:
    ShowTheaterChaser(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();


private:


};

#endif // SHOWTheaterCHASE_H
