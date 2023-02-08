#ifndef _EFFECTSPRITE_H_
#define _EFFECTSPRITE_H_

#include "axmol.h"

class LightEffect;

class EffectSprite : public ax::Sprite
{
public:
    static EffectSprite *create(const std::string_view &filename);
    static EffectSprite *createWithSpriteFrame(ax::SpriteFrame *spriteFrame);
    
    void setEffect(LightEffect *effect, const std::string &normalMapFilename);
    
protected:
    virtual void draw(ax::Renderer *renderer, const ax::Mat4 &transform, uint32_t flags) override;
    
    ~EffectSprite();
    
    LightEffect *_effect = nullptr;
    ax::Texture2D *_normalmap = nullptr;
};

#endif
