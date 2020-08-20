#include "samplelightshowplugin.h"

SampleLightShowPlugIn::SampleLightShowPlugIn(Ws2811Wrapper *ledWrapper, const LedLightShows &lightShow) : LightShowPlugin(ledWrapper, lightShow)
{

}


/*********************************************/
// the class factories
extern "C" ILightShow* create(Ws2811Wrapper *ledWrapper, const LedLightShows &lightShow)
{
    return new ILightShow(ledWrapper, lightShow);
}

extern "C" void destroy(ILightShow* p)
{
    delete p;
}

