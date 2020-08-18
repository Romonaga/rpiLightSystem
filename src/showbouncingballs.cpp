#include "showbouncingballs.h"
#include <QDebug>


#include <QDateTime>
#include <QTime>


#include "pixalfonts.h"


using namespace std;



ShowBouncingBalls::ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    width_ = _ledWrapper->getColumns();
    height_ = _ledWrapper->getRows();

    // Allocate memory
    values_ = new int*[width_];
    for (int x=0; x<width_; ++x) {
      values_[x] = new int[height_];
    }
    newValues_ = new int*[width_];
    for (int x=0; x<width_; ++x) {
      newValues_[x] = new int[height_];
    }

    // Init values randomly
    srand(time(NULL));
    for (int x=0; x<width_; ++x) {
      for (int y=0; y<height_; ++y) {
        values_[x][y]=rand()%2;
      }
    }
    r_ = rand()%255;
    g_ = rand()%255;
    b_ = rand()%255;

    if (r_<150 && g_<150 && b_<150) {
      int c = rand()%3;
      switch (c) {
      case 0:
        r_ = 200;
        break;
      case 1:
        g_ = 200;
        break;
      case 2:
        b_ = 200;
        break;
      }
    }
}

ShowBouncingBalls::~ShowBouncingBalls()
{
    for (int x=0; x<width_; ++x) {
      delete [] values_[x];
    }
    delete [] values_;
    for (int x=0; x<width_; ++x) {
      delete [] newValues_[x];
    }
    delete [] newValues_;

}
void ShowBouncingBalls::startShow()
{
    while(_endTime > time(nullptr) && _running == true)
    {

        updateValues();
        for (int x=0; x<width_; ++x)
        {
            for (int y=0; y<height_; ++y)
            {
                if (values_[x][y])
                    _ledWrapper->setPixelColor(y, x, r_, g_, b_);
                else
                    _ledWrapper->setPixelColor(y, x, 0, 0, 0);
            }
        }

        _ledWrapper->show();
        Ws2811Wrapper::waitMillSec(_wait);
    }
}

int ShowBouncingBalls::numAliveNeighbours(int x, int y)
{
  int num=0;
  if (torus_) {
    // Edges are connected (torus)
    num += values_[(x-1+width_)%width_][(y-1+height_)%height_];
    num += values_[(x-1+width_)%width_][y                    ];
    num += values_[(x-1+width_)%width_][(y+1        )%height_];
    num += values_[(x+1       )%width_][(y-1+height_)%height_];
    num += values_[(x+1       )%width_][y                    ];
    num += values_[(x+1       )%width_][(y+1        )%height_];
    num += values_[x                  ][(y-1+height_)%height_];
    num += values_[x                  ][(y+1        )%height_];
  }
  else {
    // Edges are not connected (no torus)
    if (x>0) {
      if (y>0)
        num += values_[x-1][y-1];
      if (y<height_-1)
        num += values_[x-1][y+1];
      num += values_[x-1][y];
    }
    if (x<width_-1) {
      if (y>0)
        num += values_[x+1][y-1];
      if (y<31)
        num += values_[x+1][y+1];
      num += values_[x+1][y];
    }
    if (y>0)
      num += values_[x][y-1];
    if (y<height_-1)
      num += values_[x][y+1];
  }
  return num;
}

void ShowBouncingBalls::updateValues()
{
  // Copy values to newValues
  for (int x=0; x<width_; ++x) {
    for (int y=0; y<height_; ++y) {
      newValues_[x][y] = values_[x][y];
    }
  }
  // update newValues based on values
  for (int x=0; x<width_; ++x) {
    for (int y=0; y<height_; ++y) {
      int num = numAliveNeighbours(x,y);
      if (values_[x][y]) {
        // cell is alive
        if (num < 2 || num > 3)
          newValues_[x][y] = 0;
      }
      else {
        // cell is dead
        if (num == 3)
          newValues_[x][y] = 1;
      }
    }
  }
  // copy newValues to values
  for (int x=0; x<width_; ++x) {
    for (int y=0; y<height_; ++y) {
      values_[x][y] = newValues_[x][y];
    }
  }
}



/*

void ShowBouncingBalls::startShow()
{
    uint32_t snapshotBufferSize = 0;
    int  drawCol  = 0;
    _rowStart = 0;
    ws2811_led_t* snapShotBuffer = nullptr;



    QTime qtime;
    QString timeStr;

    snapShotBuffer = snapShot(_rowStart, CLOCKMAXROWS, &snapshotBufferSize);
    if(snapShotBuffer != nullptr)
    {
        drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;


        while(_endTime > time(nullptr) && _running == true)
        {
            timeStr = qtime.currentTime().toString().toStdString().c_str();

            for(int letter = 0; letter < timeStr.length(); letter++)
            {

                qDebug() << "val: " << (int)timeStr.toStdString().c_str()[letter] << "let: " << (int)/*timeStr.toStdString().c_str()[letter]
                if((int)timeStr.toStdString().c_str()[letter] < 48 || (int)timeStr.toStdString().c_str()[letter] > 59) //only attempt to print what we know.
                    continue;

                for(int col = CLOCKMAXCOLS; col < (CLOCKMAXCOLS * 2); col++)
                {
                    for(int row = 0; row < CLOCKMAXROWS; row++)
                    {


                        if(clockMatrix[(int)timeStr.toStdString().c_str()[letter] - 48][row * CLOCKMAXCOLS + (col - CLOCKMAXCOLS)] == 1) //should this pixal be on?
                        {
                            _ledWrapper->setPixelColor(row + _rowStart, drawCol , _color1);
                        }
                    }


                    _ledWrapper->show();

                    Ws2811Wrapper::waitMillSec(_wait);

                    shiftColumns(CLOCKMAXROWS, _rowStart, _color1, snapShotBuffer);

                }
            }

        }

        delete [] snapShotBuffer;
    }


}


*/
