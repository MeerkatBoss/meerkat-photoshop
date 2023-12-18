#include "GUI/Titlebar.h"

#include <SFML/Graphics/Text.hpp>
#include <cstdio>

#include "AssetManager.h"
#include "Impl/Util/Sfml.h"
#include "Math.h"
#include "Sprite/CompositeSprite.h"
#include "Sprite/RectangleSprite.h"
#include "Sprite/TextSprite.h"

namespace gui
{

void Titlebar::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  const plug::Color bg_color(70, 70, 80);
  const plug::Color fg_color(240, 240, 240);

  CompositeSprite sprite;
  sprite.addLayer(RectangleSprite(bg_color, 0));
  sprite.addLayer(TextSprite(m_name, 0.9 * getLayoutBox().getSize().y, fg_color,
                             TextAlign::Center));
  sprite.draw(getLayoutBox(), stack, target);
}

} // namespace gui
