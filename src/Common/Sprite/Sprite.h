/**
 * @file Sprite.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __COMMON_SPRITE_SPRITE_H
#define __COMMON_SPRITE_SPRITE_H

#include "Common/Plug/Graphics.h"
#include "Common/Plug/LayoutBox.h"
#include "Common/Plug/Math.h"

class Sprite
{
public:
  virtual void draw(const plug::LayoutBox& layout_box, plug::TransformStack& stack,
            plug::RenderTarget& target) const = 0;

  virtual Sprite* clone(void) const = 0;

  virtual ~Sprite() = default;
};

#endif /* Sprite.h */
