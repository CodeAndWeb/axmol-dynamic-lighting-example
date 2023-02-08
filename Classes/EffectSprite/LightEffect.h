#ifndef _LIGHTEFFECT_H_
#define _LIGHTEFFECT_H_

#include "Effect.h"


class LightEffect : public Effect
{
public:
    static LightEffect* create();
    
    void setLightPos(const ax::Vec3& pos);
    void setLightColor(const ax::Color3B& color);
    void setAmbientLightColor(const ax::Color3B& color);
    void setBrightness(float value);
    void setLightCutoffRadius(float value);
    void setLightHalfRadius(float value);
    
    void prepareForRender(ax::Sprite *sprite, ax::Texture2D *normalmap);
    
    ax::Color3B getLightColor() const { return _lightColor; }
    ax::Color3B getAmbientLightColor() const { return _ambientLightColor; }

protected:
    bool init();

    ax::Vec3 _lightPos;
    ax::Color3B _lightColor;
    ax::Color3B _ambientLightColor;
    float _lightCutoffRadius;
    float _lightHalfRadius;
    float _brightness;
};

#endif
