#include "matrixvolbars.h"


MatrixVolBars::MatrixVolBars(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{


}


void MatrixVolBars::drawBarRow(int bar, int y, ws2811_led_t color)
{
  for (int x=bar*_barWidth; x<(bar+1)*_barWidth; ++x)
  {
    _ledWrapper->setPixelColor( (_height - 1) - y, x, color);
  }

}


void MatrixVolBars::startShow()
{
    if(_settings->getChannels()[_channelId]->matrixdirection() == 0) return;


    int heightGreen;
    int heightYellow;
    int heightOrange;

    int* barFreqs;
    int* barMeans;
    int t = 0;

    const int width = _ledWrapper->getColumns();
    _height = _ledWrapper->getRows();
    _barWidth = width/_width;
    _barHeights = new int[_width];
    barMeans = new int[_width];
    barFreqs = new int[_width];
    heightGreen  = _height*4/12;
    heightYellow = _height*8/12;
    heightOrange = _height*10/12;

    // Array of possible bar means
    int numMeans = 10;
    int means[10] = {1,2,3,4,5,6,7,8,16,32};
    for (int i=0; i<numMeans; ++i)
    {
      means[i] = _height - means[i]*_height/8;
    }
    // Initialize bar means randomly

    srand(time(NULL));
    for (int i=0; i<_width; ++i)
    {
      barMeans[i] = rand()%numMeans;
      barFreqs[i] = 1<<(rand()%3);
    }

    // Start the loop
    while(_endTime > time(nullptr) && _running == true)
    {
      if (t % 8 == 0)
      {
        // Change the means
        for (int i=0; i<_width; ++i)
        {
          barMeans[i] += rand()%3 - 1;
          if (barMeans[i] >= numMeans)
            barMeans[i] = numMeans-1;
          if (barMeans[i] < 0)
            barMeans[i] = 0;
        }
      }

      // Update bar heights
      t++;
      for (int i=0; i<_width; ++i) {
        _barHeights[i] = (_height - means[barMeans[i]])
          * sin(0.1*t*barFreqs[i]) + means[barMeans[i]];
        if (_barHeights[i] < _height/8)
          _barHeights[i] = rand() % (_height/8) + 1;
      }

      for (int i=0; i<_width; ++i)
      {
        int y;
        for (y=0; y<_barHeights[i]; ++y)
        {
          if (y<heightGreen)
            drawBarRow(i, y, _color1);
          else if (y<heightYellow)
            drawBarRow(i, y, _color2);
          else if (y<heightOrange)
            drawBarRow(i, y, _color3);
          else
            drawBarRow(i, y, _color4);

        }

        // Anything above the bar should be black
        for (; y<_height; ++y)
        {
          drawBarRow(i, y, 0);
        }
      }

      _ledWrapper->show();
      Ws2811Wrapper::waitMillSec(_wait);

    }

    delete [] _barHeights;
    delete [] barFreqs;
    delete [] barMeans;

  }


