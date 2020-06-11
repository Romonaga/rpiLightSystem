#ifndef SHOWFIRE_H
#define SHOWFIRE_H

#include "ilightshow.h"

class ShowFire : public ILightShow
{
    Q_OBJECT
public:
    ShowFire(SystemSettings* settings, Ws2811Wrapper* ledWrapper, QObject *parent = nullptr);

    // ILightShow interface
public:
    bool showParms(const QString &showParms);
    void startShow();
    void stopShow();
};

#endif // SHOWFIRE_H
