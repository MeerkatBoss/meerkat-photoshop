#include "GUI/Titlebar.h"

#include <SFML/Graphics/Text.hpp>
#include <cstdio>
#include <cstring>

#include "AssetManager.h"
#include "GUI/Widget.h"
#include "Impl/Util/Sfml.h"
#include "Math.h"
#include "Sprite/CompositeSprite.h"
#include "Sprite/RectangleSprite.h"
#include "Sprite/TextSprite.h"

static const plug::Color bg_color(70, 70, 80);
static const plug::Color fg_color(240, 240, 240);

namespace gui
{

Titlebar::Titlebar(const char* name, const plug::LayoutBox& layout_box) :
    Widget(layout_box), m_name(nullptr), m_sprite()
{
  setName(name);
  /*
  m_sprite.addLayer(RectangleSprite(bg_color, 0));
  m_sprite.addLayer(TextSprite(m_name, 0.9 * getLayoutBox().getSize().y,
                               fg_color, TextAlign::Center));
                               */
}

void Titlebar::setName(const char* name)
{
  const size_t name_len = strlen(name);
  m_name = new char[name_len + 1];
  strncpy(m_name, name, name_len);
  m_name[name_len] = '\0';

  m_sprite.clear();
  m_sprite.addLayer(RectangleSprite(bg_color, 0));
  m_sprite.addLayer(TextSprite(m_name, 0.9 * getLayoutBox().getSize().y,
                               fg_color, TextAlign::Center));
}

void Titlebar::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  Widget::draw(stack, target);
  m_sprite.draw(getLayoutBox(), stack, target);
}

} // namespace gui
