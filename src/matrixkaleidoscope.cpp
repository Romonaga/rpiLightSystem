#include "matrixkaleidoscope.h"
#include <QDebug>




MatrixKaleidoscope::MatrixKaleidoscope(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
    _width = _ledWrapper->getColumns() - 1; // We need an odd width
    _height = _ledWrapper->getRows() - 1; // We need an odd height

        // Allocate memory
    _values = new int*[_width];
    for (int x=0; x<_width; ++x)
    {
        _values[x] = new int[_height];
    }

    _newValues = new int*[_width];
    for (int x=0; x<_width; ++x)
    {
        _newValues[x] = new int[_height];
    }

    // Init values
    for (int x=0; x<_width; ++x)
    {
        for (int y=0; y<_height; ++y)
        {
            _values[x][y] = 0;
        }
    }
}

MatrixKaleidoscope::~MatrixKaleidoscope()
{
    for (int x=0; x<_width; ++x)
    {
        delete [] _values[x];
    }
    delete [] _values;

    for (int x=0; x<_width; ++x)
    {
        delete [] _newValues[x];
    }

    delete [] _newValues;
}

void MatrixKaleidoscope::updateValues()
{
    // Copy values to newValues
    for (int x=0; x<_width; ++x)
    {
      for (int y=0; y<_height; ++y)
      {
        _newValues[x][y] = _values[x][y];
      }
    }

    // Update newValues based on values
    for (int x=0; x<_width; ++x)
    {
      for (int y=0; y<_height; ++y)
      {
        if (_values[x][y] > 3)
        {
          // Collapse
          if (x>0)
            _newValues[x-1][y]++;
          if (x<_width-1)
            _newValues[x+1][y]++;
          if (y>0)
            _newValues[x][y-1]++;
          if (y<_height-1)
            _newValues[x][y+1]++;

          _newValues[x][y] -= 4;
        }
      }
    }
    // Copy newValues to values
    for (int x=0; x<_width; ++x)
    {
      for (int y=0; y<_height; ++y)
      {
        _values[x][y] = _newValues[x][y];
      }
    }
}

void MatrixKaleidoscope::startShow()
{
    while (_running && _endTime > time(nullptr))
    {
        _values[_width/2][_height/2]++;
        updateValues();

        for (int x=0; x<_width; ++x)
        {
            for (int y=0; y<_height; ++y)
            {
                switch (_values[x][y])
                {
                    case 0:
                        _ledWrapper->setPixelColor(y,x, 0, 0, 0);
                    break;

                    case 1:
                        _ledWrapper->setPixelColor(y,x, _color1);
                    break;

                    case 2:
                        _ledWrapper->setPixelColor(y,x, _color2);
                    break;

                    case 3:
                        _ledWrapper->setPixelColor(y,x, _color3);
                    break;

                    default:
                        _ledWrapper->setPixelColor(y,x, _color4);
                }
            }
        }
        _ledWrapper->show();
        _ledWrapper->waitMillSec(_wait);
    }
}
