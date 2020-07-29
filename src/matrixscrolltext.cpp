#include "matrixscrolltext.h"
#include <QVector>
#include <math.h>
#include <QDebug>

#include "font5x7ext.h"



MatrixScrollText::MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}
 void MatrixScrollText::startShow()
{
    int bitCounter = 0;

    while(_endTime > time(nullptr))
    {

      for(int letter = 0; letter < _matrixText.length(); letter++)
      {

        if((int)_matrixText.toStdString().c_str()[letter] < 32 || (int)_matrixText.toStdString().c_str()[letter] > 122)
            continue;

        bitCounter = 0;
        _ledWrapper->clearLeds();
        for(int row = 0; row < MAXROWS; row++)
        {
            if(_running == false)
              return;

            for(int col = 0; col < MAXCOLS; col++)
            {

                if(bitsPerLetter[(int)_matrixText.toStdString().c_str()[letter] - 32][bitCounter] == 1)
                    _ledWrapper->setPixelColor(row, col, _color1);
                else
                    _ledWrapper->setPixelColor(row, col, _ledWrapper->Color(0,0,0));
                bitCounter++;
            }
        }

        _ledWrapper->show();
        if(_running == false)
          return;
         Ws2811Wrapper::waitMillSec(_wait);

      }

    }

}


