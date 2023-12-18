/**
 * @file RectangleSprite.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __COMMON_SPRITE_RECTANGLE_SPRITE_H
#define __COMMON_SPRITE_RECTANGLE_SPRITE_H

#include "Common/Sprite/Sprite.h"

class RectangleSprite : public Sprite
{
public:
  RectangleSprite(plug::Color color, double radius) :
      m_color(color), m_radius(radius)
  {
  }

  RectangleSprite(const RectangleSprite& other) = default;

  virtual void draw(const plug::LayoutBox& layout_box,
                    plug::TransformStack&  stack,
                    plug::RenderTarget&    target) const override;

  virtual Sprite* clone(void) const override
  {
    return new RectangleSprite(*this);
  }

private:
  plug::Color m_color;
  double      m_radius;
};

#endif /* RectangleSprite.h */
