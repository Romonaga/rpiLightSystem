#include "showbouncingballs.h"
#include <QVector>
#include <math.h>
#include <QDebug>

ShowBouncingBalls::ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

}

void ShowBouncingBalls::startShow()
{
    float Gravity = -9.81;
  int StartHeight = 1;

  float Height[_width];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[_width];
  float TimeSinceLastBounce[_width];
  int   Position[_width];
  long  ClockTimeSinceLastBounce[_width];
  float Dampening[_width];

  for (int i = 0 ; i < _width ; i++)
  {
    ClockTimeSinceLastBounce[i] = time(nullptr);
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(_width,2);
  }

  while(_endTime > time(nullptr))
  {
    if(_running ==  false)
      return;

    for (int i = 0 ; i < _width ; i++)
    {
      TimeSinceLastBounce[i] =  time(nullptr) - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;

      if ( Height[i] < 0 )
      {
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = time(nullptr);

        if ( ImpactVelocity[i] < 0.01 )
        {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (_ledWrapper->getNumberLeds() - 1) / StartHeight);
      qDebug() << "Position: " <<  Position[i] << " Height: "  << Height[i] << " I: " << i << " width: " << _width;
    }

    for (int i = 0 ; i < _width ; i++)
    {
        //qDebug() << Position[i];
       _ledWrapper->setPixelColor(_settings->getStripRows(),Position[i],_color1);
    }
    _ledWrapper->show();

    _ledWrapper->clearLeds();
  }
}
/*
void ShowBouncingBalls::startShow()
{

    time_t start_time = time(nullptr);
    QVector<time_t> clockTimeSinceLastBounce;
    QVector<time_t> timeSinceLastBounce;


    QVector<int> height;
    QVector<int> position;
    QVector<int> impactVelocity;

    QVector<int> dampening;


    int startHeight = 1;
    double impactVelocityStart = sqrt(-2 * -9.81 * 1);


    time_t last_ClockTimeSinceLastBounce = start_time;

    for(int counter = 0; counter < _width; counter++)
    {
        clockTimeSinceLastBounce.append(start_time - last_ClockTimeSinceLastBounce);
        height.append(startHeight);
        position.append(0);
        impactVelocity.append(sqrt(-2 * -9.81 * 1));
        timeSinceLastBounce.append(0);

        dampening.append(0.90 - (( double) counter / pow(_width, 2)));

    }


    while(_endTime > time(nullptr))
    {
        int ballCount = 0;
        for(ballCount = 0; ballCount < _width; ballCount++)
        {
            timeSinceLastBounce[ballCount] = time(nullptr) - clockTimeSinceLastBounce[ballCount];
            height[ballCount] = 0.5 * (-9.81) *  pow(timeSinceLastBounce[ballCount],2) + impactVelocity[ballCount] * timeSinceLastBounce[ballCount];
            if(height[ballCount] < 0)
                height[ballCount] = 0;

            impactVelocity[ballCount] = dampening[ballCount] * impactVelocity[ballCount];
            clockTimeSinceLastBounce[ballCount] = time(nullptr);

            if (impactVelocity[ballCount] < 0.01)
                impactVelocity[ballCount] = impactVelocityStart;
        }

        position[ballCount] = round(height[ballCount] * (_ledWrapper->getNumberLeds()-1)/startHeight);


        for(int i= 0; i < _width; i++)
            _ledWrapper->setPixelColor(_settings->getStripHeight(), position[i],_color1);

        _ledWrapper->show();

        for(int i= 0; i < _width; i++)
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i, _color1);


        for(u_int32_t i = 0; i < _ledWrapper->getNumberLeds() ; i++)
            _ledWrapper->setPixelColor(_settings->getStripHeight(), i, 255,0,0);


         Ws2811Wrapper::waitMillSec(_wait);

    }
}

*/
/*
 *
 *
 * def bouncing_balls(strip, playtime, ball_count=2, wait_ms=200):
    """Shows an accelerated pixel with physicslike a ball in a flipper game"""
    import time, math
    start_time = time.time()
    ClockTimeSinceLastBounce = [0 for i in range(ball_count)]
    StartHeight=1

    for i in range(ball_count):
        ClockTimeSinceLastBounce[i] = time.time()

    Height = [0 for i in range(ball_count)]
    Position = [0 for i in range(ball_count)]
    ImpactVelocity = [0 for i in range(ball_count)]
    ImpactVelocityStart= math.sqrt(-2 * -9.81 * 1)
    Dampening = [0 for i in range(ball_count)]
    TimeSinceLastBounce = [0 for i in range(ball_count)]

    for i in range(0,ball_count,1):
        last_ClockTimeSinceLastBounce = ClockTimeSinceLastBounce[i]
        ClockTimeSinceLastBounce[i] = time.time() - last_ClockTimeSinceLastBounce

        Height[i] = StartHeight
        Position[i] = 0
        ImpactVelocity[i] = math.sqrt(-2 * -9.81 * 1)
        TimeSinceLastBounce[i] = 0
        Dampening[i] = 0.90 - (float(i)/(ball_count**2))
    act_time = time.time()
    while ((act_time+ playtime)> time.time()):
        for i in range(ball_count):
            TimeSinceLastBounce[i] = time.time() - ClockTimeSinceLastBounce[i]
            Height[i] = 0.5 * (-9.81) * (TimeSinceLastBounce[i]**2) + ImpactVelocity[i] * TimeSinceLastBounce[i]
            if (Height[i] < 0):
                Height[i] = 0
                ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i]
                ClockTimeSinceLastBounce[i] = time.time()
                if (ImpactVelocity[i] < 0.01):
                    ImpactVelocity[i] = ImpactVelocityStart

            Position[i] = round(Height[i] * (strip.numPixels()-1)/StartHeight)
        for i in range(ball_count):
            strip.setPixelColorRGB(Position[i], 0, 0,255)
        strip.show()
        for i in range(strip.numPixels()):
            strip.setPixelColorRGB(i, 0,0,0)
 *
 * */
