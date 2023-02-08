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

#include "Effect.h"

USING_NS_AX;


bool Effect::initProgramState(std::string_view fragmentFilename)
{
    auto fileUtiles       = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
    auto fragSource       = fileUtiles->getStringFromFile(fragmentFullPath);

#if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID)
    _fragSource = fragSource;
#endif
    auto program      = ProgramManager::newProgram(positionTextureColor_vert, fragSource, VertexLayoutHelper::setupSprite);
    auto programState = new backend::ProgramState(program);
    AX_SAFE_RELEASE(_programState);
    AX_SAFE_RELEASE(program);
    _programState = programState;

    return _programState != nullptr;
}


Effect::Effect()
: _programState(nullptr)
{
#if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = _glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}


Effect::~Effect()
{
    AX_SAFE_RELEASE_NULL(_programState);
#if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}
