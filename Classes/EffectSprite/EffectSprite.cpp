/****************************************************************************
 Copyright (c) 2014-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

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

/*
 * based on axmol/tests/cpp-tests/Classes/ShaderTest/ShaderTest2.cpp
 * extended by CodeAndWeb GmbH https://www.codeandweb.com
 */

#include "EffectSprite.h"
#include "LightEffect.h"

USING_NS_AX;


EffectSprite *EffectSprite::create(const std::string_view &filename)
{
    auto ret = new EffectSprite;
    if (ret->initWithFile(filename))
    {
        ret->autorelease();
        return ret;
    }
    AX_SAFE_RELEASE(ret);
    return nullptr;
}


EffectSprite *EffectSprite::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
    auto ret = new EffectSprite;
    if (ret->initWithSpriteFrame(spriteFrame))
    {
        ret->autorelease();
        return ret;
    }
    AX_SAFE_RELEASE(ret);
    return nullptr;
}


void EffectSprite::setEffect(LightEffect *effect, const std::string &normalMapFilename)
{
    _normalmap = Director::getInstance()->getTextureCache()->addImage(normalMapFilename);
    AX_ASSERT(_normalmap);
    AX_SAFE_RETAIN(_normalmap);
    
    if (_effect != effect)
    {
        AX_SAFE_RELEASE(_effect);
        _effect = effect;
        AX_SAFE_RETAIN(_effect);

        setProgramState(_effect->newProgramState());
    }
}


void EffectSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if (_effect != nullptr)
    {
        _effect->prepareForRender(this, _normalmap);
    }
    Sprite::draw(renderer, transform, flags);
}

EffectSprite::~EffectSprite()
{
    AX_SAFE_RELEASE(_effect);
    AX_SAFE_RELEASE(_normalmap);
}
