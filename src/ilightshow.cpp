#include "ilightshow.h"
#include <math.h>

#include <QDebug>
#include <random>
#include <math.h>

#include <stdio.h>
#include <sys/types.h>



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

           if(_showParmsJson["c"].isObject())
           {
               jsonColors = _showParmsJson["c"].toObject();

               if(jsonColors["c4"].isString())
               {
                  _color4 = std::stoul(jsonColors["c4"].toString().replace("#","0x").toStdString().c_str(), nullptr, 16);

               }

               if(jsonColors["c3"].isString())
               {
                  _color3 = std::stoul(jsonColors["c3"].toString().replace("#","0x").toStdString().c_str(), nullptr, 16);
               }

               if(jsonColors["c2"].isString())
               {
                  _color2 = std::stoul(jsonColors["c2"].toString().replace("#","0x").toStdString().c_str(), nullptr, 16);
               }

               if(jsonColors["c1"].isString())
               {
                   jsonColor = jsonColors["c1"].toObject();
                  _color1 = std::stoul(jsonColors["c1"].toString().replace("#","0x").toStdString().c_str(), nullptr, 16);

                  _r  =_ledWrapper->Red(_color1);
                  _g = _ledWrapper->Green(_color1);
                  _b = _ledWrapper->Blue(_color1);

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
        case 9:
        case 10:
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


    if (x == 0)
    {
        _ledWrapper->setPixelColor( cx, cy + y, _color1);
        _ledWrapper->setPixelColor(cx, cy - y, _color1);
        _ledWrapper->setPixelColor(cx + y, cy, _color1);
        _ledWrapper->setPixelColor(cx - y, cy, _color1);
    }
    else if (x == y)
    {
        _ledWrapper->setPixelColor(cx + x, cy + y,_color1);
        _ledWrapper->setPixelColor(cx - x, cy + y, _color1);
        _ledWrapper->setPixelColor(cx + x, cy - y, _color1);
        _ledWrapper->setPixelColor(cx - x, cy - y, _color1);
    }
    else if (x < y)
    {
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
    while (x < y)
    {
        x++;
        if (p < 0)
        {
            p += 2*x+1;
        } else
        {
            y--;
            p += 2*(x-y)+1;
        }
        circlePoints(xCenter, yCenter, x, y);
    }
}



void ILightShow::drawTriangle(int startRow, int startCol, int size, int direction)
{

    switch((TrigDirection)direction)
    {
        case PointUp:
            drawline(startRow - 1, startCol - 1, (startRow - 1 ) + size, (startCol - 1) - size);
            drawline(startRow - 1, startCol - 1, (startRow - 1)  + size, (startCol - 1) + size);
            drawline( (startRow - 1) + (size - 1), (startCol - size) + 1, (startRow - 1) + (size - 1), (size - 1) + (startCol - 1));
            break;

        case PointRight:
            drawline(startRow - 1 , startCol - 1, startRow - size, (startCol - 1) - size);
            drawline(startRow - 1 , startCol - 1, (startRow - 1) + size, (startCol - 1) - size);
            drawline((startRow - 1) - (size - 1), startCol - size  , (startRow - 1) + size, (startCol - 1) - (size - 1));
            break;

        case PointDown:
            drawline(startRow - 1, startCol - 1, startRow - size, (startCol - 1) - size);
            drawline(startRow - 1, startCol - 1, startRow - size, (startCol - 1) + size);
            drawline(startRow - size, (startCol) - size + 1, startRow - size, (size + startCol) - 2);
            break;


        case PointLeft:
            drawline(startRow - 1, startCol - 1, (startRow - 1 ) - size, (startCol - 1) + size);
            drawline(startRow - 1, startCol - 1, (startRow - 1)  + size, (startCol - 1) + size);
            drawline( (startRow - size) + 1, (startCol - 1) + (size - 1), (startRow - 1) + (size - 1), (startCol - 1) + (size - 1));
            break;


    }



}


void ILightShow::drawline(int x1, int y1, int x2, int y2)
{
    float x, y, dx, dy, step;
    int i;

    dx = (x2 - x1);
    dy = (y2 - y1);

    if (abs(dx) >= abs(dy))
        step = abs(dx);
    else
        step = abs(dy);

    dx = dx / step;
    dy = dy / step;

    x = x1;
    y = y1;
    i = 1;

    while (i <= step)
    {

        _ledWrapper->setPixelColor(x, y, _color1);
        x = x + dx;
        y = y + dy;
        i = i + 1;

    }

}

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
void ILightShow::shiftColumns(int maxRows, int rowStart, ws2811_led_t color, ws2811_led_t *snapShotBuffer)
{
    ws2811_led_t drawColor;

    if(snapShotBuffer == nullptr) return;

    int past = 0;
    for(int current = 0; current  < _settings->getChannels()[_channelId]->stripColumns() - 1 ; current ++)
    {

        for(int row = _rowStart; row < (maxRows + rowStart); row++)
        {
            past = current + 1;

            drawColor = _ledWrapper->getPixelColor(row, past);                                                              //init drawColor to past color

            if( (drawColor != color && _ledWrapper->getPixelColor(row, current) != color) )                             //replay saved color if needed
                drawColor = snapShotBuffer[( (row - rowStart) * _settings->getChannels()[_channelId]->stripColumns()) + current];

            _ledWrapper->setPixelColor(row , current, drawColor);                                                           // move to present
            _ledWrapper->setPixelColor(row , past, snapShotBuffer[( (row - rowStart) * _settings->getChannels()[_channelId]->stripColumns()) + past]);     //set  past's past color to saved color (yes, I know)..


        }
    }

    _ledWrapper->show();
}



//all we are doing here is captruing our drawing area.  As it is defined,
// we dont need to deal with the complete matrix..  Just what we touch.
ws2811_led_t *ILightShow::snapShot(int rowStart, int maxRows, int numCols, uint32_t* snapShotBufferSize)
{
    ws2811_led_t* snapShotBuffer = nullptr;

    //We only care about what we might overlay.
    *snapShotBufferSize = maxRows * numCols;
    if(*snapShotBufferSize > 0)
    {
        snapShotBuffer = new ws2811_led_t[*snapShotBufferSize];
        for(int col = 0; col < numCols; col++)
        {
            for(int row = 0; row < maxRows ; row++)
                snapShotBuffer[ (row  * numCols) + col]  = _ledWrapper->getPixelColor(row + rowStart, col);
        }
    }

    return snapShotBuffer;
}

//This will reprint the snapshot so to speak.  Here we will
//simply get the picture and repaint it, to put the picture back to how
//it was before we touched it
void ILightShow::replaySnapShot(int rowStart, int maxRows, int numCols, ws2811_led_t *snapShotBuffer)
{
    if(snapShotBuffer == nullptr) return;

    for(int col = 0; col < numCols; col++)
    {
        for(int row = 0; row < maxRows; row++)
            _ledWrapper->setPixelColor(row + rowStart, col, snapShotBuffer[ (row  * numCols) + col]);

    }

    _ledWrapper->show();
}

ws2811_led_t*  ILightShow::resampleColor(int newWidth,  int newHeight, int width, int height, ws2811_led_t* imageData)
{
    int i;
    int j;
    int l;
    int c;
    float t;
    float u;
    float tmp;
    float d1, d2, d3, d4;
    u_int p1, p2, p3, p4; /* nearby pixels */
    u_char red, green, blue;
    ws2811_led_t *imageDataOut = nullptr;

    //sanity checks to prevent crashes, one never knows with my math.
    if(newWidth * newHeight <= 0) return imageDataOut;
    if(width * height <= 0) return imageDataOut;
    if(imageData == nullptr) return imageDataOut;

    imageDataOut = new ws2811_led_t[newWidth * newHeight];
    for (i = 0; i < newHeight; i++)
    {
        for (j = 0; j < newWidth; j++)
        {

            tmp = (float) (i) / (float) (newHeight - 1) * (height - 1);
            l = (int) floor(tmp);
            if (l < 0)
            {
                l = 0;
            } else
            {
                if (l >= height - 1)
                {
                    l = height - 2;
                }
            }

            u = tmp - l;
            tmp = (float) (j) / (float) (newWidth - 1) * (width - 1);
            c = (int) floor(tmp);
            if (c < 0)
            {
                c = 0;
            } else
            {
                if (c >= width - 1)
                {
                    c = width - 2;
                }
            }
            t = tmp - c;

            /* coefficients */
            d1 = (1 - t) * (1 - u);
            d2 = t * (1 - u);
            d3 = t * u;
            d4 = (1 - t) * u;

            /* nearby pixels: a[i][j] */
            p1 = *((u_int*)imageData + (l * width) + c);
            p2 = *((u_int*)imageData + (l * width) + c + 1);
            p3 = *((u_int*)imageData + ((l + 1)* width) + c + 1);
            p4 = *((u_int*)imageData + ((l + 1)* width) + c);

            /* color components */
            blue = (u_char)p1 * d1 + (u_char)p2 * d2 + (u_char)p3 * d3 + (u_char)p4 * d4;
            green = (u_char)(p1 >> 8) * d1 + (u_char)(p2 >> 8) * d2 + (u_char)(p3 >> 8) * d3 + (u_char)(p4 >> 8) * d4;
            red = (u_char)(p1 >> 16) * d1 + (u_char)(p2 >> 16) * d2 + (u_char)(p3 >> 16) * d3 + (u_char)(p4 >> 16) * d4;

            /* new pixel R G B  */
            *((u_int*)imageDataOut + (i * newWidth) + j) = (red << 16) | (green << 8) | (blue);
        }
    }
    return imageDataOut;
}

//You ARE responsable for this memory allocation!
unsigned char* ILightShow::resampleRGB(int newWidth, int newHeight, int width, int height, unsigned char* imageData)
{
    if(imageData == nullptr) return nullptr;

    // Get a new buuffer to interpolate into
    unsigned char* newData = new unsigned char [newWidth * newHeight * 3];

    double scaleWidth =  (double)newWidth / (double)width;
    double scaleHeight = (double)newHeight / (double)height;

    for(int cy = 0; cy < newHeight; cy++)
    {
        for(int cx = 0; cx < newWidth; cx++)
        {
            int pixel = (cy * (newWidth *3)) + (cx*3);
            int nearestMatch =  (((int)(cy / scaleHeight) * (width *3)) + ((int)(cx / scaleWidth) *3) );

            newData[pixel    ] =  imageData[nearestMatch    ];
            newData[pixel + 1] =  imageData[nearestMatch + 1];
            newData[pixel + 2] =  imageData[nearestMatch + 2];
        }
    }

    return newData;
}


void ILightShow::scrollText(QString msg, int maxRows, int maxCols, int rowStart, int drawCol, ws2811_led_t color,  ws2811_led_t *snapShotBuffer, int delay)
{

    if(snapShotBuffer == nullptr) return;

    for(int letter = 0; letter < msg.length(); letter++)
    {

        if((int)msg.toStdString().c_str()[letter] < 32 || (int)msg.toStdString().c_str()[letter] > 122) //only attempt to print what we know.
            continue;


        for(int col = maxCols; col < (maxCols * 2); col++)
        {
            for(int row = 0; row < maxRows; row++)
            {

                if(textMatrix[(int)msg.toStdString().c_str()[letter] - 32][row * maxCols + (col - maxCols)] == 1) //should this pixal be on?
                    _ledWrapper->setPixelColor(row + rowStart, drawCol , color);
            }


            _ledWrapper->show();

            Ws2811Wrapper::waitMillSec(delay);

            shiftColumns(maxRows, rowStart, color, snapShotBuffer);

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
