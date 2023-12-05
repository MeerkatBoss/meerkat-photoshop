#include "GUI/CanvasView.h"

#include "Common/GUI/Widget.h"

namespace gui
{

void CanvasView::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  using plug::Vec2d;
  using plug::Vertex;

  const plug::Color bg_color(40, 40, 45);

  // const auto [tl, tr, bl, br] = getRect(getLayoutBox());
  const Vec2d canvas_size     = m_canvas.getSize();

  const Vec2d tex_tl(0, 0);
  const Vec2d tex_tr(canvas_size.x, 0);
  const Vec2d tex_bl(0, canvas_size.y);
  const Vec2d tex_br(canvas_size.x, canvas_size.y);

  plug::VertexArray array(plug::TriangleStrip, 4);
  stack.enter(getCanvasTransform());

  array[0] = Vertex{.position = stack.apply(tex_tl), .tex_coords = tex_tl};
  array[1] = Vertex{.position = stack.apply(tex_tr), .tex_coords = tex_tr};
  array[2] = Vertex{.position = stack.apply(tex_bl), .tex_coords = tex_bl};
  array[3] = Vertex{.position = stack.apply(tex_br), .tex_coords = tex_br};
  target.draw(array, m_canvas.getTexture());

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

plug::Transform CanvasView::getCanvasTransform(void) const
{
  return plug::Transform(getLayoutBox().getPosition(),
                         getLayoutBox().getSize() / m_canvas.getSize());
}

} // namespace gui
