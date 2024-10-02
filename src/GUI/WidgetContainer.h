/**
 * @file WidgetContainer.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-29
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_WIDGET_CONTAINER_H
#define __GUI_WIDGET_CONTAINER_H

#include "DynArray.h"
#include "Common/GUI/Widget.h"
#include "Math.h"

namespace gui
{

class WidgetContainer : public Widget
{
public:
  WidgetContainer(const plug::LayoutBox& layout_box) :
      Widget(layout_box), m_widgets()
  {
  }

  virtual ~WidgetContainer() override
  {
    for (size_t i = 0; i < m_widgets.getSize(); ++i)
    {
      delete m_widgets[i];
    }
  }

  virtual void onEvent(const plug::Event& event, plug::EHC& context) override;

  virtual void draw(plug::TransformStack& transform_stack,
                    plug::RenderTarget&   draw_target) override;

  virtual void onParentUpdate(const plug::LayoutBox& parent_box) override;

  DynArray<Widget*>&       getWidgets() { return m_widgets; }
  const DynArray<Widget*>& getWidgets() const
  {
    return const_cast<WidgetContainer*>(this)->getWidgets();
  }

  void addWidget(Widget* widget)
  {
    m_widgets.pushBack(widget);
    widget->onParentUpdate(getLayoutBox());
  }

private:
  DynArray<Widget*> m_widgets;
};

} // namespace gui

#endif /* WidgetContainer.h */
