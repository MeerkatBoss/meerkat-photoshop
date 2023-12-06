#include "GUI/Titlebar.h"

#include <SFML/Graphics/Text.hpp>

#include "AssetManager.h"
#include "Common/Math.h"
#include "Impl/Util/Sfml.h"

namespace gui
{

void Titlebar::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  const plug::Color bg_color(70, 70, 80);
  const sf::Color   fg_color(240, 240, 240);

  sf::Text text(m_name, AssetManager::getDefaultFont(),
                getLayoutBox().getSize().y * 0.5);
  text.setFillColor(fg_color);

  m_renderTexture.clear(getSFMLColor(bg_color));
  m_renderTexture.draw(text);
  m_renderTexture.display();

  sf::Image     image = m_renderTexture.getTexture().copyToImage();
  plug::Texture texture(image.getSize().x, image.getSize().y,
                        (const plug::Color*)image.getPixelsPtr());

  const auto [tl, tr, bl, br] = getRect(getLayoutBox());

  const double text_width  = text.getGlobalBounds().width * 1.1;
  const double text_height = text.getGlobalBounds().height;
  const double text_offset = (getLayoutBox().getSize().x - text_width) / 2;

  const Vec2d text_tl = tl + Vec2d(text_offset, 0);
  const Vec2d text_tr = tr - Vec2d(text_offset, 0);
  const Vec2d text_bl = bl + Vec2d(text_offset, 0);
  const Vec2d text_br = br - Vec2d(text_offset, 0);

  const Vec2d texture_tl(0, 0);
  const Vec2d texture_tr(text_width, 0);
  const Vec2d texture_bl(0, text_height * 1.5);
  const Vec2d texture_br(text_width, text_height * 1.5);

  stack.enter(Transform(getLayoutBox().getPosition()));

  plug::VertexArray array(plug::TriangleStrip, 4);

  array[0] = plug::Vertex{
      .position = stack.apply(tl), .tex_coords = Vec2d(), .color = bg_color};
  array[1] = plug::Vertex{
      .position = stack.apply(tr), .tex_coords = Vec2d(), .color = bg_color};
  array[2] = plug::Vertex{
      .position = stack.apply(bl), .tex_coords = Vec2d(), .color = bg_color};
  array[3] = plug::Vertex{
      .position = stack.apply(br), .tex_coords = Vec2d(), .color = bg_color};
  target.draw(array);

  array[0] = plug::Vertex{.position   = stack.apply(text_tl),
                          .tex_coords = texture_tl,
                          .color      = plug::Color()};
  array[1] = plug::Vertex{.position   = stack.apply(text_tr),
                          .tex_coords = texture_tr,
                          .color      = plug::Color()};
  array[2] = plug::Vertex{.position   = stack.apply(text_bl),
                          .tex_coords = texture_bl,
                          .color      = plug::Color()};
  array[3] = plug::Vertex{.position   = stack.apply(text_br),
                          .tex_coords = texture_br,
                          .color      = plug::Color()};
  target.draw(array, texture);

  stack.leave();
}

} // namespace gui
