#include "GUI/CanvasView.h"

#include "Common/GUI/Widget.h"

namespace gui
{

void CanvasView::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  using plug::Vec2d;
  using plug::Vertex;

  const plug::Color bg_color(100, 100, 100);

  plug::VertexArray array(plug::TriangleStrip, 4);

  if (m_isFocused)
  {
    const plug::Color border_color(200, 128, 0);
    const auto [tl, tr, bl, br] = getRect(getLayoutBox());
    const double border_width   = 5;
    const Vec2d  x_off(border_width, 0);
    const Vec2d  y_off(0, border_width);

    array[0] = Vertex{.position   = stack.apply(tl - x_off - y_off),
                      .tex_coords = Vec2d(),
                      .color      = border_color};
    array[1] = Vertex{.position   = stack.apply(tr + x_off - y_off),
                      .tex_coords = Vec2d(),
                      .color      = border_color};
    array[2] = Vertex{.position   = stack.apply(bl - x_off + y_off),
                      .tex_coords = Vec2d(),
                      .color      = border_color};
    array[3] = Vertex{.position   = stack.apply(br + x_off + y_off),
                      .tex_coords = Vec2d(),
                      .color      = border_color};
    target.draw(array);
  }

  const auto [tl, tr, bl, br] = getRect(getLayoutBox());

  // Draw background
  array[0] = Vertex{
      .position = stack.apply(tl), .tex_coords = Vec2d(), .color = bg_color};
  array[1] = Vertex{
      .position = stack.apply(tr), .tex_coords = Vec2d(), .color = bg_color};
  array[2] = Vertex{
      .position = stack.apply(bl), .tex_coords = Vec2d(), .color = bg_color};
  array[3] = Vertex{
      .position = stack.apply(br), .tex_coords = Vec2d(), .color = bg_color};
  target.draw(array);

  const Vec2d canvas_size = m_canvas.getSize();

  const Vec2d tex_tl(0, 0);
  const Vec2d tex_tr(canvas_size.x, 0);
  const Vec2d tex_bl(0, canvas_size.y);
  const Vec2d tex_br(canvas_size.x, canvas_size.y);

  stack.enter(getCanvasTransform());

  // Draw canvas
  array[0] = Vertex{.position = stack.apply(tex_tl), .tex_coords = tex_tl};
  array[1] = Vertex{.position = stack.apply(tex_tr), .tex_coords = tex_tr};
  array[2] = Vertex{.position = stack.apply(tex_bl), .tex_coords = tex_bl};
  array[3] = Vertex{.position = stack.apply(tex_br), .tex_coords = tex_br};
  target.draw(array, m_canvas.getTexture());

  // Draw tool widgets
  plug::Widget* tool_widget = m_palette.getActiveTool().getWidget();
  if (m_isFocused && tool_widget != nullptr)
  {
    tool_widget->draw(stack, target);
  }
  stack.leave();

  stack.enter(Transform(getLayoutBox().getPosition()));
  m_titlebar.draw(stack, target);
  stack.leave();
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
    if (event.button_id == plug::MouseButton::Left)
    {
      context.stack.enter(Transform(getLayoutBox().getPosition()));
      if (m_titlebar.covers(context.stack, event.pos))
      {
        m_isMoving = true;
        m_lastPos  = context.stack.restore(event.pos);
      }
      context.stack.leave();

      if (m_isMoving)
      {
        return;
      }

      context.stopped = true;
      context.stack.enter(getCanvasTransform());

      m_palette.getActiveTool().onMainButton({plug::State::Pressed},
                                             context.stack.restore(event.pos));

      context.stack.leave();
    }
    else if (event.button_id == plug::MouseButton::Right)
    {
      context.stopped = true;
      context.stack.enter(getCanvasTransform());

      m_palette.getActiveTool().onSecondaryButton(
          {plug::State::Pressed}, context.stack.restore(event.pos));

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

  if (event.button_id == plug::MouseButton::Left)
  {
    context.stopped = true;
    context.stack.enter(getCanvasTransform());
    if (m_isMoving)
    {
      m_isMoving = false;
      return;
    }

    m_palette.getActiveTool().onMainButton({plug::State::Released},
                                           context.stack.restore(event.pos));

    context.stack.leave();
  }
  else if (event.button_id == plug::MouseButton::Right)
  {
    context.stopped = true;
    context.stack.enter(getCanvasTransform());

    m_palette.getActiveTool().onSecondaryButton(
        {plug::State::Released}, context.stack.restore(event.pos));

    context.stack.leave();
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
  }

  if (m_isMoving)
  {
    const Vec2d parent_pos = context.stack.restore(event.pos);

    getLayoutBox().setPosition(parent_pos - m_lastPos);

    return;
  }

  context.stack.enter(getCanvasTransform());
  m_palette.getActiveTool().onMove(context.stack.restore(event.pos));
  context.stack.leave();
}

void CanvasView::onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                   plug::EHC&                        context)
{
  if (context.stopped)
  {
    return;
  }
  if (event.key_id == plug::KeyCode::LShift ||
      event.key_id == plug::KeyCode::RShift)
  {
    context.stopped = true;
    m_palette.getActiveTool().onModifier1({plug::State::Pressed});
  }
  if (event.key_id == plug::KeyCode::LControl ||
      event.key_id == plug::KeyCode::RControl)
  {
    context.stopped = true;
    m_palette.getActiveTool().onModifier2({plug::State::Pressed});
  }
  if (event.key_id == plug::KeyCode::LAlt ||
      event.key_id == plug::KeyCode::RAlt)
  {
    context.stopped = true;
    m_palette.getActiveTool().onModifier3({plug::State::Pressed});
  }
}

void CanvasView::onKeyboardReleased(const plug::KeyboardReleasedEvent& event,
                                    plug::EHC&                         context)
{
  if (context.stopped)
  {
    return;
  }
  if (event.key_id == plug::KeyCode::LShift ||
      event.key_id == plug::KeyCode::RShift)
  {
    m_palette.getActiveTool().onModifier1({plug::State::Released});
  }
  if (event.key_id == plug::KeyCode::LControl ||
      event.key_id == plug::KeyCode::RControl)
  {
    m_palette.getActiveTool().onModifier2({plug::State::Released});
  }
  if (event.key_id == plug::KeyCode::LAlt ||
      event.key_id == plug::KeyCode::RAlt)
  {
    m_palette.getActiveTool().onModifier3({plug::State::Released});
  }
}

static inline double getFitScale(const Vec2d& original, const Vec2d& target)
{
  const double scale_x = target.x / original.x;
  const double scale_y = target.y / original.y;
  return scale_x < scale_y ? scale_x : scale_y;
}

plug::Transform CanvasView::getCanvasTransform(void) const
{
  const double scale =
      getFitScale(m_canvas.getSize(), m_canvasLayoutBox.getSize());
  const Vec2d offset =
      (m_canvasLayoutBox.getSize() - scale * m_canvas.getSize()) / 2;

  return Transform(getLayoutBox().getPosition() + m_canvasLayoutBox.getPosition() + offset,
                   Vec2d(scale, scale));
}

} // namespace gui
