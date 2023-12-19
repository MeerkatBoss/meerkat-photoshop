#include "GUI/EditorView.h"

#include "Common/GUI/Widget.h"
#include "DynArray.h"
#include "GUI/CanvasView.h"
#include "Layout/LayoutBox.h"
#include "Math.h"

namespace gui
{

EditorView::EditorView(EditorState&           editor_state,
                       const plug::LayoutBox& layout_box) :
    Widget(layout_box),
    m_editorState(editor_state),
    m_filterSelector(editor_state.getFilters(),
                     layout::LayoutBox(5_cm, 50_per, layout::Align::TopRight)),
    m_toolSelector(editor_state.getTools(),
                   layout::LayoutBox(5_cm, 50_per, layout::Align::BottomRight)),
    m_colorSelector(
        editor_state.getColors(),
        layout::LayoutBox(5_cm, 100_per, layout::Align::CenterLeft)),
    m_views(),
    m_activeView(nullptr)
{
  m_colorSelector.addColor(plug::Color(0, 0, 0));
  m_colorSelector.addColor(plug::Color(255, 0, 0));
  m_colorSelector.addColor(plug::Color(0, 255, 0));
  m_colorSelector.addColor(plug::Color(0, 0, 255));
  m_colorSelector.addColor(plug::Color(255, 255, 0));
  m_colorSelector.addColor(plug::Color(255, 0, 255));
  m_colorSelector.addColor(plug::Color(0, 255, 255));
  m_colorSelector.addColor(plug::Color(255, 255, 255));
}

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
  m_activeViewIdx = m_views.getSize() - 1;
}

void EditorView::setActiveView(CanvasView* canvas_view)
{
  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    if (m_views[i] == canvas_view)
    {
      if (m_activeView != nullptr)
      {
        m_activeView->unfocus();
      }

      m_editorState.setActiveCanvas(&canvas_view->getCanvas());
      m_activeView = canvas_view;
      m_activeView->focus();
      m_activeViewIdx = i;
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

  m_colorSelector.draw(stack, target);

  m_toolSelector.draw(stack, target);

  m_filterSelector.draw(stack, target);

  stack.leave();
}

void EditorView::onEvent(const plug::Event& event, plug::EHC& context)
{
  Widget::onEvent(event, context);

  context.stack.enter(Transform(getLayoutBox().getPosition()));

  m_filterSelector.onEvent(event, context);

  m_toolSelector.onEvent(event, context);

  m_colorSelector.onEvent(event, context);

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
  m_filterSelector.onParentUpdate(getLayoutBox());
  m_toolSelector.onParentUpdate(getLayoutBox());
  m_colorSelector.onParentUpdate(getLayoutBox());

  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    m_views[i]->onParentUpdate(getLayoutBox());
  }
}

template<typename T>
static void pruneArray(DynArray<T*>& array)
{
  while (!array.isEmpty() && array.back() == nullptr)
  {
    array.popBack();
  }

  size_t it = 0;
  while (it < array.getSize())
  {
    if (array[it] == nullptr)
    {
      array[it] = array.back();
      array.popBack();
    }
    ++it;
  }
}

void EditorView::onTick(const plug::TickEvent&, plug::EHC&)
{
  bool isActiveViewClosed = false;
  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    if (m_views[i]->isClosed())
    {
      if (m_views[i] == m_activeView)
      {
        isActiveViewClosed = true;
      }
      m_editorState.saveCanvas(&m_views[i]->getCanvas());

      delete m_views[i];
      m_views[i] = nullptr;
    }
  }

  pruneArray(m_views);
  if (isActiveViewClosed)
  {
    if (m_views.getSize() > 0)
    {
      m_activeView = m_views[0];
    }
    m_activeView = nullptr;
  }
}

void EditorView::onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                   plug::EHC&                        context)
{
  /* TODO: Allow creating and opening canvases with hotkeys and menus */

  if (context.stopped)
  {
    return;
  }

  if (event.ctrl && event.key_id == plug::KeyCode::F)
  {
    plug::Canvas* active_canvas = m_editorState.getActiveCanvas();
    if (active_canvas != nullptr && m_editorState.getFilters().hasLastFilter())
    {
      m_editorState.getFilters().getLastFilter().applyFilter(*active_canvas);
    }
  }

  static const char* names[] = {
      "Unknown.png",   "Unknown_1.png", "Unknown_2.png", "Unknown_3.png",
      "Unknown_4.png", "Unknown_5.png", "Unknown_6.png", "Unknown_7.png",
      "Unknown_8.png", "Unknown_9.png"};

  if (event.ctrl && event.key_id == plug::KeyCode::N)
  {
    m_editorState.newCanvas(names[m_editorState.getCanvasCount()], 500, 500);
    addCanvasView(new CanvasView(m_editorState.getTools(),
                                 *m_editorState.getAllCanvases().back(),
                                 layout::LayoutBox(15_cm, 15_cm)));
  }

  if (event.key_id == plug::KeyCode::Tab)
  {
    if (m_views.getSize() == 0)
    {
      return;
    }

    size_t new_idx = (m_activeViewIdx + 1) % m_views.getSize();
    setActiveView(m_views[new_idx]);
  }

  if (event.key_id == plug::KeyCode::Enter)
  {
    plug::Filter* selected      = m_filterSelector.getSelectedFilter();
    plug::Canvas* active_canvas = m_editorState.getActiveCanvas();

    if (selected != nullptr && active_canvas != nullptr)
    {
      selected->applyFilter(*active_canvas);
      m_filterSelector.deselect();
      size_t filter_count = m_editorState.getFilters().getFilterCount();
      for (size_t i = 0; i < filter_count; ++i)
      {
        if (selected == &m_editorState.getFilters().getFilter(i))
        {
          m_editorState.getFilters().setLastFilter(i);
          break;
        }
      }
    }
  }

  if (event.key_id == plug::KeyCode::Escape)
  {
    m_filterSelector.deselect();
  }
}

} // namespace gui
