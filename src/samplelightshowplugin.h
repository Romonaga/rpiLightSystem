#ifndef SAMPLELIGHTSHOWPLUGIN_H
#define SAMPLELIGHTSHOWPLUGIN_H
#include "ws2811wrapper.h"
#include "ilightshow.h"

#include <QObject>

class SampleLightShowPlugIn : ILightShow
{
    Q_OBJECT
public:
    SampleLightShowPlugIn(Ws2811Wrapper *ledWrapper, const LedLightShows &lightShow);

    // ILightShow interface
private:
    void startShow();
};

void SampleLightShowPlugIn::startShow()
{
}

#endif // SAMPLELIGHTSHOWPLUGIN_H
