#ifndef MatrixScrollText_H
#define MatrixScrollText_H

#include "ilightshow.h"





class MatrixScrollText : public ILightShow
{
    Q_OBJECT
public:
    MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~MatrixScrollText();
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();
    void shiftColumns();

private:
    ws2811_led_t* _image;
    int _rowStart;
    int _drawCol ;

};

#endif // ShowBouncingBalls_H
