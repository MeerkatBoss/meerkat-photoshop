/**
 * @file Button.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-22
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_BUTTON_H
#define __GUI_BUTTON_H

#include "Common/GUI/Widget.h"
#include "Sprite/Sprite.h"

namespace gui
{

class ButtonController
{
public:
  virtual void onClick([[maybe_unused]] size_t button_id) { return; }
  virtual void onHold([[maybe_unused]] size_t button_id,
                      [[maybe_unused]] double delta_time)
  {
    return;
  }
  virtual void onRelease([[maybe_unused]] size_t button_id) { return; }

  virtual ~ButtonController() = default;
};

class Button : public Widget
{
public:
  Button(const Sprite& sprite, ButtonController& controller, const plug::LayoutBox& layout_box) :
      Widget(layout_box),
      m_buttonId(s_idCounter++),
      m_controller(controller),
      m_sprite(sprite.clone()),
      m_pressed(false),
      m_hovered(false)
  {
  }

  virtual ~Button(void) override
  {
    delete m_sprite;
  }

  void setSprite(const Sprite& sprite)
  {
    delete m_sprite;
    m_sprite = sprite.clone();
  }

  const Sprite& getSprite(void) const { return *m_sprite; }

  virtual void draw(plug::TransformStack& transform_stack,
                    plug::RenderTarget&   target) override;

protected:
  virtual void onMousePressed(const plug::MousePressedEvent& event,
                              plug::EHC&                     context) override;

  virtual void onMouseReleased(const plug::MouseReleasedEvent& event,
                               plug::EHC& context) override;

  virtual void onMouseMove(const plug::MouseMoveEvent& event,
                           plug::EHC&                  context) override;

  virtual void onTick(const plug::TickEvent& event,
                      plug::EHC&             context) override;

private:
  static size_t s_idCounter;

  size_t            m_buttonId;
  ButtonController& m_controller;
  Sprite*           m_sprite;

  bool              m_pressed;
  bool              m_hovered;
};

} // namespace gui

#endif /* Button.h */
