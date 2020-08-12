// //////////////////////////////////////////////////////////
// toojpeg.cpp
// written by Stephan Brumme, 2018-2019
// see https://create.stephan-brumme.com/toojpeg/
//

//Forked and Modified By Robert Winslow.
//I forked this fantasitc library, however i needed to make it a bit more C++ like
//The only modification is the passing of a void pointer so I can pass the calling class in.
//As i needed to be able to reaccess the calling class.  Fantastic bit of work that saved me hours
//Of time.  Thank You sir for your hard work!


#include "toojpeg.h"

// notes:
// - the "official" specifications: https://www.w3.org/Graphics/JPEG/itu-t81.pdf and https://www.w3.org/Graphics/JPEG/jfif3.pdf
// - a short documentation of the JFIF/JPEG file format can be found in the Wikipedia: https://en.wikipedia.org/wiki/JPEG_File_Interchange_Format
// - the popular STB Image library includes Jon's JPEG encoder as well: https://github.com/nothings/stb/blob/master/stb_image_write.h
// - the most readable JPEG book (from a developer's perspective) is Miano's "Compressed Image File Formats" (1999, ISBN 0-201-60443-4),
//   used copies are really cheap nowadays and it includes a CD with C++ sources as well (plus detailled format descriptions of GIF+PNG)
// - much more detailled is Mitchell/Pennebaker's "JPEG: Still Image Data Compression Standard" (1993, ISBN 0-442-01272-1)
//   which contains the official JPEG standard, too - fun fact: I bought a signed copy in a second-hand store without noticing

