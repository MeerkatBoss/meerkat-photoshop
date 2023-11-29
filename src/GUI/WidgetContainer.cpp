#include "GUI/WidgetContainer.h"

namespace gui
{

void WidgetContainer::onEvent(const plug::Event& event,
                              plug::EHC& context)
{
  for (size_t i = 0; i < m_widgets.getSize(); ++i)
  {
    m_widgets[i]->onEvent(event, context);
  }
}

void WidgetContainer::draw(plug::TransformStack&     transform_stack,
                           plug::RenderTarget& draw_target)
{
  if (m_widgets.isEmpty())
  {
    return;
  }

  transform_stack.enter(Transform(getLayoutBox().getPosition()));

  for (size_t i = m_widgets.getSize(); i > 0; --i)
  {
    m_widgets[i - 1]->draw(transform_stack, draw_target);
  }
  transform_stack.leave();
}

void WidgetContainer::onParentUpdate(const plug::LayoutBox& parent_box)
{
  getLayoutBox().onParentUpdate(parent_box);
  size_t widget_count = m_widgets.getSize();
  for (size_t i = 0; i < widget_count; ++i)
  {
    m_widgets[i]->onParentUpdate(getLayoutBox());
  }
}

} // namespace gui
