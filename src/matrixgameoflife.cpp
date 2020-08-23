#include "matrixgameoflife.h"
#include <QDebug>

MatrixGameOfLife::MatrixGameOfLife(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    _torus = true;
    // Allocate memory

    _values = new int*[_ledWrapper->getColumns()];
    _newValues = new int*[_ledWrapper->getColumns()];

    for (uint32_t x=0; x <_ledWrapper->getColumns(); ++x)
    {
        _values[x] = new int[_ledWrapper->getRows()];
        _newValues[x] = new int[_ledWrapper->getRows()];

        for (uint32_t y=0; y<_ledWrapper->getRows(); ++y)
        {
            _values[x][y] = genRand(0, time(nullptr)) % 2;
        }

    }
}

MatrixGameOfLife::~MatrixGameOfLife()
{
    for (uint32_t x=0; x <_ledWrapper->getColumns(); ++x)
        delete [] _values[x];
    delete [] _values;

    for (uint32_t x=0; x <_ledWrapper->getColumns(); ++x)
        delete [] _newValues[x];
    delete [] _newValues;
}

int MatrixGameOfLife::numAliveNeighbours(int x, int y)
{
    int num=0;
    if (_torus)
    {
        // Edges are connected (torus)
        num += _values[(x-1+_ledWrapper->getColumns()) % _ledWrapper->getColumns()][(y-1+ _ledWrapper->getRows()) % _ledWrapper->getRows()];
        num += _values[(x-1+_ledWrapper->getColumns()) % _ledWrapper->getColumns()][y];
        num += _values[(x-1+_ledWrapper->getColumns()) % _ledWrapper->getColumns()][(y+1) % _ledWrapper->getRows()];
        num += _values[(x+1)%_ledWrapper->getColumns()][(y-1+_ledWrapper->getRows()) % _ledWrapper->getRows()];
        num += _values[(x+1)%_ledWrapper->getColumns()][y];
        num += _values[(x+1)%_ledWrapper->getColumns()][(y+1) % _ledWrapper->getRows()];
        num += _values[x][(y-1+_ledWrapper->getRows()) % _ledWrapper->getRows()];
        num += _values[x][(y+1)%_ledWrapper->getRows()];
    }
    else
    {
        // Edges are not connected (no torus)
        if (x > 0)
        {
            if (y > 0)
                num += _values[x-1][y-1];
            if ( y < (int)_ledWrapper->getRows() - 1)
                num += _values[x-1][y+1];

            num += _values[x-1][y];
        }

        if (x < (int)_ledWrapper->getColumns() - 1)
        {
            if (y>0)
                num += _values[x+1][y-1];
            if (y<31)
                num += _values[x+1][y+1];
            num += _values[x+1][y];
        }

        if (y>0)
            num += _values[x][y-1];
        if (y < (int)_ledWrapper->getRows()-1)
            num += _values[x][y+1];
    }
    return num;
}

void MatrixGameOfLife::updateValues()
{


    // update newValues based on values
    for (uint32_t x=0; x<_ledWrapper->getColumns(); ++x)
    {
        for (uint32_t y=0; y<_ledWrapper->getRows(); ++y)
        {
            _newValues[x][y] = _values[x][y];
            int num = numAliveNeighbours(x,y);
            if (_values[x][y])
            {
               // cell is alive
               if (num < 2 || num > 3)
                   _newValues[x][y] = 0;

            }
            else
            {
               // cell is dead
               if (num == 3)
                   _newValues[x][y] = 1;
            }
        }
    }

    // copy newValues to values
    for (uint32_t x=0; x<_ledWrapper->getColumns(); ++x)
    {
      for (uint32_t y=0; y<_ledWrapper->getRows(); ++y)
      {
        _values[x][y] = _newValues[x][y];
      }
    }


}

void MatrixGameOfLife::startShow()
{
    while (_running && _endTime > time(nullptr))
    {
        updateValues();

        for (uint32_t x=0; x <_ledWrapper->getColumns(); ++x)
        {
            for (uint32_t y=0; y<_ledWrapper->getRows(); ++y)
            {
                if (_values[x][y])
                   _ledWrapper->setPixelColor(y, x, _r, _g, _b);
                else
                   _ledWrapper->setPixelColor(y, x, 0, 0, 0);


            }

        }

         _ledWrapper->show();
        Ws2811Wrapper::waitMillSec(_wait);
    }
}
