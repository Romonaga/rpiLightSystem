#include "ilightshow.h"
#include <math.h>

#include <QDebug>
#include <random>
#include <math.h>


ILightShow::ILightShow(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString& showParms) :
        _ledWrapper(ledWrapper), _lightShow(lightShow), _showParms(showParms)
{
    _logger = DNRLogger::instance();
    _settings = SystemSettings::getInstance();

    srand(time(nullptr));
    _userId = 1;
    _running = false;
    _wait = 100;
    _clearOnStart = false;
    _clearOnFinish = false;
    _numMins = 1;
    _width = 5;
    _colorEvery = 2;
    _channelId = 0;
    _color1 = _ledWrapper->Color(125, 125, 125);
    _color2 = _ledWrapper->Color(0, 125, 125);
    _color3 = _ledWrapper->Color(125, 125, 0);
    _color4 = _ledWrapper->Color(125, 0, 125);
    _rowStart = 0;
    _drawCol = 0;

    QJsonObject jsonColors;
    QJsonObject jsonColor;
    _image = nullptr;

   QJsonDocument doc = QJsonDocument::fromJson(showParms.toUtf8());

   if(!doc.isNull())
   {
       if(doc.isObject())
       {



            _showParmsJson = doc.object();

            if(_showParmsJson.value("UserID").isString())
                _userId = _showParmsJson.value("UserID").toString().toInt();

            if(_showParmsJson.value("brightness").isString())
                _brightness = _showParmsJson.value("brightness").toString().toInt();

            if(_showParmsJson.value("delay").isString())
                _wait = _showParmsJson.value("delay").toString().toInt();

            _clearOnStart = (bool)_showParmsJson.value("clearStart").toInt();
            _clearOnFinish = (bool)_showParmsJson.value("clearFinish").toInt();
            _useGammaCorrection = (bool)_showParmsJson.value("gammaCorrection").toInt();

            _channelId = (_showParmsJson.value("channelId").isString()) ? _showParmsJson.value("channelId").toString().toInt() - 1 : 0;

            if(_showParmsJson.value("minutes").isString())
                _numMins = _showParmsJson.value("minutes").toString().toDouble();

            if(_showParmsJson.value("width").isString())
                _width = _showParmsJson.value("width").toString().toInt();

            if(_showParmsJson.value("colorEvery").isString())
                _colorEvery = _showParmsJson.value("colorEvery").toString().toInt();

            if(_showParmsJson.value("matrixText").isString())
                _matrixText = _showParmsJson.value("matrixText").toString();

           if(_showParmsJson.value("position").isString())
           {
               _rowStart =  (_settings->getChannels()[_channelId]->stripRows()/ 2) - (MAXROWS / 2) - 1;
               _drawCol = _settings->getChannels()[_channelId]->stripColumns() - 1;


               switch(_showParmsJson.value("position").toString().toInt())
               {

               case 1:
                    _rowStart = 0;
                   break;

               case 2:
                    _rowStart =  (_settings->getChannels()[_channelId]->stripRows() / 2) - (MAXROWS / 2) - 1;
                   break;

               case 3:
                   _rowStart = _settings->getChannels()[_channelId]->stripRows() - MAXROWS;
                   break;


               }
           }

           if(_showParmsJson["colors"].isObject())
           {
               jsonColors = _showParmsJson["colors"].toObject();

               if(jsonColors["color4"].isObject())
               {
                   jsonColor = jsonColors["color4"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();

                  _color4 = _ledWrapper->Color(_r, _g, _b);

               }

               if(jsonColors["color3"].isObject())
               {
                   jsonColor = jsonColors["color3"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color3 = _ledWrapper->Color(_r, _g, _b);

               }

               if(jsonColors["color2"].isObject())
               {
                   jsonColor = jsonColors["color2"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color2 = _ledWrapper->Color(_r, _g, _b);

               }

               if(jsonColors["color1"].isObject())
               {
                   jsonColor = jsonColors["color1"].toObject();
                   _r = jsonColor.value("r").toInt();
                   _g = jsonColor.value("g").toInt();
                   _b = jsonColor.value("b").toInt();
                  _color1 = _ledWrapper->Color(_r, _g, _b);

               }

           }


       }
       else
       {
           _logger->logWarning("ILightShow::ShowFire Invalid JSON");
       }
   }
   else
   {
       _logger->logWarning("ILightShow::ShowFire Document is not an object");
   }

}

ILightShow::~ILightShow()
{
    if(_image != nullptr)
        delete [] _image;
}

void ILightShow::run()
{
    _running = true;
    if(_clearOnStart)
        _ledWrapper->clearLeds();

    _ledWrapper->setCurChannel((ws2811Channel)_channelId);

    _settings->getChannels()[_channelId]->setBrightness(_brightness);
     gammaCorrection();

    _endTime =  time(nullptr) + (_numMins * 60);

    startShow();

  _running = false;

    if(_clearOnFinish)
        _ledWrapper->clearLeds();

     emit showComplete(this);



}

void ILightShow::stopShow()
{
    _running = false;
    wait();
}

LedLightShows ILightShow::getLightShow() const
{
    return _lightShow;
}


void ILightShow::gammaCorrection()
{

   /* for(int  counter = 0; counter <= 255; counter++)
    {
        _gammaCorrection[counter] = (!_useGammaCorrection) ? counter :
                (int)(pow((float)counter / (float)255.00, _settings->getGamma()) * 255.00 + 0.5);
    }
*/
    if(true == _useGammaCorrection)
        _ledWrapper->setCustomGammaCorrection(_settings->getChannels()[_channelId]->gamma());
      else
        _ledWrapper->setCustomGammaCorrection(0);

}


QString ILightShow::getShowName()
{
    const char* lightShowNames[] = {LIGHT_SHOWS(LIGHT_SHOWS_SHOW)};
    return lightShowNames[_lightShow];
}

QString ILightShow::getShowParms() const
{
    return _showParms;
}

bool ILightShow::isRunning() const
{
    return _running;
}


int ILightShow::genRand(unsigned int min, unsigned int max)
{
    std::random_device seeder;
    std::mt19937 engine(seeder());
    std::uniform_int_distribution<unsigned int> dist(min, max);
    return dist(engine);

}

int ILightShow::getUserId() const
{
    return _userId;
}


ws2811_led_t ILightShow::gamaColor(ws2811_led_t inColor)
{
    return (ws2811_led_t) ((inColor / 255) ^ (ws2811_led_t)(1 / _settings->getChannels()[_channelId]->gamma())) * 255;
}


void ILightShow::drawCircle(int xCenter, int yCenter, int radius)
{

    switch(radius)
    {
        case 1:
        case 3:
        case 4:
        case 6:
            circleMidpoint(xCenter, yCenter, radius);
           break;

        case 2:
        case 5:
        case 7:
        case 8:
            circleBres(xCenter, yCenter, radius);
            break;

        default:
            circleMidpoint(xCenter, yCenter, radius);
            break;


    }

}


void ILightShow::drawCircle(int xc, int yc, int x, int y)
{


    _ledWrapper->setPixelColor(xc+x, yc+y, _color1);
    _ledWrapper->setPixelColor(xc-x, yc+y, _color1);
    _ledWrapper->setPixelColor(xc+x, yc-y, _color1);
    _ledWrapper->setPixelColor(xc-x, yc-y, _color1);
    _ledWrapper->setPixelColor(xc+y, yc+x, _color1);
    _ledWrapper->setPixelColor(xc-y, yc+x, _color1);
    _ledWrapper->setPixelColor(xc+y, yc-x, _color1);
    _ledWrapper->setPixelColor(xc-y, yc-x, _color1);

}



void ILightShow::circleBres(int xc, int yc, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y);
    while (y >= x)
    {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(xc, yc, x, y);

    }
}


void ILightShow::circlePoints(int cx, int cy, int x, int y)
{


    if (x == 0) {
        _ledWrapper->setPixelColor( cx, cy + y, _color1);
        _ledWrapper->setPixelColor(cx, cy - y, _color1);
        _ledWrapper->setPixelColor(cx + y, cy, _color1);
        _ledWrapper->setPixelColor(cx - y, cy, _color1);
    } else
    if (x == y) {
        _ledWrapper->setPixelColor(cx + x, cy + y,_color1);
        _ledWrapper->setPixelColor(cx - x, cy + y, _color1);
        _ledWrapper->setPixelColor(cx + x, cy - y, _color1);
        _ledWrapper->setPixelColor(cx - x, cy - y, _color1);
    } else
    if (x < y) {
        _ledWrapper->setPixelColor(cx + x, cy + y, _color1);
        _ledWrapper->setPixelColor(cx - x, cy + y, _color1);
        _ledWrapper->setPixelColor(cx + x, cy - y, _color1);
        _ledWrapper->setPixelColor(cx - x, cy - y, _color1);
        _ledWrapper->setPixelColor(cx + y, cy + x, _color1);
        _ledWrapper->setPixelColor(cx - y, cy + x, _color1);
        _ledWrapper->setPixelColor(cx + y, cy - x, _color1);
        _ledWrapper->setPixelColor(cx - y, cy - x, _color1);
    }
}


void ILightShow::circleMidpoint(int xCenter, int yCenter, int radius)
{
    int x = 0;
    int y = radius;
    int p = (5 - radius*4)/4;

    circlePoints(xCenter, yCenter, x, y);
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2*x+1;
        } else {
            y--;
            p += 2*(x-y)+1;
        }
        circlePoints(xCenter, yCenter, x, y);
    }
}


