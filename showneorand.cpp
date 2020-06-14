#include "showneorand.h"
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))


ShowNeoRand::ShowNeoRand(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms) :
    ILightShow(ledWrapper, lightShow, showParms)
{

    _rno = rand();
    _cno = 0;
}



void ShowNeoRand::startShow()
{

   int32_t tw;
   uint32_t saved = 0;     // saved colour (twinkle)
   unsigned int index;     // used to set colours, range 0-0x1FF
   u_int32_t cnt = 0;        // counter for pixel to set to new colour

   int fade[_ledWrapper->getNumberLeds()];  //fade increment
   int col[_ledWrapper->getNumberLeds()];    // holds colour values, shift>>6 to get index

   memset(&fade, 0, sizeof(fade));
   memset(&col, 0, sizeof(col));



   _rno = random9();
   for(int counter = 0; counter < _numLoops; counter++)
   {
     if (_running == false) return;
     for (u_int32_t i = 0; i < _ledWrapper->getNumberLeds(); i++)
     {

       if (_ledWrapper->getPixelColor(_settings->getStripHeight(), i) == _ledWrapper->Color(255,255,255))
           _ledWrapper->setPixelColor(_settings->getStripHeight(), i, saved); // return colour after sparkle


       int c = i % 7;          // c=c nominal colour 0-red, 1-orange, etc

       if (i == cnt)
       {
         _cno = random9();      // new colour setting (0 - 0x1FF)
         fade[i] = _cno - (col[i] >> 6); // new fade increment. 64 steps to reach cno
       }

       col[i] += fade[i];      // next setting

       index = col[i] >> 6;    // range 0-0x1FF

       switch (c) {          // update LED colours
         case 0:   // red
           _red = 255;
           _green = 0;
           _blue = 0;
           if (highByte(index) == 0)
           { // 0-0xFF
             _blue = (0xFF - lowByte(index)) >> 1; //max 127
           }
           else {                    // 0x100-0x1FF
             _green = lowByte(index) >> 2; //max 63
           }
           break;

         case 1:   // orange
           _red = 255;
           _green = 32;
           _blue = 0;
           if (highByte(index) == 0) { // 0-0xFF
             _green -= (0xFF - lowByte(index)) >> 3; //1 - 32
           }
           else {                    // 0x100-0x1FF
             _green += lowByte(index) >> 1;       //32 - 140
           }
           break;

         case 2:     // yellow
           _red = 255;
           _green = 127;
           _blue = 0;
           if (highByte(index) == 0) { // 0-0xFF
             _green -= (0xFF - lowByte(index)) >> 4; //64 - 127
           }
           else {                    // 0x100-0x1FF
             _green += lowByte(index);         //127 - 154
             if (_green > 0xFF) {
               _red -= _green - 0xFF;
               _green = 0xFF;
             }
           }
           break;

         case 3:     // green
           _red = 0;
           _green = 255;
           _blue = 0;
           if (highByte(index) == 0) { // 0-0xFF
             _red = 0xFF - lowByte(index); // 255 - 0
           }
           else {                    // 0x100-0x1FF
             _blue += lowByte(index) * 2;       //127 - 154
             if (_blue > 0xFF) {
               _green -= _blue - 0xFF;
               _blue = 0xFF;
             }
           }
           break;

         case 4:     // blue
           _red = 0;
           _green = 0;
           _blue = 255;
           if (highByte(index) == 0) { // 0-0xFF
             _green = (0xFF - lowByte(index)) * 2; // 255 - 0
             if (_green > 0xFF) {
               _blue -= _green - 0xFF;
               _green = 0xFF;
             }
           }
           else {                    // 0x100-0x1FF
             _red = lowByte(index) >> 2;       // red 0-64
           }
           break;

         case 5:     // indigo
           _red = 40;
           _green = 0;
           _blue = 255;
           if (highByte(index) == 0) { // 0-0xFF
             _red -= (0xFF - lowByte(index)) >> 2; // 255 - 0

           }
           else {                    // 0x100-0x1FF
             _red += lowByte(index) >> 1;       // red 40-255
             if (_red > 0xFF) {
               _blue -= _red - 0xFF;
               _red = 0xFF;
             }
           }
           break;
         case 6:   // violet
           _red = 255;
           _green = 0;
           _blue = 80;
           if (highByte(index) == 0) { // 0-0xFF
             _blue += (0xFF - lowByte(index)) >> 1; // 255 - 0
             if (_blue > 0xFF) {
               _red -= _blue - 0xFF;
               _blue = 0xFF;
             }
           }
           else {                    // 0x100-0x1FF
             _blue -= lowByte(index) >> 1;       // red 40-255

           }
           break;

         default:                  // should not get here
           _red = 0;
           _green = 0;
           _blue = 0;
       }

       _ledWrapper->setPixelColor(_settings->getStripHeight(), i, _ledWrapper->Color(_red, _green, _blue)); // set colour

     }  //  for(int i=0;i<NUMPIXELS;i++){

     cnt += 1;
     if (cnt >= _ledWrapper->getNumberLeds()) cnt = 0;


     tw = _cno - 0xDF;

     if (tw < (int)_ledWrapper->getNumberLeds() && tw == 0 )
     {
       _ledWrapper->setPixelColor(_settings->getStripHeight(), tw, _ledWrapper->Color(255,255,255));
     }


     _ledWrapper->show();    // This sends the updated pixel colors to the string.
     if(_running == false)
         return;

   }



}


uint32_t ShowNeoRand::Bow(int n)
{

  _cno = int(n / 64);     // find LED colour (0-6)

  _rno = n - (_cno * 64);  // index from LED colour to next colour (0-63)

  switch (_cno) {          // update LED colours
    case 0:   // red
      _red = 255;
      _blue = 0;

      _green = _rno >> 1;   // use shift for div 2
      break;

    case 1:   // orange
      _red = 255;
      _blue = 0;

      _green = 32 + ((_rno * 3) >> 1);
      break;

    case 2:     // yellow
      _red = 255;
      _blue = 0;

      _green = 128 + (_rno * 6);

      if (_green > 255) {
        _red -= (_green - 255);
        _green = 255;
      }
      break;

    case 3:     // green
      _red = 0;
      _green = 255;

      _blue = _rno * 8;

      if (_blue > 255) {
        _green -= (_blue - 255);
        _blue = 255;
      }
      break;

    case 4:     // blue
      _green = 0;
      _blue = 255;

      _red = _rno;
      break;

    case 5:     // indigo
      _green = 0;
      _blue = 255;

      _red = 64 + ((11 * _rno) >> 1);

      if (_red > 255) {
        _blue -= (_red - 255);
        _red = 255;
      }
      break;
    case 6:     // violet
      _red = 255;
      _green = 0;

      _blue = 96 - ((_rno * 3) >> 1);

      break;

    default:                  // should not get here
      _red = 0;
      _green = 0;
      _blue = 0;
  }

    return _ledWrapper->Color(_red, _green, _blue);

}

unsigned int ShowNeoRand::random9()
{

  unsigned int newbit = (((_rno >> 8) ^ (_rno >> 4)) & 1);
  _rno = ((_rno << 1) | newbit) & 0x1FF;
  return _rno;
}


