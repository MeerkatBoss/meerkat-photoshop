/**
 * @file CompositeSprite.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __COMMON_SPRITE_COMPOSITE_SPRITE_H
#define __COMMON_SPRITE_COMPOSITE_SPRITE_H

#include "DynArray.h"
#include "Sprite/Sprite.h"

class CompositeSprite : public Sprite
{
public:
  CompositeSprite(void) : m_sprites() {}

  virtual ~CompositeSprite(void) override;

  void addLayer(const Sprite& sprite);

  virtual void draw(const plug::LayoutBox& layout_box,
                    plug::TransformStack&  stack,
                    plug::RenderTarget&    target) const override;

  virtual Sprite* clone(void) const override;

private:
  DynArray<Sprite*> m_sprites;
};

#endif /* CompositeSprite.h */