namespace // anonymous namespace to hide local functions / constants / etc.
{

// ////////////////////////////////////////
// data types (for internal use only)

// one byte
typedef unsigned char   uint8_t;
// two bytes
typedef unsigned short uint16_t;
typedef          short  int16_t;
// four bytes (or more)
typedef          int    int32_t;

// ////////////////////////////////////////
// basic structs (for internal use only)

// represent a few bits, typically a Huffman code
struct BitCode
{
  BitCode() {}       // undefined state, must be initialized at a later time
  BitCode(uint16_t code_, uint8_t numBits_)
  : code(code_), numBits(numBits_) {}
  uint16_t code;     // JPEG's Huffman codes are limited to 16 bits
  uint8_t  numBits;  // actual number of bits
};

// store the most recently encoded bits that are not written yet
struct BitBuffer
{
  BitBuffer()        // actually, there will be only one instance of this object
  : bits(0), numBits(0) {}
  int32_t bits;      // actually only at most 24 bits are used
  uint8_t numBits;   // number of valid bits (the right-most bits)
};

// ////////////////////////////////////////
// constants (for internal use only)

// quantization tables from JPEG Standard, Annex K
// there are a few experts proposing slightly more efficient values, e.g. https://www.imagemagick.org/discourse-server/viewtopic.php?t=20333
// btw: Google's Guetzli project optimizes the quantization tables per image
const uint8_t DefaultQuantLuminance[8*8] =
    { 16, 11, 10, 16, 24, 40, 51, 61,
      12, 12, 14, 19, 26, 58, 60, 55,
      14, 13, 16, 24, 40, 57, 69, 56,
      14, 17, 22, 29, 51, 87, 80, 62,
      18, 22, 37, 56, 68,109,103, 77,
      24, 35, 55, 64, 81,104,113, 92,
      49, 64, 78, 87,103,121,120,101,
      72, 92, 95, 98,112,100,103, 99 };
const uint8_t DefaultQuantChrominance[8*8] =
    { 17, 18, 24, 47, 99, 99, 99, 99,
      18, 21, 26, 66, 99, 99, 99, 99,
      24, 26, 56, 99, 99, 99, 99, 99,
      47, 66, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99,
      99, 99, 99, 99, 99, 99, 99, 99 };

// 8x8 blocks are processed in zig-zag order
// most encoders use a zig-zag table, I switched to its inverse for performance reasons
// note: ZigZagInv[ZigZag[i]] = i
const uint8_t ZigZagInv[8*8] =
    {  0, 1, 8,16, 9, 2, 3,10,   // ZigZag[] =  0, 1, 5, 6,14,15,27,28,
      17,24,32,25,18,11, 4, 5,   //             2, 4, 7,13,16,26,29,42,
      12,19,26,33,40,48,41,34,   //             3, 8,12,17,25,30,41,43,
      27,20,13, 6, 7,14,21,28,   //             9,11,18,24,31,40,44,53,
      35,42,49,56,57,50,43,36,   //            10,19,23,32,39,45,52,54,
      29,22,15,23,30,37,44,51,   //            20,22,33,38,46,51,55,60,
      58,59,52,45,38,31,39,46,   //            21,34,37,47,50,56,59,61,
      53,60,61,54,47,55,62,63 }; //            35,36,48,49,57,58,62,63

// some constants for our DCT
const auto SqrtHalfSqrt = 1.306562965f; // sqrt((2 + sqrt(2)) / 2)  = cos(pi * 1 / 8) * sqrt(2)
const auto HalfSqrtSqrt = 0.382683432f; // sqrt( 2 - sqrt(2)) / 2   = cos(pi * 3 / 8)
const auto InvSqrt      = 0.707106781f; // 1 / sqrt(2)              = cos(pi * 2 / 8)
const auto InvSqrtSqrt  = 0.541196100f; // 1 / sqrt(2 - sqrt(2))    = cos(pi * 3 / 8) * sqrt(2)
// scaling constants for AAN DCT algorithm: AanScaleFactors[0] = 1, AanScaleFactors[k=1..7] = cos(k*PI/16) * sqrt(2)
const float AanScaleFactors[8] = { 1, 1.387039845f, SqrtHalfSqrt, 1.175875602f, 1, 0.785694958f, InvSqrtSqrt, 0.275899379f };

// use Huffman code tables recommended by the JPEG standard Annex K
// - they work quite well for most images but are not optimal
// - CodesPerBitsize tables define how many Huffman codes will have a certain bitsize (plus 1),
//   e.g. DcLuminanceCodesPerBitsize[2] = 5 because there are 5 Huffman codes being 2+1=3 bits long
// - Values tables are a list of values ordered by their Huffman code bitsize,
//   e.g. AcLuminanceValues => Huffman(0x01,0x02,0x03) will have 2 bits, Huffman(0x00) will have 3 bits, Huffman(0x04,0x11,0x05) will have 4 bits, ...

// Huffman definitions for first DC/AC tables (luminance / Y channel)
const uint8_t DcLuminanceCodesPerBitsize[16]   = { 0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0 };   // sum = 12
const uint8_t DcLuminanceValues         [12]   = { 0,1,2,3,4,5,6,7,8,9,10,11 };         // => 12 codes
const uint8_t AcLuminanceCodesPerBitsize[16]   = { 0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,125 }; // sum = 162
const uint8_t AcLuminanceValues        [162]   =                                        // => 162 codes
    { 0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08, // 16*10+2 because
      0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28, // upper 4 bits can be 0..F
      0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59, // while lower 4 bits can be 1..A
      0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89, // plus two special codes 0x00 and 0xF0
      0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6, // order of these symbols was determined empirically by JPEG committee
      0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,
      0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };
// Huffman definitions for second DC/AC tables (chrominance / Cb and Cr channels)
const uint8_t DcChrominanceCodesPerBitsize[16] = { 0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 };   // sum = 12
const uint8_t DcChrominanceValues         [12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };         // => 12 codes (identical to DcLuminanceValues)
const uint8_t AcChrominanceCodesPerBitsize[16] = { 0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,119 }; // sum = 162
const uint8_t AcChrominanceValues        [162] =                                        // => 162 codes
    { 0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91, // same number of symbol, just different order
      0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26, // (which is more efficient for AC coding)
      0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,
      0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,
      0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,
      0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,
      0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };

// ////////////////////////////////////////
// helper functions / templates

// same as std::min()
template <typename Number>
Number minimum(Number value, Number maximum)
{
  return value <= maximum ? value : maximum;
}

// restrict a value to the interval [minimum, maximum]
template <typename Number, typename Limit>
Number clamp(Number value, Limit minimum, Limit maximum)
{
  if (value <= minimum) return minimum; // never smaller than the minimum
  if (value >= maximum) return maximum; // never bigger  than the maximum
  return value;                         // value was inside interval, keep it
}

// convert from RGB to YCbCr, constants are similar to ITU-R, see https://en.wikipedia.org/wiki/YCbCr#JPEG_conversion
template <typename T>
float rgb2y (T r, T g, T b) { return +0.299f   * r +0.587f    * g +0.114f  * b; }
template <typename T>
float rgb2cb(T r, T g, T b) { return -0.16874f * r -0.33126f * g +0.5f     * b; } // ITU: -0.168736f * r -0.331264f * g +0.5f      * b
template <typename T>
float rgb2cr(T r, T g, T b) { return +0.5f     * r -0.41869f * g -0.08131f * b; } // ITU: +0.5f      * r -0.418688f * g -0.081312f * b

// start a new JFIF block
void writeMarker(void* caller, TooJpeg::WRITE_ONE_BYTE output, uint8_t id, uint16_t length)
{
  output(caller, 0xFF); output(caller, id);      // ID, always preceded by 0xFF
  output(caller, uint8_t(length >> 8));  // length of the following data (big-endian)
  output(caller, uint8_t(length & 0xFF));
}

// write bits stored in BitCode, keep excess bits in BitBuffer
void writeBits(void* caller, TooJpeg::WRITE_ONE_BYTE output, BitBuffer& buffer, BitCode data)
{
  // append the new bits to those bits leftover from previous call(s)
  buffer.numBits += data.numBits;
  buffer.bits   <<= data.numBits;
  buffer.bits    |= data.code;

  // write all "full" bytes
  while (buffer.numBits >= 8)
  {
    // extract highest 8 bits
    buffer.numBits -= 8;
    auto oneByte = (buffer.bits >> buffer.numBits) & 0xFF;
    output(caller, oneByte);

    if (oneByte == 0xFF) // 0xFF has a special meaning for JPEGs (it's a block marker)
      output(caller, 0);         // therefore pad a zero to indicate "nope, this one ain't a marker, it's just a coincidence"

    // note: I don't clear those written bits, therefore buffer.bits contains garbage in the high bits
    //       if you really want to "clean up" (e.g. for debugging purposes) then uncomment the following line
    //buffer.bits &= (1 << buffer.numBits) - 1;
  }
}

// convert to a JPEG codeword
BitCode convertCode(int16_t value)
{
  // positive value: code = value,     numBits = position of highest set bit
  // negative value: ignore sign, then numBits = position of highest set bit, and code = (2^numBits) - 1 + value
  auto absolute = value >= 0 ? +value : -value; // by the way: value is never zero
  auto mask = 0;       // will be 2^numBits - 1
  // find position of highest set bit, fast way for GCC: result.numBits = 32 - __builtin_clz(value);
  auto numBits = 0;
  while (absolute > mask)
  {
    numBits++;
    mask = 2*mask + 1; // append a set bit (numBits increased by one, so we need to update 2^numBits - 1)
  }

  // remember: mask = 2^numBits - 1, so if the original value was negative we can cheaply compute code = (2^numBits) - 1 + value = mask + value
  return BitCode(value >= 0 ? value : value + mask, numBits);
}

// forward DCT computation "in one dimension" (fast AAN algorithm: Arai, Agui and Nakajima: "A fast DCT-SQ scheme for images")
template <int stride> // stride must be 1 (=horizontal) or 8 (=vertical)
void DCT(float block[8*8])
{
  // modify in-place
  auto& block0 = block[0 * stride];
  auto& block1 = block[1 * stride];
  auto& block2 = block[2 * stride];
  auto& block3 = block[3 * stride];
  auto& block4 = block[4 * stride];
  auto& block5 = block[5 * stride];
  auto& block6 = block[6 * stride];
  auto& block7 = block[7 * stride];

  // based on https://dev.w3.org/Amaya/libjpeg/jfdctflt.c , the original variable names can be found in my comments
  auto add07 = block0 + block7; auto sub07 = block0 - block7; // tmp0, tmp7
  auto add16 = block1 + block6; auto sub16 = block1 - block6; // tmp1, tmp6
  auto add25 = block2 + block5; auto sub25 = block2 - block5; // tmp2, tmp5
  auto add34 = block3 + block4; auto sub34 = block3 - block4; // tmp3, tmp4

  auto add0347 = add07 + add34; auto sub07_34 = add07 - add34; // tmp10, tmp13 ("even part" / "phase 2")
  auto add1256 = add16 + add25; auto sub16_25 = add16 - add25; // tmp11, tmp12

  block0 = add0347 + add1256; block4 = add0347 - add1256; // "phase 3"

  auto z1 = (sub16_25 + sub07_34) * InvSqrt; // all temporary z-variables kept their original names
  block2 = sub07_34 + z1; block6 = sub07_34 - z1; // "phase 5"

  auto sub23_45 = sub25 + sub34; // tmp10 ("odd part" / "phase 2")
  auto sub12_56 = sub16 + sub25; // tmp11
  auto sub01_67 = sub16 + sub07; // tmp12

  auto z5 = (sub23_45 - sub01_67) * HalfSqrtSqrt;
  auto z2 = sub23_45 * InvSqrtSqrt  + z5;
  auto z3 = sub12_56 * InvSqrt;
  auto z4 = sub01_67 * SqrtHalfSqrt + z5;
  auto z6 = sub07 + z3; // z11 ("phase 5")
  auto z7 = sub07 - z3; // z13
  block1 = z6 + z4; block7 = z6 - z4; // "phase 6"
  block5 = z7 + z2; block3 = z7 - z2;
}

// run DCT, quantize and write Huffman bit codes
int16_t encodeBlock(void* caller, TooJpeg::WRITE_ONE_BYTE output, BitBuffer& buffer,
                    float block[8][8], const float scaled[8*8], int16_t lastDC,
                    const BitCode huffmanDC[256], const BitCode huffmanAC[256])
{
  // "linearize" the 8x8 block, treat it as a flat array of 64 floats
  auto block64 = (float*) block;

  // DCT: rows
  for (auto offset = 0; offset < 8; offset++)
    DCT<1>(block64 + offset*8);
  // DCT: columns
  for (auto offset = 0; offset < 8; offset++)
    DCT<8>(block64 + offset*1);

  // scale
  for (auto i = 0; i < 8*8; i++)
    block64[i] *= scaled[i];

  // encode DC (the first coefficient is the "average color" of the 8x8 block)
  // convert to an integer
  auto DC = int16_t(block64[0] + (block64[0] >= 0 ? +0.5f : -0.5f)); // C++11's nearbyint() achieves a similar effect
  // same "average color" as previous block ?
  if (DC == lastDC)
    writeBits(caller, output, buffer, huffmanDC[0x00]); // yes, write a special short symbol
  else
  {
    auto bits = convertCode(DC - lastDC);       // nope, encode the difference to previous block's average color
    writeBits(caller, output, buffer, huffmanDC[bits.numBits]);
    writeBits(caller, output, buffer, bits);
  }

  // quantize and zigzag the other 63 coefficients
  auto posNonZero = 0; // find last coefficient which is not zero (because trailing zeros are encoded very efficiently)
  int16_t quantized[8*8];
  for (auto i = 1; i < 8*8; i++) // start at 1 because block64[0]=DC was already processed
  {
    auto value = block64[ZigZagInv[i]];
    // round to nearest integer (actually, rounding is performed by casting from float to int16)
    quantized[i] = int16_t(value + (value >= 0 ? +0.5f : -0.5f)); // C++11's nearbyint() achieves a similar effect
    // remember offset of last non-zero coefficient
    if (quantized[i] != 0)
      posNonZero = i;
  }

  // encode ACs (Q[1..63])
  for (auto i = 1; i <= posNonZero; i++) // Q[0] was already written, start at Q[1] and skip all trailing zeros
  {
    // zeros are encoded in a special way
    auto offset = 0; // upper 4 bits count the number of consecutive zeros
    while (quantized[i] == 0) // found a few zeros, let's count them
    {
      i++;
      offset    +=  1 << 4; // add 1 to the upper 4 bits
      // split into blocks of at most 16 consecutive zeros
      if (offset > 15 << 4) // remember, the counter is in the upper 4 bits
      {
        offset = 0;
        writeBits(caller, output, buffer, huffmanAC[0xF0]); // 0xF0 is a special code for "16 zeros"
      }
    }

    // merge number of zeros with the number of bits of the next non-zero value
    auto bits = convertCode(quantized[i]);
    offset   += bits.numBits;
    writeBits(caller, output, buffer, huffmanAC[offset]);
    writeBits(caller, output, buffer, bits);
  }

  // send end-of-block code (0x00), only needed if there are trailing zeros
  if (posNonZero < 8*8 - 1) // = 63
    writeBits(caller, output, buffer, huffmanAC[0x00]);

  return DC;
}

// Jon's code includes the pre-generated Huffman codes
// I don't like these "magic constants" and compute them on my own :-)
void generateHuffmanTable(const uint8_t numCodes[16], const uint8_t* values, BitCode result[256])
{
  uint16_t huffmanCode = 0; // no JPEG Huffman code exceeds 16 bits
  // process all bitsizes 1 thru 16
  for (auto numBits = 1; numBits <= 16; numBits++)
  {
    // ... and each code of these bitsizes
    for (auto i = 0; i < numCodes[numBits - 1]; i++) // note: numCodes array starts at zero, but smallest bitsize is 1
    {
      auto current = *values++;
      result[current].code    = huffmanCode++;
      result[current].numBits = numBits;
    }
    // next Huffman code needs to be one bit wider
    huffmanCode <<= 1;
  }
}

} // end of anonymous namespace

