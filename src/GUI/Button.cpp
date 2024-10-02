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
  if (event.button_id == plug::MouseButton::Left)
  {
    m_controller.onRelease(m_buttonId);
    m_pressed = false;
  }
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

void Button::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  m_sprite->draw(getLayoutBox(), stack, target);
}

} // namespace gui
