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
#include "GUI/Widget.h"
#include "Tool/ToolPalette.h"

namespace gui
{

class CanvasView : public Widget
{
public:
  CanvasView(ToolPalette& palette, Canvas& canvas,
             const plug::LayoutBox& layout_box) :
      Widget(layout_box), m_palette(palette), m_canvas(canvas)
  {
  }

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override;

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

  ToolPalette& m_palette;
  Canvas&      m_canvas;
};

} // namespace gui

#endif /* CanvasView.h */
