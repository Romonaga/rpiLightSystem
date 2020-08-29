#include "matrixantart.h"
#include <QDebug>


MatrixAntArt::MatrixAntArt(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{
    _colRowData = new int*[_ledWrapper->getColumns()];
    for (uint32_t x=0; x<_ledWrapper->getColumns(); ++x)
    {
      _colRowData[x] = new int[_ledWrapper->getRows()];
    }
}

MatrixAntArt::~MatrixAntArt()
{
    for (uint32_t x=0; x <_ledWrapper->getColumns(); ++x)
    {
        delete [] _colRowData[x];
    }
    delete [] _colRowData;
}


void MatrixAntArt::startShow()
{

    if(_settings->getChannels()[_channelId]->matrixdirection() == 0) return;

    _antX = _ledWrapper->getColumns() / 2;
    _antY = (_ledWrapper->getRows() / 2) - 3;
    _antDirection = genRand(0, 3);

    if(_clearOnStart == true)
    {
        for (uint32_t x=0; x < _ledWrapper->getColumns(); ++x)
        {
          for (uint32_t y=0; y < _ledWrapper->getRows(); ++y)
          {
            _colRowData[x][y] = 0;
          }
        }
        _ledWrapper->clearLeds();
    }


    while (_running == true)
    {
        // LLRR
        switch (_colRowData[_antX][_antY])
        {

        case 0:
        case 1:
          _antDirection = (_antDirection+1+4) % 4;
          break;

        case 2:
        case 3:
          _antDirection = (_antDirection-1+4) % 4;
          break;
        }

        _colRowData[_antX][_antY] = (_colRowData[_antX][_antY] + 1) % 4;
        int oldX = _antX;
        int oldY = _antY;
        switch (_antDirection)
        {
        case 0:
          _antX++;
          break;
        case 1:
          _antY++;
          break;
        case 2:
          _antX--;
          break;
        case 3:
          _antY--;
          break;
        }
        moveAnt(oldX, oldY);

        if (_antX >= _ledWrapper->getColumns() || _antY >= _ledWrapper->getRows()) return;

        moveAnt(_antX, _antY);
        _ledWrapper->waitMillSec(_wait);

      }
}

void MatrixAntArt::moveAnt(uint32_t x, uint32_t y)
{
    switch (_colRowData[x][y])
    {
        case 0:
          _ledWrapper->setPixelColor(y, x, _color1);
          break;
        case 1:
          _ledWrapper->setPixelColor(y, x, _color2);
          break;
        case 2:
          _ledWrapper->setPixelColor(y, x, _color3);
          break;
        case 3:
          _ledWrapper->setPixelColor(y, x, _color4);
          break;
    }

    if (x == _antX && y == _antY)
        _ledWrapper->setPixelColor(y, x, 0);

     _ledWrapper->show();

  }
