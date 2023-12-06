/**
 * @file ColorSelector.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_COLOR_SELECTOR_H
#define __GUI_COLOR_SELECTOR_H

#include "Common/GUI/Widget.h"
#include "DynArray.h"
#include "Layout/Units.h"
#include "Tool/ColorPalette.h"
#include "Common/Layout/LayoutBox.h"

namespace gui
{

class ColorSelector : public Widget
{
  class ColorRect : public Widget
  {
  public:
    ColorRect(plug::Color color, const plug::LayoutBox& layout_box) :
        Widget(layout_box), m_color(color)
    {
    }

    void setColor(plug::Color color) { m_color = color; }

    plug::Color getColor(void) const { return m_color; }

    virtual void draw(plug::TransformStack& transform_stack,
                      plug::RenderTarget&   target) override;

  private:
    plug::Color m_color;
  };

public:
  ColorSelector(ColorPalette& palette, const plug::LayoutBox& layout_box) :
      Widget(layout_box), m_palette(palette),
      m_mainRect(palette.getFGColor(),
          layout::LayoutBox())
  {
    layout::LayoutBox box(100_per, 10_per, layout::Align::TopCenter);
    box.setPadding(5_mm);
    m_mainRect.setLayoutBox(box);
  }

  virtual ~ColorSelector(void) override;

  virtual void draw(plug::TransformStack& transform_stack,
                    plug::RenderTarget&   target) override;

  void onParentUpdate(const plug::LayoutBox& parent_box) override;

  void addColor(plug::Color color);

protected:
  virtual void onMousePressed(const plug::MousePressedEvent& event,
                              plug::EHC&                     context) override;

private:
  ColorPalette& m_palette;
  DynArray<ColorRect*> m_selectors;
  ColorRect m_mainRect;
};

} // namespace gui

#endif /* ColorSelector.h */
