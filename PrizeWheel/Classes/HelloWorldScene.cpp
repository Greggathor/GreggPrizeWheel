/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

USING_NS_CC;


Sprite* prizeSprites[8];
Label* prizeLabels[8];

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto mynode = MyDrawNode::create(); // Create an instance of MyDrawNode
    //this->addChild(mynode);             // Add the instance to the parent

    std::string resourcePath = "C:/Users/Gregg/cocos2d-x-4.0/my_games/GreggPrizeWheel/PrizeWheel/Resources/";

    auto wheelBorder = Sprite::create(resourcePath + "WheelAssets/wheel_border.png");
    this->addChild(wheelBorder);

    auto wheelArrow = Sprite::create(resourcePath + "WheelAssets/wheel_arrow.png");
    wheelBorder->addChild(wheelArrow, 5);
    wheelArrow->setAnchorPoint(Vec2(0.5f, 0.8f));
    wheelArrow->setPosition(wheelBorder->getBoundingBox().size.width / 2.0f, wheelBorder->getBoundingBox().size.height);

    wheelSections = Sprite::create(resourcePath + "WheelAssets/wheel_sections_8.png");
    wheelBorder->addChild(wheelSections, -1);
    wheelSections->setPosition(wheelBorder->getBoundingBox().size.width / 2.0f, wheelBorder->getBoundingBox().size.height / 2.0f);
    
    //Create prize sprites on wheel
    prizeSprites[0] = Sprite::create(resourcePath + "WheelAssets/heart.png");
    prizeLabels[0] = Label::createWithSystemFont("30 min", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    prizeSprites[1] = Sprite::create(resourcePath + "WheelAssets/brush.png");
    prizeLabels[1] = Label::createWithSystemFont("x3", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    prizeSprites[2] = Sprite::create(resourcePath + "WheelAssets/gem.png");
    prizeLabels[2] = Label::createWithSystemFont("x35", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    prizeSprites[3] = Sprite::create(resourcePath + "WheelAssets/hammer.png");
    prizeLabels[3] = Label::createWithSystemFont("x3", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    prizeSprites[4] = Sprite::create(resourcePath + "WheelAssets/coin.png");
    prizeLabels[4] = Label::createWithSystemFont("x750", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    prizeSprites[5] = Sprite::create(resourcePath + "WheelAssets/brush.png");
    prizeLabels[5] = Label::createWithSystemFont("x1", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    prizeSprites[6] = Sprite::create(resourcePath + "WheelAssets/gem.png");
    prizeLabels[6] = Label::createWithSystemFont("x75", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    prizeSprites[7] = Sprite::create(resourcePath + "WheelAssets/hammer.png");
    prizeLabels[7] = Label::createWithSystemFont("x1", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);

    for (int i = 0; i < 8; i++)
    {
        float iRotation = (360.0 / 16.0) + (360.0 / 8.0) * i;
        wheelSections->setRotation(iRotation);
        wheelSections->addChild(prizeSprites[i], 1);
        prizeSprites[i]->setPosition(wheelSections->convertToNodeSpace(Vec2(0.0f, wheelSections->getBoundingBox().size.height * 0.25f)));
        prizeSprites[i]->setRotation(-iRotation);

        prizeSprites[i]->addChild(prizeLabels[i], 2);
        prizeLabels[i]->setPosition(prizeSprites[i]->getBoundingBox().size.width / 2.0f, 0.0f);
        prizeLabels[i]->setTextColor(Color4B::BLUE);
        //prizeLabels[i]->enableOutline(Color4B::BLACK, 30);
    }
    /*
    Vec2 pOne = prizeSprites[0]->getPosition();
    Vec2 pOneW = wheelSections->convertToWorldSpace(prizeSprites[0]->getPosition());
    Vec2 wSecW = wheelBorder->convertToWorldSpace(wheelSections->getPosition());
    //*/

    //set position and scale of entire wheel
    wheelBorder->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100);
    wheelBorder->setScale(0.4f);

    //Creating button that spins wheel
    auto spinButton = MenuItemImage::create(
        resourcePath + "WheelAssets/spin_button.png",
        resourcePath + "WheelAssets/spin_button.png",
        CC_CALLBACK_1(HelloWorld::SpinWheel, this));
    spinButton->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8);
    spinButton->setScale(0.4f);

    auto menu = Menu::create(spinButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    auto buttonLabel = Label::createWithSystemFont("Spin", "Ariel", 36, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    buttonLabel->setPosition(spinButton->getPosition());
    this->addChild(buttonLabel, 2);

    return true;
}

void HelloWorld::SpinWheel(Ref* pSender)
{
    //spin testing
    auto fastSpin = RotateBy::create(3.0f, 720.0f);
    auto n_fastSpin = RotateBy::create(3.0f, -720.0f);
    auto slowSpin = RotateBy::create(1.0f, 90.0f);
    auto startSpin = EaseBackIn::create(slowSpin->clone());
    //auto slowdownSpin = EaseOut::create(fastSpin->clone(), 0.5f);
    auto spinSequence = Sequence::create(startSpin, fastSpin, nullptr);

    wheelSections->runAction(spinSequence);

}