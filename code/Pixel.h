//
//  Pixel.h
//  ProjetMOPSI
//
//  Created by Emile Mathieu on 23/01/15.
//
//

#ifndef __ProjetMOPSI__Pixel__
#define __ProjetMOPSI__Pixel__

#include <stdio.h>

class Pixel {
private:
    int level;
    int index;
public:
    Pixel(int newLevel, int newIndex):level(newLevel),index(newIndex){}
    int getIndex(){return index;}
    int getLevel(){return level;}
};

class comparePixel{
public:
    bool operator()(Pixel& p1, Pixel& p2){
        return p1.getIndex() >= p2.getIndex();
    }
};




#endif /* defined(__ProjetMOPSI__Pixel__) */
