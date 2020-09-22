#pragma once
#ifndef PrizeList_hpp
#define PrizeList_hpp

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <fstream>
//#include <iomanip>
#include "cocos2d.h"

using namespace cocos2d;

class PrizeList
{
public:
    std::vector<Sprite*> sprites;       //contains all the sprites for prizes
    std::vector<Label*> labels;         //contains all descriptions for prizes
    std::vector<int> probabilities;   //chance of landing on object
    std::vector<float> rotations;       //degree position of prize on wheel

    //PrizeList();
    void add(Sprite* sprite, Label* label, const int& probability);
    int winPrize();
    void testWinRate(int attempts);

};

#endif