// -------------------- the only externally visible function ... --------------------

namespace TooJpeg
{

// handle       - callback that stores a single byte (writes to disk, memory, ...)
// width,height - image size
// pixels       - stored in RGB format or grayscale, stored from upper-left to lower-right
// isRGB        - true if RGB format (3 bytes per pixel); false if grayscale (1 byte per pixel)
// quality      - between 1 (worst) and 100 (best)
// downsample   - if true then YCbCr 4:2:0 format is used (smaller size, minor quality loss) instead of 4:4:4, not relevant for grayscale
// comment      - optional JPEG comment (0/NULL if no comment)
bool writeJpeg(void* caller, TooJpeg::WRITE_ONE_BYTE output, const void* pixels_, unsigned short width, unsigned short height,
               bool isRGB, unsigned char quality_, bool downsample, const char* comment)
{
  // reject invalid pointers
  if (!output || !pixels_)
    return false;
  // check image format
  if (width == 0 || height == 0)
    return false;

  // quality level
  auto quality = clamp<uint16_t>(quality_, 1, 100);
  // formula taken from libjpeg
  quality = quality < 50 ? 5000 / quality : 200 - quality * 2;

  // number of components
  auto numComponents = isRGB ? 3 : 1;
  // note: if there is just one component (=grayscale), then only luminance needs to be stored in the file
  //       thus everything related to chrominance need not to be written to the JPEG
  //       I still compute a few things, like quantization tables to avoid a complete code mess

  // grayscale images can't be downsampled (because there are no Cb + Cr channels)
  if (!isRGB)
    downsample = false;

  // ////////////////////////////////////////
  // JFIF headers
  const uint8_t HeaderJfif[2+2+16] =
      { 0xFF,0xD8,         // SOI marker (start of image)
        0xFF,0xE0,         // JFIF APP0 tag
        0,16,              // length: 16 bytes (14 bytes payload + 2 bytes for this length field)
        'J','F','I','F',0, // JFIF identifier, zero-terminated
        1,1,               // JFIF version 1.1
        0,                 // no density units specified
        0,1,0,1,           // density: 1 pixel "per pixel" horizontally and vertically
        0,0 };             // no thumbnail (size 0 x 0)
  for (auto c : HeaderJfif)
    output(caller, c);

  // ////////////////////////////////////////
  // comment (optional)
  if (comment != 0)
  {
    // look for zero terminator
    auto length = 0; // = strlen(comment);
    while (comment[length] != 0)
      length++;

    // write COM marker
    writeMarker(caller, output, 0xFE, length + 2);     // length: number of bytes (without zero terminator) + 2 bytes for this length field
    // ... and write the comment itself
    for (auto i = 0; i < length; i++)
      output(caller, comment[i]);
  }

  // ////////////////////////////////////////
  // adjust quantization tables to desired quality
  uint8_t quantLuminance  [8*8];
  uint8_t quantChrominance[8*8];
  for (auto i = 0; i < 8*8; i++)
  {
    auto luminance   = (DefaultQuantLuminance  [ZigZagInv[i]] * quality + 50) / 100;
    auto chrominance = (DefaultQuantChrominance[ZigZagInv[i]] * quality + 50) / 100;

    // clamp to 1..255
    quantLuminance  [i] = uint8_t(clamp(luminance,   1, 255));
    quantChrominance[i] = uint8_t(clamp(chrominance, 1, 255));
  }

  // write quantization tables
  writeMarker(caller, output, 0xDB, 2 + (isRGB ? 2 : 1) * (1 + 8*8)); // length: 65 bytes per table + 2 bytes for this length field
                                                              // each table has 64 entries and is preceded by an ID byte

  output(caller, 0); // first  quantization table
  for (auto c : quantLuminance)
    output(caller, c);
  if (isRGB) // chrominance is only relevant for color images
  {
    output(caller, 1); // second quantization table
    for (auto c : quantChrominance)
      output(caller, c);
  }

  // ////////////////////////////////////////
  // write image infos (SOF0 - start of frame)
  writeMarker(caller, output, 0xC0, 2+6+3*numComponents); // length: 6 bytes general info + 3 per channel + 2 bytes for this length field

  // 8 bits per channel
  output(caller, 8);

  // image dimensions (big-endian)
  output(caller, uint8_t(height >> 8)); output(caller, uint8_t(height & 0xFF));
  output(caller, uint8_t(width  >> 8)); output(caller, uint8_t(width  & 0xFF));

  // sampling and quantization tables for each component
  output(caller, numComponents);           // 1 component (grayscale, Y only) or 3 components (Y,Cb,Cr)
  for (auto id = 1; id <= numComponents; id++)
  {
    output(caller, id);                    // component ID (Y=1, Cb=2, Cr=3)
    // bitmasks for sampling: highest 4 bits: horizontal, lowest 4 bits: vertical
    output(caller, id == 1 && downsample ? 0x22 : 0x11); // 0x11 is default YCbCr 4:4:4 and 0x22 stands for YCbCr 4:2:0
    output(caller, id == 1 ? 0 : 1);       // use quantization table 0 for Y, else table 1
  }

  // ////////////////////////////////////////
  // Huffman tables
  // DHT marker - define Huffman tables
  writeMarker(caller, output, 0xC4, isRGB ? (2+208+208) : (2+208));
                                  // 2 bytes for the length field, store chrominance only if needed
                                  //   1+16+12  for the DC luminance
                                  //   1+16+162 for the AC luminance   (208 = 1+16+12 + 1+16+162)
                                  //   1+16+12  for the DC chrominance
                                  //   1+16+162 for the AC chrominance (208 = 1+16+12 + 1+16+162, same as above)

  // store luminance's DC+AC Huffman table definitions
  output(caller, 0x00); // highest 4 bits: 0 => DC, lowest 4 bits: 0 => Y (baseline)
  for (auto c : DcLuminanceCodesPerBitsize)
    output(caller, c);
  for (auto c : DcLuminanceValues)
    output(caller, c);
  output(caller, 0x10); // highest 4 bits: 1 => AC, lowest 4 bits: 0 => Y (baseline)
  for (auto c : AcLuminanceCodesPerBitsize)
    output(caller, c);
  for (auto c : AcLuminanceValues)
    output(caller, c);

  // compute actual Huffman code tables (see Jon's code for precalculated tables)
  BitCode huffmanLuminanceDC[256];
  BitCode huffmanLuminanceAC[256];
  generateHuffmanTable(DcLuminanceCodesPerBitsize, DcLuminanceValues, huffmanLuminanceDC);
  generateHuffmanTable(AcLuminanceCodesPerBitsize, AcLuminanceValues, huffmanLuminanceAC);

  // chrominance is only relevant for color images
  BitCode huffmanChrominanceDC[256];
  BitCode huffmanChrominanceAC[256];
  if (isRGB)
  {
    // store luminance's DC+AC Huffman table definitions
    output(caller, 0x01); // highest 4 bits: 0 => DC, lowest 4 bits: 1 => Cr,Cb (baseline)
    for (auto c : DcChrominanceCodesPerBitsize)
      output(caller, c);
    for (auto c : DcChrominanceValues)
      output(caller, c);
    output(caller, 0x11); // highest 4 bits: 1 => AC, lowest 4 bits: 1 => Cr,Cb (baseline)
    for (auto c : AcChrominanceCodesPerBitsize)
      output(caller, c);
    for (auto c : AcChrominanceValues)
      output(caller, c);

    // compute actual Huffman code tables (see Jon's code for precalculated tables)
    generateHuffmanTable(DcChrominanceCodesPerBitsize, DcChrominanceValues, huffmanChrominanceDC);
    generateHuffmanTable(AcChrominanceCodesPerBitsize, AcChrominanceValues, huffmanChrominanceAC);
  }

  // ////////////////////////////////////////
  // start of scan (there is only a single scan for baseline JPEGs)
  writeMarker(caller, output, 0xDA, 2+1+2*numComponents+3);

  // assign Huffman tables to each component
  output(caller, numComponents);
  for (auto id = 1; id <= numComponents; id++)
  {
    // component ID (Y=1, Cb=2, Cr=3)
    output(caller, id);
    // highest 4 bits: DC Huffman table, lowest 4 bits: AC Huffman table
    output(caller, id == 1 ? 0x00 : 0x11); // Y: tables 0 for DC and AC; Cb + Cr: tables 1 for DC and AC
  }

  // constant values for our baseline JPEGs with a single sequential scan
  output(caller,  0); // spectral selection: must start at  0
  output(caller, 63); // spectral selection: must stop  at 63
  output(caller,  0); // successive approximation: must be  0

  // adjust quantization tables with AAN scaling factors to simplify DCT
  float scaledLuminance  [8*8];
  float scaledChrominance[8*8];
  for (auto i = 0; i < 8*8; i++)
  {
    auto row    = ZigZagInv[i] / 8; // same as i >> 3
    auto column = ZigZagInv[i] % 8; // same as i &  7
    auto factor = 1 / (AanScaleFactors[row] * AanScaleFactors[column] * 8);
    scaledLuminance  [ZigZagInv[i]] = factor / quantLuminance  [i];
    scaledChrominance[ZigZagInv[i]] = factor / quantChrominance[i];
    // if you really want JPEGs that are bitwise identical to Jon's code then you need slightly different formulas (note: sqrt(8) = 2.828427125f)
    //const float aasf[] = { 1.0f * 2.828427125f, 1.387039845f * 2.828427125f, 1.306562965f * 2.828427125f, 1.175875602f * 2.828427125f, 1.0f * 2.828427125f, 0.785694958f * 2.828427125f, 0.541196100f * 2.828427125f, 0.275899379f * 2.828427125f }; // line 240 of jo_jpeg.cpp
    //scaledLuminance  [ZigZagInv[i]] = 1 / (quantLuminance  [i] * aasf[row] * aasf[column]); // lines 266-267 of jo_jpeg.cpp
    //scaledChrominance[ZigZagInv[i]] = 1 / (quantChrominance[i] * aasf[row] * aasf[column]);
  }

  // all encoded bits pass through this buffer, it writes to output whenever a byte is completed
  BitBuffer buffer;

  // just convert image data from void*
  auto pixels = (const uint8_t*)pixels_;

  // break down the image into 8x8 blocks and convert from RGB or grayscale to YCbCr color space
  float Y[8][8], Cb[8][8], Cr[8][8];
  // average color of the previous 8x8 block
  int16_t lastYDC = 0, lastCbDC = 0, lastCrDC = 0;

  // downsampling of Cb and Cr channels, if sampling = 2 then 2x2 samples are used
  auto sampling   = downsample ? 2 : 1;
  auto numSamples = sampling * sampling; // 1 (grayscale, YCbCr444) or 4 (YCbCr420)
  // only for downsampled: sum of four pixels' red, green, blue components
  float red[8][8] = { { 0 } }, green[8][8] = { { 0 } }, blue[8][8] = { { 0 } }; // uint16_t works, too, but float is faster

  // process MCUs (minimum codes units)
  for (auto mcuY = 0; mcuY < height; mcuY += 8 * sampling)
    for (auto mcuX = 0; mcuX < width; mcuX += 8 * sampling)
    {
      // YCbCr 4:4:4 format: each MCU is a 8x8 block - the same applies to grayscale images, too
      // YCbCr 4:2:0 format: each MCU represents a 16x16 block, stored as 4x 8x8 Y-blocks plus 1x 8x8 Cb and 1x 8x8 Cr blocks)
      for (auto block = 0; block < numSamples; block++) // this loop is iterated once (grayscale, 4:4:4) or four times (4:2:0)
      {
        // subdivide into 8x8 blocks where blockX and blockY indicate the minimum x and y of the current block
        auto blockX = 8 * (block & 1); // same as 8 * (block % 2) => { 0, 8, 0, 8 }
        auto blockY = 4 * (block & 2); // same as 8 * (block / 2) => { 0, 0, 8, 8 }

        // now we finally have an 8x8 block ...
        for (auto deltaY = 0; deltaY < 8; deltaY++)
          for (auto deltaX = 0; deltaX < 8; deltaX++)
          {
            // find actual pixel position within the current image
            auto column   = minimum(mcuX + blockX + deltaX, width  - 1); // must not exceed image borders, replicate last row/column if needed
            auto row      = minimum(mcuY + blockY + deltaY, height - 1);
            auto pixelPos = row * (int32_t)width + column; // the cast ensures that we don't run into multiplication overflows

            // grayscale images have solely a Y channel which can be easily derived from the input pixel by shifting it by 128
            if (!isRGB)
            {
              Y[deltaY][deltaX] = pixels[pixelPos] - 128.f;
              continue;
            }

            // RGB: 3 bytes per pixel (whereas grayscale images have only 1 byte per pixel)
            pixelPos *= numComponents;
            float r = pixels[pixelPos    ];
            float g = pixels[pixelPos + 1];
            float b = pixels[pixelPos + 2];

            // compute Y channel
            Y[deltaY][deltaX] = rgb2y(r, g, b) - 128; // again, the JPEG standard requires Y to be shifted by 128

            if (downsample)
            {
              // defer Cb/Cr computation if YCbCr420 mode: we must average 2x2 pixels, so let's "shrink" a 16x16 area to 8x8
              auto x = (blockX + deltaX) / 2;
              auto y = (blockY + deltaY) / 2;

              // add red, green, blue (note: each number should be weighted by 1/4, this is done later)
              red  [y][x] += r;
              green[y][x] += g;
              blue [y][x] += b;
            }
            else
            {
              // YCbCr444: computation of Cb and Cr is similar to Y channel but without the need to subtract 128
              Cb[deltaY][deltaX] = rgb2cb(r, g, b);
              Cr[deltaY][deltaX] = rgb2cr(r, g, b);
            }
          }

        // encode Y channel
        lastYDC = encodeBlock(caller, output, buffer, Y, scaledLuminance, lastYDC, huffmanLuminanceDC, huffmanLuminanceAC);
      }

      if (!isRGB)
        continue;

      // ////////////////////////////////////////
      // YCbCr420 / downsampled: convert summed RGB values to Cb and Cr
      if (downsample)
        for (auto y = 0; y < 8; y++)
          for (auto x = 0; x < 8; x++)
          {
            // each number in the arrays "red", "green" and "blue" consists of the summed values of four pixels
            // so I still have to divide them by 4 to get their average value
            auto r = red  [y][x] / 4.f; // numSamples = 4
            auto g = green[y][x] / 4.f;
            auto b = blue [y][x] / 4.f;

            Cb[y][x] = rgb2cb(r, g, b);
            Cr[y][x] = rgb2cr(r, g, b);

            // reset arrays for next block
            red[y][x] = green[y][x] = blue[y][x] = 0;
          }

      // encode Cb + Cr channels
      lastCbDC = encodeBlock(caller, output, buffer, Cb, scaledChrominance, lastCbDC, huffmanChrominanceDC, huffmanChrominanceAC);
      lastCrDC = encodeBlock(caller, output, buffer, Cr, scaledChrominance, lastCrDC, huffmanChrominanceDC, huffmanChrominanceAC);
    }

  // fill remaining bits with 1s
  writeBits(caller, output, buffer, { 0x7F, 7 }); // seven set bits: 0x7F = binary 0111 1111

  // ///////////////////////////
  // EOI marker
  output(caller, 0xFF); output(caller, 0xD9); // no length, therefore I can't use writeMarker()
  return true;
} // writeJpeg()

} // namespace TooJpeg
