/**
 * @file FilterSelector.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_FILTER_SELECTOR_H
#define __GUI_FILTER_SELECTOR_H

#include "Common/GUI/Widget.h"
#include "Common/Layout/LayoutBox.h"
#include "DynArray.h"
#include "Filter/FilterPalette.h"
#include "GUI/Titlebar.h"
#include "LogHelpers.h"
#include "Math.h"

namespace gui
{

class FilterSelector : public Widget
{
  class FilterOption : public Widget
  {
  public:
    FilterOption(plug::Filter* filter, const plug::LayoutBox& layout_box) :
        Widget(layout_box),
        m_filter(filter),
        m_text(filter->getPluginData()->getName(),
               layout::LayoutBox(100_per, 100_per))
    {
    }

    virtual ~FilterOption(void) override = default;

    plug::Filter* getFilter(void) const { return m_filter; }

    virtual void draw(plug::TransformStack& stack,
                      plug::RenderTarget&   target) override
    {
      stack.enter(Transform(getLayoutBox().getPosition()));
      m_text.draw(stack, target);
      stack.leave();
    }

    void onParentUpdate(const plug::LayoutBox& parent_box) override
    {
      Widget::onParentUpdate(parent_box);
      m_text.onParentUpdate(getLayoutBox());
    }

  private:
    plug::Filter* m_filter;
    Titlebar      m_text;
  };

public:
  FilterSelector(FilterPalette& palette, const plug::LayoutBox& layout_box);

  virtual ~FilterSelector(void) override;

  plug::Filter* getSelectedFilter(void) { return m_selected; }

  void deselect(void) { m_selected = nullptr; }

  virtual void onEvent(const plug::Event& event, plug::EHC& context) override;

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override;

  virtual void onParentUpdate(const plug::LayoutBox& parent_box) override;

protected:
  virtual void onMousePressed(const plug::MousePressedEvent& event,
                              plug::EHC&                     context) override;

private:
  static Logger s_logger;

  void updateFilterList(void);

  void addFilterOption(plug::Filter* filter);

  FilterPalette& m_palette;
  plug::Filter*  m_selected;

  DynArray<FilterOption*> m_options;
};

} // namespace gui

#endif /* FilterSelector.h */