/*
 *
 *  void dda_circle(int xc,int yc,int r)
{

   float xc1,xc2,yc1,yc2,eps,sx,sy;

  int val,i;

  xc1=r;

  yc1=0;

  sx=xc1;

  sy=yc1;

  i=0;

  do{

      val=pow(2,i);

      i++;

      }while(val<r);

  eps = 1/pow(2,i-1);

  do{

      xc2 = xc1 + yc1*eps;
      yc2 = yc1 - eps*xc2;

      putpixel(xc+xc2,yc-yc2,3);

      xc1=xc2;

      yc1=yc2;

     }while((yc1-sy)<eps || (sx-xc1)>eps);

}

private final void circlePoints(int cx, int cy, int x, int y, int pix)
    {
        int act = Color.red.getRGB();

        if (x == 0) {
            raster.setPixel(act, cx, cy + y);
            raster.setPixel(pix, cx, cy - y);
            raster.setPixel(pix, cx + y, cy);
            raster.setPixel(pix, cx - y, cy);
        } else
        if (x == y) {
            raster.setPixel(act, cx + x, cy + y);
            raster.setPixel(pix, cx - x, cy + y);
            raster.setPixel(pix, cx + x, cy - y);
            raster.setPixel(pix, cx - x, cy - y);
        } else
        if (x < y) {
            raster.setPixel(act, cx + x, cy + y);
            raster.setPixel(pix, cx - x, cy + y);
            raster.setPixel(pix, cx + x, cy - y);
            raster.setPixel(pix, cx - x, cy - y);
            raster.setPixel(pix, cx + y, cy + x);
            raster.setPixel(pix, cx - y, cy + x);
            raster.setPixel(pix, cx + y, cy - x);
            raster.setPixel(pix, cx - y, cy - x);
        }
    }

    public void circleMidpoint(int xCenter, int yCenter, int radius, Color c)
    {
        int pix = c.getRGB();
        int x = 0;
        int y = radius;
        int p = (5 - radius*4)/4;

        circlePoints(xCenter, yCenter, x, y, pix);
        while (x < y) {
            x++;
            if (p < 0) {
                p += 2*x+1;
            } else {
                y--;
                p += 2*(x-y)+1;
            }
            circlePoints(xCenter, yCenter, x, y, pix);
        }
    }


void ILightShow::midPointCircleDraw(int x_centre, int y_centre, int r)
{
    int x = r, y = 0;

    // Printing the initial point on the axes
    // after translation
    _ledWrapper->setPixelColor(x + x_centre, y + y_centre, _color2);


    // When radius is zero only a single
    // point will be printed
    if (r > 0)
    {
        _ledWrapper->setPixelColor(x + x_centre, -y + y_centre, _color2);
        _ledWrapper->setPixelColor(y + x_centre, x + y_centre, _color2);
        _ledWrapper->setPixelColor(-y + x_centre, x + y_centre, _color2);
    }

    // Initialising the value of P
    int P = 1 - r;
    while (x > y)
    {
        y++;

        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2*y + 1;
        // Mid-point is outside the perimeter
        else
        {
            x--;
            P = P + 2*y - 2*x + 1;
        }

        // All the perimeter points have already been printed
        if (x < y)
            break;

        // Printing the generated point and its reflection
        // in the other octants after translation
        _ledWrapper->setPixelColor(x + x_centre ,  y + y_centre, _color2);
        _ledWrapper->setPixelColor(-x + x_centre,  y + y_centre, _color2);
        _ledWrapper->setPixelColor(x + x_centre ,  -y + y_centre, _color2);
        _ledWrapper->setPixelColor(-x + x_centre,  -y + y_centre, _color2);

        // If the generated point is on the line x = y then
        // the perimeter points have already been printed
        if (x != y)
        {
            _ledWrapper->setPixelColor(y + x_centre, x + y_centre, _color2);
            _ledWrapper->setPixelColor(-y + x_centre, x + y_centre, _color2);
            _ledWrapper->setPixelColor(y + x_centre, -x + y_centre, _color2);
            _ledWrapper->setPixelColor(-y + x_centre, -x + y_centre, _color2);
        }
    }
}

*/

