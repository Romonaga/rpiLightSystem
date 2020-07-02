#ifndef ShowTwinkle_H
#define ShowTwinkle_H

#include "ilightshow.h"
#include <QMap>


class ShowTwinkle : public ILightShow
{
    Q_OBJECT
public:
    ShowTwinkle(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~ShowTwinkle();

    // ILightShow interface
public:
    void startShow();
    void stopShow();

private:
    void resetLeds();
    
private:
    QMap<unsigned int, unsigned int> _twinks;


};

#endif // ShowTwinkle_H