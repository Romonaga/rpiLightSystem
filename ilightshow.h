#ifndef ILIGHTSHOW_H
#define ILIGHTSHOW_H

#include <QThread>
#include <QString>
#include "ws2811wrapper.h"
#include "systemsettings.h"
#include "dnrlogger.h"


#include  <condition_variable>


#define LIGHT_SHOWS(X)                                                  \
            X(0, Nope, "Why Are We Here?"),                           \
            X(1, Blink, "Blink"),                           \
            X(2, Chaser, "Chaser"),                              \
            X(3, TC, "Theater Chase"),                                    \
            X(4, TCR, "Theater Chase Rainbow"),                        \
            X(5, Color3R, "Color 3 Reverse"),   \
            X(6, Cyclon, "Cylon"),                          \
            X(7, ColorWipe, "Color Wipe"),                    \
            X(8, HAndH, "Half n Half"),                    \
            X(9, Rainbow, "Rainbow"),                   \
            X(10, RainbowCycle, "Rainbow Cycle"),                   \
            X(11, NeoRand, "NeoRand (Twinkle)"),                   \
            X(12, Flame, "Flame"),                   \
            X(13, ColorThirds, "ColorThirds"),                   \
            X(14, ColorForths, "ColorForths"),                   \
            X(15, TriChaser, "Tri-Color Chase")                   \


#define LIGHT_SHOWS_ENUM(type, name, str) name = type
#define LIGHT_SHOWS_STRING(type, name, str) str

typedef enum {
    LIGHT_SHOWS(LIGHT_SHOWS_ENUM),

    LIGHT_SHOWS_ENUM_COUNT
} LedLightShows;


class ILightShow : public QThread
{
    Q_OBJECT

public:
    explicit ILightShow(SystemSettings* settings, Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms);
    void stopShow();


    LedLightShows getLightShow() const;
    QString getShowName();

private:
    virtual void startShow() = 0;

protected:
    SystemSettings* _settings;
    Ws2811Wrapper* _ledWrapper;
    QString _showParms;

    bool _running;

    LedLightShows _lightShow;
    DNRLogger* _logger;

protected:
    void run();


signals:
    void showComplete(ILightShow*);

};

Q_DECLARE_INTERFACE(ILightShow,"ILightShow")
#endif // ILIGHTSHOW_H