void ILightShow::drawBox(int startRow, int startcol, int length, int height)
{
    int col = 0;
    int row = 0;

    for(col = startcol; col < (startcol + length) ; col++)
        _ledWrapper->setPixelColor(startRow, col, _color1);

    for(row = startRow; row < (startRow + height) ; row++)
        _ledWrapper->setPixelColor(row, col - 1, _color1);

    for(col = col; col > startcol ; col--)
        _ledWrapper->setPixelColor(row, col - 1, _color1);

    for(row = row; row > startRow; row--)
        _ledWrapper->setPixelColor(row, col, _color1);

}



//logic is clear, but we are walking from where we drew the row
//and we are moving that row forward, and preserving the original color, if it is not a letter's color
void ILightShow::shiftColumns()
{
    ws2811_led_t drawColor;

    int past = 0;
    for(int current = 0; current  < _settings->getChannels()[_channelId]->stripColumns() - 1 ; current ++)
    {

        for(int row = _rowStart; row < (MAXROWS + _rowStart); row++)
        {
            past = current + 1;

            drawColor = _ledWrapper->getPixelColor(row, past);                                                              //init drawColor to past color

            if( (drawColor != _color1 && _ledWrapper->getPixelColor(row, current) != _color1) )                             //replay saved color if needed
                drawColor = _image[( (row - _rowStart) * _settings->getChannels()[_channelId]->stripColumns()) + current];

            _ledWrapper->setPixelColor(row , current, drawColor);                                                           // move to present
            _ledWrapper->setPixelColor(row , past, _image[( (row - _rowStart) * _settings->getChannels()[_channelId]->stripColumns()) + past]);     //set  past's past color to saved color (yes, I know)..


        }
    }

    _ledWrapper->show();
}

