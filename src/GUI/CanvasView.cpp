#include "GUI/CanvasView.h"

#include "GUI/Widget.h"

namespace gui
{

static constexpr double THICKNESS = 10;

void CanvasView::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  using plug::Vec2d;
  using plug::Vertex;

  const auto [tl, tr, bl, br] = getRect(getLayoutBox());
  const Vec2d canvas_size     = m_canvas.getSize();

  const Vec2d tex_tl(0, 0);
  const Vec2d tex_tr(canvas_size.x, 0);
  const Vec2d tex_bl(0, canvas_size.y);
  const Vec2d tex_br(canvas_size.x, canvas_size.y);

  plug::VertexArray array(plug::TriangleStrip, 4);
  array[0] = Vertex{.position = stack.apply(tl), .tex_coords = tex_tl};
  array[1] = Vertex{.position = stack.apply(tr), .tex_coords = tex_tr};
  array[2] = Vertex{.position = stack.apply(bl), .tex_coords = tex_bl};
  array[3] = Vertex{.position = stack.apply(br), .tex_coords = tex_br};

  target.draw(array, m_canvas.getTexture());
}

void CanvasView::onMousePressed(const plug::MousePressedEvent& event,
                                plug::EHC&                     context)
{
  if (context.overlapped || context.stopped)
  {
    return;
  }

  if (covers(context.stack, event.pos))
  {
    context.overlapped = true;
    if (event.button_id == plug::MouseButton::Left && !m_isDrawing)
    {
      context.stopped = true;
      context.stack.enter(getCanvasTransform());

      m_isDrawing = true;
      m_lastPos   = event.pos;
      drawLine(context.stack.restore(event.pos),
               context.stack.restore(event.pos), THICKNESS);

      context.stack.leave();
    }
  }
}

void CanvasView::onMouseReleased(const plug::MouseReleasedEvent& event,
                                 plug::EHC&                      context)
{
  if (covers(context.stack, event.pos))
  {
    context.overlapped = true;
  }
  if (m_isDrawing)
  {
    context.stack.enter(getCanvasTransform());
    drawLine(context.stack.restore(event.pos), context.stack.restore(event.pos),
             THICKNESS);
    context.stack.leave();

    m_isDrawing = false;
  }
}

void CanvasView::onMouseMove(const plug::MouseMoveEvent& event,
                             plug::EHC&                  context)
{
  if (context.stopped)
  {
    return;
  }
  if (covers(context.stack, event.pos))
  {
    context.overlapped = true;
    if (m_isDrawing)
    {
      context.stack.enter(getCanvasTransform());
      drawLine(context.stack.restore(m_lastPos),
               context.stack.restore(event.pos), THICKNESS);
      context.stack.leave();
      m_lastPos = event.pos;
    }
  }
}

plug::Transform CanvasView::getCanvasTransform(void) const
{
  return plug::Transform(getLayoutBox().getPosition(),
                         getLayoutBox().getSize() / m_canvas.getSize());
}

void CanvasView::drawLine(const plug::Vec2d start, const plug::Vec2d end,
                          double thickness)
{
  const size_t      point_count = 20;
  const plug::Color brush_color(0, 0, 0);

  const plug::Vec2d direction = end - start;
  plug::VertexArray array(plug::TriangleFan, point_count + 1);

  for (size_t i = 0; i < point_count; ++i)
  {
    const double      angle = i * (2 * M_PI / point_count);
    const plug::Vec2d offset(cos(angle), sin(angle));

    if (dot(offset, direction) < 0)
    {
      array[i] = plug::Vertex{.position   = start + thickness * offset,
                              .tex_coords = plug::Vec2d(),
                              .color      = brush_color};
    }
    else
    {
      array[i] = plug::Vertex{.position   = end + thickness * offset,
                              .tex_coords = plug::Vec2d(),
                              .color      = brush_color};
    }
  }
  array[point_count] = array[0];

  m_canvas.draw(array);
}

} // namespace gui
