/****************************************************************************
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

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "axmol.h"

//
// Effect
//
class Effect : public ax::Ref
{
public:
    ax::backend::ProgramState* getProgramState() const { return _programState; }

protected:
    bool initProgramState(std::string_view fragmentFilename);
    Effect();
    virtual ~Effect();
    ax::backend::ProgramState* _programState = nullptr;
#if (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID)
    std::string _fragSource;
    ax::EventListenerCustom* _backgroundListener;
#endif
    
    template <typename T>
    void setUniform(std::string uniform, T value);
};

template<typename T>
void Effect::setUniform(std::string uniformName, T value)
{
    auto uniformLocation = _programState->getUniformLocation(uniformName);
    _programState->setUniform(uniformLocation, &value, sizeof(value));
}

#endif