//all we are doing here is captruing our drawing area.  As it is defined,
// we dont need to deal with the complete matrix..  Just what we touch.
void ILightShow::snapShot()
{

    //We only care about what we might overlay.
    _image = new ws2811_led_t[MAXROWS * _settings->getChannels()[_channelId]->stripColumns()];

    for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
    {
        for(int row = 0; row < MAXROWS ; row++)
            _image[ (row  * _settings->getChannels()[_channelId]->stripColumns()) + col]  = _ledWrapper->getPixelColor(row + _rowStart, col);
    }
}

//This will reprint the snapshot so to speak.  Here we will
//simply get the picture and repaint it, to put the picture back to how
//it was before we touched it
void ILightShow::replaySnapShot()
{
    for(int col = 0; col < _settings->getChannels()[_channelId]->stripColumns(); col++)
    {
        for(int row = 0; row < MAXROWS; row++)
            _ledWrapper->setPixelColor(row + _rowStart, col, _image[ (row  * _settings->getChannels()[_channelId]->stripColumns()) + col]);

    }

    _ledWrapper->show();

    delete [] _image;
    _image = nullptr;
}

void ILightShow::scrollText(QString msg, bool noDelay)
{


    for(int letter = 0; letter < msg.length(); letter++)
    {

        if((int)msg.toStdString().c_str()[letter] < 32 || (int)msg.toStdString().c_str()[letter] > 122) //only attempt to print what we know.
            continue;


        for(int col = MAXCOLS; col < (MAXCOLS * 2); col++)
        {
            for(int row = 0; row < MAXROWS; row++)
            {

                if(letterMatrix[(int)msg.toStdString().c_str()[letter] - 32][row * MAXCOLS + (col - MAXCOLS)] == 1) //should this pixal be on?
                    _ledWrapper->setPixelColor(row + _rowStart, _drawCol , _color1);
            }


            _ledWrapper->show();

            if(noDelay == false)
                Ws2811Wrapper::waitMillSec(_wait);

            shiftColumns();

        }
    }

}



//encoded = ((original / 255) ^ (1 / gamma)) * 255
//Gamma decoding is used to restore the original values, so the formula for that is:
//original = ((encoded / 255) ^ gamma) * 255



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
