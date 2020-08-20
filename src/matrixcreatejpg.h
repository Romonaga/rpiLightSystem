#ifndef MatrixCreateJpg_H
#define MatrixCreateJpg_H

#include "ilightshow.h"
#include <vector>




class MatrixCreateJpg : public ILightShow
{
    Q_OBJECT
public:
    MatrixCreateJpg(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();


private:
    std::vector<unsigned char> _jpgBuffer;

};

#endif // MatrixCreateJpg_H
