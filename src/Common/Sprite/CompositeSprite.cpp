#include "Common/Sprite/CompositeSprite.h"

#include <cstdio>

CompositeSprite::~CompositeSprite(void)
{
  const size_t sprite_count = m_sprites.getSize();
  for (size_t i = 0; i < sprite_count; ++i)
  {
    delete m_sprites[i];
  }
}

void CompositeSprite::addLayer(const Sprite& sprite)
{
  m_sprites.pushBack(sprite.clone());
}

void CompositeSprite::draw(const plug::LayoutBox& layout_box,
                           plug::TransformStack&  stack,
                           plug::RenderTarget&    target) const
{
  const size_t sprite_count = m_sprites.getSize();
  for (size_t i = 0; i < sprite_count; ++i)
  {
    m_sprites[i]->draw(layout_box, stack, target);
  }
}

Sprite* CompositeSprite::clone(void) const
{
  CompositeSprite* copied = new CompositeSprite();

  const size_t sprite_count = m_sprites.getSize();
  for (size_t i = 0; i < sprite_count; ++i)
  {
    copied->addLayer(*m_sprites[i]);
  }
  return copied;
}
