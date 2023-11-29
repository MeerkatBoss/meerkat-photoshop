/**
 * @file ToolSelector.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-29
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_TOOL_SELECTOR_H
#define __GUI_TOOL_SELECTOR_H

#include "GUI/Widget.h"
#include "Impl/LayoutBox/LayoutBox.h"
#include "Math.h"
#include "Tool/ToolPalette.h"

namespace gui
{

class ToolSelector : public Widget
{
public:
  ToolSelector(ToolPalette& palette) :
      Widget(LayoutBox()), // TODO: Replace stub layout
      m_palette(palette)
  {
  }

  virtual void draw(TransformStack&, plug::RenderTarget&) override
  {
    /* TODO: Draw tool selection buttons */
  }

protected:
  virtual void onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                 plug::EHC& context) override;

private:
  ToolPalette& m_palette;
};

} // namespace gui

#endif /* ToolSelector.h */
