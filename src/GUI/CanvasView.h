/**
 * @file CanvasView.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_CANVAS_VIEW_H
#define __GUI_CANVAS_VIEW_H
#include <cstdio>

#include "Canvas/Canvas.h"
#include "Common/GUI/Widget.h"
#include "GUI/Button.h"
#include "GUI/Titlebar.h"
#include "Layout/LayoutBox.h"
#include "Layout/Units.h"
#include "Sprite/RectangleSprite.h"
#include "Tool/ToolPalette.h"

namespace gui
{

class CanvasView : public Widget, private ButtonController
{
public:
  CanvasView(ToolPalette& palette, Canvas& canvas,
             const plug::LayoutBox& layout_box);

  void focus(void) { m_isFocused = true; }
  void unfocus(void) { m_isFocused = false; }

  void stayOpen(void) { m_isClosed = false; }
  bool isClosed(void) const { return m_isClosed; }

  Canvas&       getCanvas(void) { return m_canvas; }
  const Canvas& getCanvas(void) const { return m_canvas; }

  void onEvent(const plug::Event& event, plug::EHC& context) override;

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override;

  virtual void onParentUpdate(const plug::LayoutBox& parent_box) override
  {
    Widget::onParentUpdate(parent_box);
    m_closeButton.onParentUpdate(getLayoutBox());
    m_titlebar.onParentUpdate(getLayoutBox());
    m_canvasLayoutBox.onParentUpdate(getLayoutBox());
  }

  virtual void onClick(size_t) override
  {
    m_isClosed = true;
  }

protected:
  virtual void onMousePressed(const plug::MousePressedEvent& event,
                              plug::EHC&                     context) override;

  virtual void onMouseReleased(const plug::MouseReleasedEvent& event,
                               plug::EHC& context) override;

  virtual void onMouseMove(const plug::MouseMoveEvent& event,
                           plug::EHC&                  context) override;

  virtual void onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                 plug::EHC& context) override;

  virtual void onKeyboardReleased(const plug::KeyboardReleasedEvent& event,
                                  plug::EHC& context) override;

private:
  plug::Transform getCanvasTransform(void) const;

  Titlebar m_titlebar;
  Button   m_closeButton;

  ToolPalette& m_palette;

  layout::LayoutBox m_canvasLayoutBox;
  Canvas&           m_canvas;

  bool  m_isMoving;
  Vec2d m_lastPos;

  bool m_isFocused;

  bool m_isClosed;
};

} // namespace gui

#endif /* CanvasView.h */
