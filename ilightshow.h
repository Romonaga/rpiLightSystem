#ifndef ILIGHTSHOW_H
#define ILIGHTSHOW_H

#include <QObject>
#include <QString>
#include "ws2811wrapper.h"
#include "systemsettings.h"

#define LIGHT_SHOWS(X)                                                  \
            X(0, Nope, "Why Are We Here?"),                           \
            X(1, Blink, "Blink"),                           \
            X(2, Chaser, "Chaser"),                              \
            X(3, TC, "Theater Chase"),                                    \
            X(4, TCR, "Theater Chase Rainbow"),                        \
            X(5, Color3R, "Color 3 Reverse"),   \
            X(6, Cylon, "Cylon"),                          \
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


class ILightShow : public QObject
{
    Q_OBJECT

public:
    explicit ILightShow(SystemSettings* settings, Ws2811Wrapper* ledWrapper, QObject *parent = nullptr);

    virtual bool showParms(const QString& showParms) = 0;
    virtual void startShow() = 0;
    virtual void stopShow() = 0;


    LedLightShows lightShow() const;
    void setLightShow(const LedLightShows &lightShow);
    QString getShowName();

protected:
    SystemSettings* _settings;
    Ws2811Wrapper* _ledWrapper;
    bool _running;

    LedLightShows _lightShow;

signals:

};

Q_DECLARE_INTERFACE(ILightShow,"ILightShow")
#endif // ILIGHTSHOW_H

