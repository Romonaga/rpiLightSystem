#ifndef MatrixCreateDisplayFile_H
#define MatrixCreateDisplayFile_H

#include "ilightshow.h"
#include <vector>





class MatrixCreateDisplayFile : public ILightShow
{
    Q_OBJECT
public:
    MatrixCreateDisplayFile(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:


private:
    std::vector<unsigned char> _jpgBuffer;


};

#endif // MatrixCreateDisplayFile_H
