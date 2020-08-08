#ifndef FONT5X7EXT_H
#define FONT5X7EXT_H

#define MAXROWS 7
#define MAXCOLS 6

const short letterMatrix[][42] =
{
    //
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },
    //!
    {0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0
     },//"
    {0, 0, 0, 0, 0, 0,
     0, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },//#
    {0, 0, 1, 0, 1, 0,
     0, 1, 1, 1, 1, 1,
     0, 0, 1, 0, 1, 0,
     0, 0, 1, 0, 1, 0,
     0, 1, 1, 1, 1, 1,
     0, 0, 1, 0, 1, 0,
     0, 0, 0, 0, 0, 0
     },//$
    {0, 0, 0, 1, 0, 0,
     0, 0, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 1, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 0, 0, 1, 0, 0
    },//%
    {0, 1, 1, 0, 0, 1,
     0, 1, 1, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     1, 0, 0, 0, 1, 1,
     0, 0, 0, 0, 1, 1
     },//&
    {0, 0, 1, 1, 0, 0,
     0, 1, 0, 0, 1, 0,
     0, 1, 0, 0, 1, 0,
     0, 0, 0, 0, 0, 1,
     0, 1, 0, 0, 1, 0,
     0, 1, 0, 0, 1, 0,
     0, 0, 1, 1, 0, 1
     },//'
    {0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },//(
    {0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 0, 0
     },//}
    {0, 1, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0
     },// *
    {0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 1, 0,
     0, 1, 0, 1, 0, 0,
     1, 0, 1, 0, 1, 0,
     0, 1, 0, 1, 0, 0,
     1, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 0, 0
     },// +
    {0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     1, 1, 1, 1, 1, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },// ,
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0
     },// -
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },//.
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0
     },// /
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },//0
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 1, 1,
     0, 1, 0, 1, 0, 1,
     0, 1, 1, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//1
    {0, 0, 0, 1, 0, 0,
     0, 0, 1, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 1, 1, 0
     },//2
    {0, 1, 1, 1, 1, 0,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1
     },//3
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 1, 1, 0,
     0, 0, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//4
    {0, 0, 0, 0, 1, 1,
     0, 0, 0, 1, 0, 1,
     0, 0, 1, 0, 0, 1,
     0, 1, 1, 1, 1, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1
     },//5
    {0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0
     },//6
    {0, 0, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//7
    {0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0
     },//8
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//9
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 1,
     0, 0, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//:
    {0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },//;
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     1, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0
     },//<
    {0, 0, 0, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 1, 0
     },//=
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 0,
     0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },//>
    {1, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0
     },//?
    {0, 1, 1, 1, 0, 0,
     1, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0
     },//@
    {0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 1, 1,
     0, 1, 0, 1, 0, 1,
     0, 1, 0, 0, 1, 1,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1
     },//A
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },//B
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0
     },//C
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//D
    {0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0
     },//E
    {0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1
     },//F
    {0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0
     },//G
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//H
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1,0 , 0, 0, 1
     },//I
    {0, 1, 1, 1, 1, 1,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 1, 1, 1, 1, 1
     },//J
    {0, 0, 0, 1, 1, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//K
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 1, 0,
     0, 1, 0, 1, 0, 0,
     0, 1, 1, 0, 0, 0,
     0, 1, 0, 1, 0, 0,
     0, 1, 0, 0, 1, 0,
     0, 1, 0, 0, 0, 1
     },//L
    {0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1
     },//M
    {0, 1, 0, 0, 0, 1,
     0, 1, 1, 0, 1, 1,
     0, 1, 0, 1, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },//N
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 0, 0, 1,
     0, 1, 0, 1, 0, 1,
     0, 1, 0, 0, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },//O
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//P
    {0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0
     },//Q
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 1, 0, 1,
     0, 1, 0, 0, 1, 0,
     0, 0, 1, 1, 0, 1
     },//R
    {0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },//S
    {0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 0, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//T
    {0, 1, 1, 1, 1, 1,
     0, 1, 0, 1, 0, 1,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0
     },//U
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },//V
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 0, 1, 0,
     0, 0, 0, 1, 0, 0
     },//W
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 1, 0, 1,
     0, 1, 0, 1, 0, 1,
     0, 0, 1, 0, 1, 0
     },//X
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },//Y
    {0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0
     },//Z
    {0, 1, 1, 1, 1, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0
     },//[
    {1, 1, 1, 1, 0, 0,
     1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 0, 0
     },// "\"
    {0, 0, 0, 0, 0, 0,
     1, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 0, 0
     },// ]
    {0, 1, 1, 1, 1, 0,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 1, 0,
     0, 1, 1, 1, 1, 0
     },// ^
    {0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 1, 0, 1, 0, 0,
     1, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },// _
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 0, 0
     },// `
    {0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0
     },// a
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 1, 0,
     0, 0, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 1
     },// b
    {0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },// c
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },// d
    {0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 1
     },// e
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1
     },// f
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 1, 0,0,  1, 0,
     0, 1, 1, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0
     },// g
    {0, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
     },// h
    {0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },// i
    {0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0
     },// j
    {0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 1, 0, 1, 0, 0,
     0, 0, 1, 1, 0, 0
     },// k
    {0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 1, 0,
     0, 1, 1, 1, 0, 0,
     0, 1, 0, 0, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },// l
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 0
     },// m
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 1, 0,
     0, 1, 0, 1, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
     },// n
     {0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0,
      0, 1, 1, 1, 1, 0,
      0, 1, 0, 0, 0, 1,
      0, 1, 0, 0, 0, 1,
      0, 1, 0, 0, 0, 1,
      0, 1, 0, 0, 0, 1
    },// o
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
    },// p
    {0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0
    },// q
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 0, 0, 0, 1
    },// r
    {0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0
    },// s
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 1,
     0, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 1, 0,
     0, 0, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 0
    },// t
    {0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 1, 1, 1,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 0, 1, 0, 1,
     0, 0, 0, 1, 1, 0
    },// u
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 1
    },// v
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 0, 1, 0,
     0, 0, 0, 1, 0, 0
    },// w
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 1, 0, 1,
     0, 1, 0, 1, 0, 1,
     0, 0, 1, 0, 1, 0
    },// x
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1
    },// y
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 1,
     0, 1, 0, 0, 0, 1,
     0, 1, 1, 1, 1, 1,
     0, 0, 0, 0, 0, 1,
     0, 0, 1, 1, 1, 0
    },// z
    {0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0,
     0, 1, 1, 1, 1, 1,
     0, 1, 0, 0, 1, 0,
     0, 0, 0, 1, 0, 0,
     0, 0, 1, 0, 0, 1,
     0, 1, 1, 1, 1, 1
    }




};

#endif
