#include "LightEffect.h"

USING_NS_AX;


LightEffect* LightEffect::create()
{
    LightEffect *normalMappedSprite = new (std::nothrow) LightEffect();
    if (normalMappedSprite && normalMappedSprite->init())
    {
        normalMappedSprite->autorelease();
        return normalMappedSprite;
    }
    AX_SAFE_DELETE(normalMappedSprite);
    return nullptr;
}

bool LightEffect::init()
{
    if (initProgram("pointlight.frag"))
    {
        setLightColor(Color3B::WHITE);
        setAmbientLightColor(Color3B(63,63,63));
        setLightCutoffRadius(500.0f);
        setLightHalfRadius(0.5f);
        return true;
    }
    return false;
}

void LightEffect::setLightPos(const Vec3 &pos)
{
    _lightPos = pos;
}

void LightEffect::setLightColor(const Color3B &color)
{
    _lightColor = color;
}

void LightEffect::setAmbientLightColor(const Color3B &color)
{
    _ambientLightColor = color;
}

void LightEffect::setBrightness(float value)
{
    _brightness = value;
}

void LightEffect::setLightCutoffRadius(float value)
{
    _lightCutoffRadius = std::max(1.0f, value);
}

void LightEffect::setLightHalfRadius(float value)
{
    _lightHalfRadius = std::max(0.01f, std::min(0.99f, value));
}

void LightEffect::prepareForRender(Sprite *sprite, Texture2D *normalmap)
{
    // set light configuration
    backend::ProgramState *state = sprite->getProgramState();
    setUniform(state, "u_lightColor", Vec3(_lightColor.r,_lightColor.g,_lightColor.b)/255.0f);
    setUniform(state, "u_ambientColor", Vec3(_ambientLightColor.r,_ambientLightColor.g,_ambientLightColor.b)/255.0f);
    setUniform(state, "u_brightness", _brightness);
    setUniform(state, "u_cutoffRadius", _lightCutoffRadius);
    setUniform(state, "u_halfRadius", _lightHalfRadius);

    Point posRelToSprite = PointApplyAffineTransform(Point(_lightPos.x, _lightPos.y), sprite->getWorldToNodeAffineTransform());
    setUniform(state, "u_lightPos", Vec3(posRelToSprite.x, posRelToSprite.y, _lightPos.z));

    // set normal map texture
    auto uniformLocation = state->getUniformLocation("u_normals");
    state->setTexture(uniformLocation, 0, normalmap->getBackendTexture());

    SpriteFrame *frame = sprite->getSpriteFrame();
    Size untrimmedSize = frame->getOriginalSize();
    Size trimmedSize = frame->getRect().size;
    Vec2 framePos = frame->getRect().origin;
    Size texSize = frame->getTexture()->getContentSize();
    
    // set sprite position in sheet
    setUniform(state, "u_contentSize", sprite->getContentSize());
    setUniform(state, "u_spritePosInSheet", Vec2(framePos.x / texSize.width, framePos.y / texSize.height));
    setUniform(state, "u_spriteSizeRelToSheet", Vec2(untrimmedSize.width / texSize.width, untrimmedSize.height / texSize.height));
    setUniform(state, "u_spriteRotated", int(frame->isRotated()));
    
    // set offset of trimmed sprite
    Vec2 bottomLeft = frame->getOffset() + (untrimmedSize - trimmedSize) / 2;
    Vec2 cornerOffset = frame->isRotated() ? Vec2(bottomLeft.y, bottomLeft.x)
                                           : Vec2(bottomLeft.x, untrimmedSize.height - trimmedSize.height - bottomLeft.y);
    setUniform(state, "u_spriteOffset", cornerOffset);
}
