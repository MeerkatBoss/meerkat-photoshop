#include "GUI/Widget.h"

#include "Plug/Color.h"
#include "Plug/Math/Vec2d.h"

namespace gui
{

Rect getRect(const plug::LayoutBox& box)
{
  const plug::Vec2d position = box.getPosition();
  const plug::Vec2d size     = box.getSize();
  const plug::Vec2d off_x(size.x, 0);
  const plug::Vec2d off_y(0, size.y);

  return Rect{.tl = position,
              .tr = position + off_x,
              .bl = position + off_y,
              .br = position + size};
}

Widget& Widget::operator=(const Widget& widget)
{
  if (this == &widget)
  {
    return *this;
  }

  delete m_box;
  m_box = widget.m_box->clone();
  return *this;
}

static plug::Vertex make_vertex(const plug::Vec2d& pos, plug::Color color)
{
  return {.position = pos, .tex_coords = plug::Vec2d(), .color = color};
}

void Widget::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  auto [tl, tr, bl, br] = getRect(getLayoutBox());
  plug::Color magenta(255, 0, 255);

  plug::VertexArray vertices(plug::PrimitiveType::TriangleStrip, 4);
  vertices[0] = make_vertex(stack.apply(tl), magenta);
  vertices[1] = make_vertex(stack.apply(tr), magenta);
  vertices[2] = make_vertex(stack.apply(bl), magenta);
  vertices[3] = make_vertex(stack.apply(br), magenta);

  target.draw(vertices);
}

void Widget::onEvent(const plug::Event& event, plug::EHC& context)
{
  switch (event.getType())
  {
  case plug::EventType::Tick:
    return onTick(static_cast<const plug::TickEvent&>(event), context);
  case plug::EventType::KeyboardPressed:
    return onKeyboardPressed(
        static_cast<const plug::KeyboardPressedEvent&>(event), context);
  case plug::EventType::KeyboardReleased:
    return onKeyboardReleased(
        static_cast<const plug::KeyboardReleasedEvent&>(event), context);
  case plug::EventType::MousePressed:
    return onMousePressed(static_cast<const plug::MousePressedEvent&>(event),
                          context);
  case plug::EventType::MouseReleased:
    return onMouseReleased(static_cast<const plug::MouseReleasedEvent&>(event),
                           context);
  case plug::EventType::MouseMove:
    return onMouseMove(static_cast<const plug::MouseMoveEvent&>(event),
                       context);
  default:
    break;
  }
}

static inline bool isSmall(double a) { return fabs(a) < 1e-6; }

bool Widget::covers(plug::TransformStack&, const plug::Vec2d& position) const
{
  if (isSmall(m_box->getSize().x) || isSmall(m_box->getSize().y))
  {
    return false;
  }

  auto [tl, tr, bl, br] = getRect(getLayoutBox());

  bool top_check    = plug::cross(tl - tr, position - tr) <= 0.0;
  bool right_check  = plug::cross(tr - br, position - br) <= 0.0;
  bool bottom_check = plug::cross(br - bl, position - bl) <= 0.0;
  bool left_check   = plug::cross(bl - tl, position - tl) <= 0.0;

  return top_check && right_check && bottom_check && left_check;
}

} // namespace gui
