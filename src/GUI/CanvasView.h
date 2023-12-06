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

#include "Canvas/Canvas.h"
#include "Common/GUI/Widget.h"
#include "GUI/Titlebar.h"
#include "Layout/LayoutBox.h"
#include "Tool/ToolPalette.h"

namespace gui
{

class CanvasView : public Widget
{
public:
  CanvasView(ToolPalette& palette, Canvas& canvas,
             const plug::LayoutBox& layout_box) :
      Widget(layout_box),
      m_titlebar(canvas.getName(), layout::LayoutBox(100_per, 1_cm)),
      m_palette(palette),
      m_canvas(canvas),
      m_isMoving(false),
      m_isFocused(false)
  {
  }

  void focus(void) { m_isFocused = true; }
  void unfocus(void) { m_isFocused = false; }

  Canvas&       getCanvas(void) { return m_canvas; }
  const Canvas& getCanvas(void) const { return m_canvas; }

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override;

  void onParentUpdate(const plug::LayoutBox& parent_box) override
  {
    Widget::onParentUpdate(parent_box);
    m_titlebar.onParentUpdate(getLayoutBox());
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

  ToolPalette& m_palette;
  Canvas&      m_canvas;
  bool         m_isMoving;
  Vec2d        m_lastPos;

  bool m_isFocused;
};

} // namespace gui

#endif /* CanvasView.h */
