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

PrizeList prizeList;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
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

    srand(time(0)); //reset seed for reward rng
    timeLeft = 0.0f;
    spriteScale = 0.4f;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //add file path to Resources folder. Not working for me by default
    std::string resourcePath = FileUtils::getInstance()->FileUtils::getDefaultResourceRootPath();
    
    while(true)
    {
        std::size_t found = resourcePath.rfind("/PrizeWheel");
        if (found == std::string::npos)
            break;
            
        resourcePath.erase(found);
    }
    resourcePath += "/PrizeWheel/Resources/";
    auto searchPaths = FileUtils::getInstance()->FileUtils::getSearchPaths();
    searchPaths.push_back(resourcePath);
    FileUtils::getInstance()->setSearchPaths(searchPaths);


    //adding sprites for wheel
    auto wheelBorder = Sprite::create("WheelAssets/wheel_border.png");
    this->addChild(wheelBorder);

    auto wheelArrow = Sprite::create("WheelAssets/wheel_arrow.png");
    wheelBorder->addChild(wheelArrow, 5);
    wheelArrow->setAnchorPoint(Vec2(0.5f, 0.8f));
    wheelArrow->setPosition(wheelBorder->getBoundingBox().size.width / 2.0f, wheelBorder->getBoundingBox().size.height);

    wheelSections = Sprite::create("WheelAssets/wheel_sections_8.png");
    wheelBorder->addChild(wheelSections, -1);
    wheelSections->setPosition(wheelBorder->getBoundingBox().size.width / 2.0f, wheelBorder->getBoundingBox().size.height / 2.0f);
    
    //Create prize sprites on wheel
    prizeList.add(Sprite::create("WheelAssets/heart.png"),
        Label::createWithSystemFont("30 min", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        20);

    prizeList.add(Sprite::create("WheelAssets/brush.png"),
        Label::createWithSystemFont("x3", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        10);

    prizeList.add(Sprite::create("WheelAssets/gem.png"),
        Label::createWithSystemFont("x35", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        10);

    prizeList.add(Sprite::create("WheelAssets/hammer.png"),
        Label::createWithSystemFont("x3", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        10);

    prizeList.add(Sprite::create("WheelAssets/coin.png"),
        Label::createWithSystemFont("x750", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        5);

    prizeList.add(Sprite::create("WheelAssets/brush.png"),
        Label::createWithSystemFont("x1", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        20);

    prizeList.add(Sprite::create("WheelAssets/gem.png"),
        Label::createWithSystemFont("x75", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        5);

    prizeList.add(Sprite::create("WheelAssets/hammer.png"),
        Label::createWithSystemFont("x1", "Ariel", 42, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER),
        20);

         //correctly orient prize sprites on wheel
    for (int i = 0; i < 8; i++)
    {
        prizeList.rotations.push_back((360.0 / 16.0) + (360.0 / 8.0) * i);
        wheelSections->setRotation(prizeList.rotations[i]);
        wheelSections->addChild(prizeList.sprites[i], 4, i);
        prizeList.sprites[i]->setPosition(wheelSections->convertToNodeSpace(Vec2(0.0f, wheelSections->getBoundingBox().size.height * 0.25f)));
        prizeList.sprites[i]->setRotation(-prizeList.rotations[i]);

        prizeList.sprites[i]->addChild(prizeList.labels[i], 5);
        prizeList.labels[i]->setPosition(prizeList.sprites[i]->getBoundingBox().size.width / 2.0f, 0.0f);
        prizeList.labels[i]->setTextColor(Color4B::BLUE);
        //prizeLabels[i]->enableOutline(Color4B::BLACK, 30);
    }

    //set position and scale of entire wheel
    wheelBorder->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100);
    wheelBorder->setScale(spriteScale);


    //Creating button that spins wheel
    spinButton = MenuItemImage::create(
        "WheelAssets/spin_button.png",
        "WheelAssets/spin_button.png",
        CC_CALLBACK_1(HelloWorld::SpinWheel, this));
    spinButton->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8);

    auto spinLabel = Label::createWithSystemFont("Spin", "Ariel", 64, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    spinButton->addChild(spinLabel, 2);
    spinLabel->setPosition(spinButton->getBoundingBox().size.width / 2.0, spinButton->getBoundingBox().size.height / 2.0);

    spinButton->setScale(spriteScale);

    //Creating button to claim prize after spin
    claimButton = MenuItemImage::create(
        "WheelAssets/spin_button.png",
        "WheelAssets/spin_button.png",
        CC_CALLBACK_1(HelloWorld::ClaimPrize, this));
    claimButton->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 8);

    auto claimLabel = Label::createWithSystemFont("Claim", "Ariel", 64, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    claimButton->addChild(claimLabel, 2);
    claimLabel->setPosition(claimButton->getBoundingBox().size.width / 2.0, claimButton->getBoundingBox().size.height / 2.0);

    claimButton->setScale(spriteScale);
    auto hide = Hide::create();
    claimButton->runAction(hide);

    auto menu = Menu::create(spinButton, claimButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    scheduleUpdate();

    return true;
}

void HelloWorld::update(float delta)
{
    //Sets up scene once wheel is done spinning
    if (timeLeft > 0.0f)
    {
        timeLeft -= delta;

        if (timeLeft <= 0.0f)
        {
            auto show = Show::create();
            claimButton->runAction(show);

            iPrizePos = prizeList.sprites[prizeTag]->getPosition();
            iPrizeRot = prizeList.sprites[prizeTag]->getRotation();

            auto visibleSize = Director::getInstance()->getVisibleSize(); // Screen's visible size
            Vec2 origin = Director::getInstance()->getVisibleOrigin();    // Screen's visible origin

            // center of the screen
            float cx = origin.x + visibleSize.width * 0.5;  // X for the center of the screen
            float cy = origin.y + visibleSize.height * 0.5; // Y for the center of the screen

            float animTime = 1.0f; //length of time for the animation
            //auto moveTo = MoveTo::create(animTime, Vec2(cx, cy));
            //auto moveTo = MoveTo::create(2.0f, wheelSections->convertToNodeSpace(Vec2(cx, cy)));
            //auto moveTo = MoveTo::create(2.0f, Vec2(wheelSections->getBoundingBox().size.width / 2.0, wheelSections->getBoundingBox().size.height / 2.0));
            auto scaleUp = ScaleBy::create(animTime, 1.3f);
            auto scaleDown = scaleUp->reverse();
            auto scaleSequence = Sequence::create(scaleUp, scaleDown, nullptr);
            auto scaleRepeat = RepeatForever::create(scaleSequence);
            //auto fadeOut = FadeOut::create(animTime);
            //prizeList.sprites[prizeTag]->runAction(moveTo);
            //prizeList.sprites[prizeTag]->runAction(scaleUp);
            prizeList.sprites[prizeTag]->runAction(scaleRepeat);
            //wheelSections->runAction(fadeOut);
        }
    }
}

//Called when button to spin wheel is pressed
void HelloWorld::SpinWheel(Ref* pSender)
{
    //spin testing
    //prizeList.testWinRate(1000);

    float spinSpeed = 240.0f; // degrees/sec

    auto startSpin = RotateBy::create(1.0f, 90.0f);
    auto ebiStartSpin = EaseBackIn::create(startSpin->clone());
    auto normalSpin = RotateBy::create(3.0f, 3.0f * spinSpeed);

    //Determine prize, then find out how long and how far wheel needs to spin to land on that prize
    prizeTag = prizeList.winPrize();
    float endRotation = prizeList.rotations[prizeTag];
    float degreesTilEnd = endRotation - (wheelSections->getRotation() + 90.0f + (3.0f * spinSpeed));
     //if negative degrees, convert to positive
    while (degreesTilEnd <= 0.0f)
    {
        degreesTilEnd += 360.0f;
    }
    float timeTilEnd = degreesTilEnd / spinSpeed;
    auto endSpin = RotateBy::create(timeTilEnd, degreesTilEnd);

    auto spinSequence = Sequence::create(ebiStartSpin, normalSpin, endSpin, nullptr);

    wheelSections->runAction(spinSequence);

    auto hide = Hide::create();
    spinButton->runAction(hide);

    //set timer for how long wheel is going to spin
    timeLeft = 1.0 + 3.0 + timeTilEnd;
}

//Called when button to claim prize is pressed
void HelloWorld::ClaimPrize(Ref* pSender)
{
    auto hide = Hide::create();
    auto show = Show::create();
    claimButton->runAction(hide);
    spinButton->runAction(show);

    prizeList.sprites[prizeTag]->setScale(1.0f);
    prizeList.sprites[prizeTag]->stopAllActions();
}