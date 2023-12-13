#include "GUI/ColorSelector.h"

#include "GUI/Widget.h"
#include "Layout/LayoutBox.h"
#include "Layout/Units.h"
#include "Math.h"
#include "Sprite/RectangleSprite.h"

namespace gui
{

void ColorSelector::ColorRect::draw(plug::TransformStack& stack,
                                    plug::RenderTarget&   target)
{
  RectangleSprite sprite(m_color, 10);
  sprite.draw(getLayoutBox(), stack, target);
  /*
  auto [tl, tr, bl, br] = getRect(getLayoutBox());
  plug::VertexArray array(plug::TriangleStrip, 4);

  array[0] = plug::Vertex{
      .position = stack.apply(tl), .tex_coords = Vec2d(), .color = m_color};
  array[1] = plug::Vertex{
      .position = stack.apply(tr), .tex_coords = Vec2d(), .color = m_color};
  array[2] = plug::Vertex{
      .position = stack.apply(bl), .tex_coords = Vec2d(), .color = m_color};
  array[3] = plug::Vertex{
      .position = stack.apply(br), .tex_coords = Vec2d(), .color = m_color};
  target.draw(array);
  */
}

ColorSelector::~ColorSelector(void)
{
  const size_t selector_count = m_selectors.getSize();
  for (size_t i = 0; i < selector_count; ++i)
  {
    delete m_selectors[i];
  }
}

void ColorSelector::draw(plug::TransformStack& stack,
                         plug::RenderTarget&   target)
{
  const plug::Color bg_color(200, 200, 190);

  auto [tl, tr, bl, br] = getRect(getLayoutBox());
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

  m_mainRect.draw(stack, target);

  const size_t selector_count = m_selectors.getSize();
  for (size_t i = 0; i < selector_count; ++i)
  {
    m_selectors[i]->draw(stack, target);
  }
}

void ColorSelector::onParentUpdate(const plug::LayoutBox& parent_box)
{
  Widget::onParentUpdate(parent_box);
  m_mainRect.onParentUpdate(getLayoutBox());

  const size_t selector_count = m_selectors.getSize();
  for (size_t i = 0; i < selector_count; ++i)
  {
    m_selectors[i]->onParentUpdate(getLayoutBox());
  }
}

void ColorSelector::addColor(plug::Color color)
{
  const size_t selector_count = m_selectors.getSize();
  const size_t length_mm      = 5 + 20 * selector_count;

  layout::LayoutBox box(
      100_per, layout::Length(15 + length_mm, layout::Unit::Millimeter),
      layout::Align::BottomCenter);

  box.setPadding(0_px, layout::Length(length_mm, layout::Unit::Millimeter),
                 5_mm, 5_mm);

  m_selectors.pushBack(new ColorRect(color, box));
}

void ColorSelector::onMousePressed(const plug::MousePressedEvent& event,
                                   plug::EHC&                     context)
{
  if (context.stopped || context.overlapped)
  {
    return;
  }

  if (covers(context.stack, event.pos))
  {
    context.overlapped = true;

    context.stack.enter(Transform(getLayoutBox().getPosition()));

    const size_t selector_count = m_selectors.getSize();
    for (size_t i = 0; i < selector_count; ++i)
    {
      if (m_selectors[i]->covers(context.stack, event.pos))
      {
        m_palette.setFGColor(m_selectors[i]->getColor());
        m_mainRect.setColor(m_selectors[i]->getColor());
        break;
      }
    }

    context.stack.leave();
  }
}

} // namespace gui
