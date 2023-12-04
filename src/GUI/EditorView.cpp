#include "GUI/EditorView.h"

#include "GUI/CanvasView.h"
#include "Common/GUI/Widget.h"
#include "Math.h"

namespace gui
{

void EditorView::addCanvasView(CanvasView* canvas_view)
{
  m_editorState.setActiveCanvas(&canvas_view->getCanvas());

  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    assert(canvas_view != m_views[i]);
  }

  canvas_view->onParentUpdate(getLayoutBox());
  m_views.pushBack(canvas_view);

  if (m_activeView != nullptr)
  {
    m_activeView->unfocus();
  }
  m_activeView = canvas_view;
  m_activeView->focus();
}

void EditorView::setActiveView(CanvasView* canvas_view)
{
  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    if (m_views[i] == canvas_view)
    {
      m_editorState.setActiveCanvas(&canvas_view->getCanvas());
      m_activeView = canvas_view;
      return;
    }
  }
}

void EditorView::draw(plug::TransformStack& stack, plug::RenderTarget& target)
{
  stack.enter(Transform(getLayoutBox().getPosition()));

  const size_t view_count = m_views.getSize();
  for (size_t i = view_count; i > 0; --i)
  {
    if (m_views[i - 1] != m_activeView)
    {
      m_views[i - 1]->draw(stack, target);
    }
  }

  if (m_activeView != nullptr)
  {
    m_activeView->draw(stack, target);
  }

  m_toolSelector.draw(stack, target);

  stack.leave();
}

void EditorView::onEvent(const plug::Event& event, plug::EHC& context)
{
  Widget::onEvent(event, context);

  context.stack.enter(Transform(getLayoutBox().getPosition()));

  m_toolSelector.onEvent(event, context);

  if (m_activeView != nullptr)
  {
    m_activeView->onEvent(event, context);
  }

  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    if (m_views[i] != m_activeView)
    {
      m_views[i]->onEvent(event, context);
    }
  }

  context.stack.leave();
}

void EditorView::onParentUpdate(const plug::LayoutBox& parent_box)
{
  getLayoutBox().onParentUpdate(parent_box);
  m_toolSelector.onParentUpdate(getLayoutBox());

  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    m_views[i]->onParentUpdate(getLayoutBox());
  }
}
} // namespace gui
