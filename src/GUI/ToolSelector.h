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

#include "Common/GUI/Widget.h"
#include "Common/Layout/LayoutBox.h"
#include "GUI/Titlebar.h"
#include "Layout/Units.h"
#include "Math.h"
#include "Tool/ToolPalette.h"

namespace gui
{

class ToolSelector : public Widget
{
public:
  ToolSelector(ToolPalette& palette, const plug::LayoutBox& layout_box) :
      Widget(layout_box),
      m_palette(palette),
      m_toolTitle("",
                  layout::LayoutBox(100_per, 1_cm, layout::Align::BottomCenter))
  {
    m_toolTitle.onParentUpdate(getLayoutBox());
  }

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override
  {
    /* TODO: Draw tool selection buttons */
    m_toolTitle.setName(m_palette.getActiveTool().getPluginData()->getName());
    stack.enter(Transform(getLayoutBox().getPosition()));
    m_toolTitle.draw(stack, target);
    stack.leave();
  }
  
  virtual void onParentUpdate(const plug::LayoutBox& parent_box) override
  {
    Widget::onParentUpdate(parent_box);
    m_toolTitle.onParentUpdate(getLayoutBox());
  }

protected:
  virtual void onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                 plug::EHC& context) override;

private:
  ToolPalette& m_palette;
  Titlebar     m_toolTitle;
};

} // namespace gui

#endif /* ToolSelector.h */
