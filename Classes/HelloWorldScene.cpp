/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2021 Bytedance Inc.

 https://axmolengine.github.io/

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
#include "EffectSprite/LightEffect.h"
#include "EffectSprite/EffectSprite.h"

USING_NS_AX;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in front of filenames in "
        "HelloWorldScene.cpp\n");
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
    
    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                           AX_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    _screenH = visibleSize.height;
    _screenW = visibleSize.width;

    _lightPos = Vec3(_screenW-700, _screenH-300, 200);

    _light = LightEffect::create();
    _light->setLightPos(_lightPos);
    _light->setLightCutoffRadius(1000);
    _light->setBrightness(2.0);
    _light->setLightColor(ax::Color3B(255,160, 60));
    _light->setAmbientLightColor(ax::Color3B(96,96,80));
    _light->retain();
    
    initBackground();
    
    auto spritecache = SpriteFrameCache::getInstance();
    spritecache->addSpriteFramesWithFile("spritesheet.plist");

    Vector<SpriteFrame*> animFrames;
    char str[100];
    for(int i = 1; i <= 8; i++)
    {
        snprintf(str, sizeof(str), "character/%02d.png", i);
        animFrames.pushBack(spritecache->getSpriteFrameByName(str));
    }

    auto sprite = EffectSprite::createWithSpriteFrame(animFrames.front());

    Animation *animation = Animation::createWithSpriteFrames(animFrames, 1.0f/8);
    sprite->runAction(RepeatForever::create(Animate::create(animation)));
    sprite->setPosition(_screenW / 2.0, _screenH / 2.0 - 75.0);
    sprite->setEffect(_light, "spritesheet_n.png");
    addChild(sprite);

    _lightSprite = Sprite::create("lightbulb.png");
    _lightSprite->setPosition(_lightPos.x, _lightPos.y);
    addChild(_lightSprite);

    auto listerner = EventListenerTouchAllAtOnce::create();
    listerner->onTouchesMoved = AX_CALLBACK_2(HelloWorld::handleTouches, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listerner, this);
    
    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true; 
}

void HelloWorld::update(float delta)
{
    switch (_gameState)
    {
    case ExampleGameState::init:
        _gameState = ExampleGameState::update;
        break;

    case ExampleGameState::update:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;

    case ExampleGameState::pause:
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()
        break;

    case ExampleGameState::menu1:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateMenu1();
        break;

    case ExampleGameState::menu2:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // UpdateMenu2();
        break;

    case ExampleGameState::end:
        /////////////////////////////
        // Add your codes below...like....
        // 
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}


void HelloWorld::handleTouches(const std::vector<Touch *> &touches, Event *)
{
    for (auto &touch: touches)
    {
        Point pos = touch->getLocation();
        _lightSprite->setPosition(pos);
        _lightPos.set(pos.x, pos.y, _lightPos.z);
        _light->setLightPos(_lightPos);
    }
}


void HelloWorld::initBackground()
{
    addBackgroundTile("res/background_01.png", 0, 100);
    addBackgroundTile("res/background_01.png", 1920, 100);
    addBackgroundTile("res/foreground_01.png", 0, 200, "res/foreground_01_n.png");
    addBackgroundTile("res/foreground_02.png", 1920, 200, "res/foreground_02_n.png");
}


EffectSprite *HelloWorld::addBackgroundTile(const std::string &spriteFile,
                                            float offsetX,
                                            float speed,
                                            const std::string &normalsFile)
{
    auto background = EffectSprite::create(spriteFile);
    if (!normalsFile.empty())
    {
        background->setEffect(_light, normalsFile);
    }
    else
    {
        background->setColor(_light->getAmbientLightColor());
    }

    float offsetY = (_screenH - background->getContentSize().height) / 2.0f;

    background->setAnchorPoint(Vec2(0,0));
    addChild(background);
    
    auto a1 = MoveTo::create(0, Vec2(offsetX, offsetY));
    auto a2 = MoveTo::create((_screenW + offsetX) / speed, Vec2(-_screenW, offsetY));
    auto a3 = MoveTo::create(0, Vec2(_screenW, offsetY));
    auto a4 = MoveTo::create((_screenW - offsetX) / speed, Vec2(offsetX, offsetY));
    background->runAction(RepeatForever::create(Sequence::create(a1, a2, a3, a4, nullptr)));
    
    return background;
}
