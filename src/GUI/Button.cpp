#include "Button.h"

namespace gui
{

size_t Button::s_idCounter = 0;

void Button::onMousePressed(const plug::MousePressedEvent& event,
                            plug::EHC&                     context)
{
  if (context.stopped || context.overlapped)
  {
    return;
  }
  if (covers(context.stack, event.pos))
  {
    m_controller.onClick(m_buttonId);
    m_pressed = true;

    context.stopped    = true;
    context.overlapped = true;
  }
}

void Button::onMouseReleased(const plug::MouseReleasedEvent& event, plug::EHC&)
{
  m_controller.onRelease(m_buttonId);
  m_pressed = false;
}

void Button::onMouseMove(const plug::MouseMoveEvent& event, plug::EHC& context)
{
  if (context.overlapped)
  {
    return;
  }
  if (covers(context.stack, event.pos))
  {
    m_hovered = true;

    context.overlapped = true;
  }
}

void Button::onTick(const plug::TickEvent& event, plug::EHC&)
{
  if (m_pressed)
  {
    m_controller.onHold(m_buttonId, event.delta_time);
  }
}

static plug::Vertex make_vertex(const plug::Vec2d& pos, plug::Color color)
{
  return {.position = pos, .tex_coords = plug::Vec2d(), .color = color};
}

void Button::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  auto [tl, tr, bl, br] = getRect(getLayoutBox());
  plug::Color gray(200, 200, 200);

  plug::VertexArray vertices(plug::PrimitiveType::TriangleStrip, 4);
  vertices[0] = make_vertex(stack.apply(tl), gray);
  vertices[1] = make_vertex(stack.apply(tr), gray);
  vertices[2] = make_vertex(stack.apply(bl), gray);
  vertices[3] = make_vertex(stack.apply(br), gray);

  target.draw(vertices);
}

} // namespace gui
