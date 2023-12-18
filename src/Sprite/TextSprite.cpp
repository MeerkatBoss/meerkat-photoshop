#include "Sprite/TextSprite.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cstdio>
#include <cstring>

#include "AssetManager.h"
#include "Common/Math.h"
#include "Impl/Util/Sfml.h"

TextSprite::TextSprite(const char* text, unsigned font_size,
                       plug::Color text_color, TextAlign align) :
    m_text(nullptr),
    m_textCapacity(0),
    m_textLen(0),
    m_fontSize(font_size),
    m_color(text_color),
    m_alignment(align)
{
  const size_t text_len = strnlen(text, MAX_TEXT_LEN);
  m_text                = new char[text_len + 1];
  strncpy(m_text, text, text_len);
  m_text[text_len] = '\0';

  m_textCapacity = text_len;
  m_textLen      = text_len;
}

TextSprite::TextSprite(const TextSprite& other) :
    m_text(new char[other.m_textLen + 1]),
    m_textCapacity(other.m_textLen),
    m_textLen(other.m_textLen),
    m_fontSize(other.m_fontSize),
    m_color(other.m_color),
    m_alignment(other.m_alignment)
{
  strncpy(m_text, other.m_text, m_textLen);
  m_text[m_textLen] = '\0';
}

TextSprite& TextSprite::operator=(const TextSprite& other)
{
  if (this == &other)
  {
    return *this;
  }

  m_color = other.m_color;
  setText(other.getText());

  return *this;
}

void TextSprite::setText(const char* text)
{
  const size_t text_len = strnlen(text, MAX_TEXT_LEN);

  if (text_len > m_textCapacity)
  {
    delete[] m_text;
    m_textCapacity = text_len;
    m_text         = new char[text_len + 1];
  }

  m_textLen = text_len;
  strncpy(m_text, text, m_textLen);
  m_text[m_textLen] = '\0';
}

static inline double min(double a, double b) { return a < b ? a : b; }

struct Corners
{
  Vec2d tl, tr, bl, br;
};

static inline Corners getCornersFromSize(const Vec2d& size)
{
  return Corners{.tl = Vec2d(0, 0),
                 .tr = Vec2d(size.x, 0),
                 .bl = Vec2d(0, size.y),
                 .br = Vec2d(size.x, size.y)};
}

static inline Vec2d fitSize(const Vec2d& from, const Vec2d& to)
{
  const double scale_x = to.x / from.x;
  const double scale_y = to.y / from.y;

  if (scale_x < scale_y)
  {
    return scale_x * from;
  }
  else
  {
    return scale_y * from;
  }
}

void TextSprite::draw(const plug::LayoutBox& layout_box,
                      plug::TransformStack&  stack,
                      plug::RenderTarget&    target) const
{
  static constexpr size_t  render_width       = 16'000;
  static constexpr size_t  render_height      = 100;
  static constexpr double  font_size          = 50;
  static bool              is_texture_created = false;
  static sf::RenderTexture render_texture;
  if (!is_texture_created)
  {
    render_texture.create(render_width, render_height);
    is_texture_created = true;
  }

  sf::Text text(m_text, AssetManager::getDefaultFont());
  text.setCharacterSize(font_size);
  text.setFillColor(getSFMLColor(m_color));

  render_texture.clear(sf::Color(0, 0, 0, 0));
  render_texture.draw(text);
  render_texture.display();

  static constexpr double margin_y = 0.3;

  const double width =
      text.getGlobalBounds().width + text.getGlobalBounds().left;
  const double height = double(font_size);

  const Vec2d text_size(min(width, render_width),
                        min((1 + margin_y) * height, render_height));
  const auto [tex_tl, tex_tr, tex_bl, tex_br] = getCornersFromSize(text_size);

  const Vec2d  max_size       = fitSize(text_size, layout_box.getSize());
  const double text_scale     = double(m_fontSize) / max_size.y;
  const Vec2d  display_size   = text_scale * max_size;
  const auto [tl, tr, bl, br] = getCornersFromSize(display_size);

  Vec2d offset = Vec2d();
  if (m_alignment == TextAlign::Left)
  {
    offset = Vec2d(0, 0);
  }
  else if (m_alignment == TextAlign::Center)
  {
    offset = (layout_box.getSize() - display_size) / 2;
  }
  else
  {
    offset = layout_box.getSize() - display_size;
  }

  stack.enter(Transform(layout_box.getPosition() + offset));

  plug::VertexArray array(plug::TriangleStrip, 4);
  array[0] = plug::Vertex{.position = stack.apply(tl), .tex_coords = tex_tl};
  array[1] = plug::Vertex{.position = stack.apply(tr), .tex_coords = tex_tr};
  array[2] = plug::Vertex{.position = stack.apply(bl), .tex_coords = tex_bl};
  array[3] = plug::Vertex{.position = stack.apply(br), .tex_coords = tex_br};

  stack.leave();

  sf::Image     image = render_texture.getTexture().copyToImage();
  plug::Texture texture(image.getSize().x, image.getSize().y,
                        (const plug::Color*)image.getPixelsPtr());
  target.draw(array, texture);
}
