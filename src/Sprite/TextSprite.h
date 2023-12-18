/**
 * @file TextSprite.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-13
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __COMMON_SPRITE_TEXT_SPRITE_H
#define __COMMON_SPRITE_TEXT_SPRITE_H

#include "Common/Sprite/Sprite.h"

enum class TextAlign
{
  Left,
  Center,
  Right
};

class TextSprite : public Sprite
{
public:
  TextSprite(const char* text, unsigned font_size, plug::Color text_color,
             TextAlign align = TextAlign::Center);

  TextSprite(const TextSprite& other);

  TextSprite& operator=(const TextSprite& other);

  void setText(const char* text);

  const char* getText(void) const { return m_text; }

  void setFontSize(unsigned font_size) { m_fontSize = font_size; }

  unsigned getFontSize(void) const { return m_fontSize; }

  virtual void draw(const plug::LayoutBox& layout_box,
                    plug::TransformStack&  stack,
                    plug::RenderTarget&    target) const override;

  virtual Sprite* clone(void) const override { return new TextSprite(*this); }

  virtual ~TextSprite(void) override { delete[] m_text; }

private:
  static constexpr size_t MAX_TEXT_LEN = 256;

  char*  m_text;
  size_t m_textCapacity;
  size_t m_textLen;

  unsigned    m_fontSize;
  plug::Color m_color;
  TextAlign   m_alignment;
};

#endif /* TextSprite.h */
