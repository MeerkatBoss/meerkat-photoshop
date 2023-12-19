#include "GUI/EditorView.h"

#include "Common/GUI/Widget.h"
#include "DynArray.h"
#include "GUI/CanvasView.h"
#include "GUI/TextField.h"
#include "Layout/LayoutBox.h"
#include "Math.h"

namespace gui
{

EditorView::EditorView(EditorState&           editor_state,
                       const plug::LayoutBox& layout_box) :
    Widget(layout_box),
    m_editorState(editor_state),
    m_textField(nullptr),
    m_filterSelector(editor_state.getFilters(),
                     layout::LayoutBox(5_cm, 50_per, layout::Align::TopRight)),
    m_toolSelector(editor_state.getTools(),
                   layout::LayoutBox(5_cm, 50_per, layout::Align::BottomRight)),
    m_colorSelector(
        editor_state.getColors(),
        layout::LayoutBox(5_cm, 100_per, layout::Align::CenterLeft)),
    m_activeView(nullptr),
    m_activeViewIdx(0),
    m_views(),
    m_pendingClose(nullptr),
    m_hasPendingOpen(false)
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

  if (m_textField != nullptr)
  {
    m_textField->draw(stack, target);
  }

  stack.leave();
}

void EditorView::onEvent(const plug::Event& event, plug::EHC& context)
{
  Widget::onEvent(event, context);

  context.stack.enter(Transform(getLayoutBox().getPosition()));

  if (m_textField != nullptr)
  {
    m_textField->onEvent(event, context);
    context.stack.leave();
    return;
  }

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
  if (m_textField != nullptr)
  {
    m_textField->onParentUpdate(getLayoutBox());
  }
  m_filterSelector.onParentUpdate(getLayoutBox());
  m_toolSelector.onParentUpdate(getLayoutBox());
  m_colorSelector.onParentUpdate(getLayoutBox());

  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    m_views[i]->onParentUpdate(getLayoutBox());
  }
}

template <typename T>
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

void EditorView::closePending(void)
{
  if (m_pendingClose == nullptr)
  {
    return;
  }

  bool isActiveViewClosed = (m_pendingClose == m_activeView);

  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    if (m_views[i] == m_pendingClose)
    {
      delete m_views[i];
      m_views[i] = nullptr;
      break;
    }
  }
  pruneArray(m_views);

  if (isActiveViewClosed)
  {
    m_activeView = m_views.getSize() > 0 ? m_views[0] : nullptr;
  }
}

void EditorView::onTick(const plug::TickEvent&, plug::EHC&)
{
  const size_t view_count = m_views.getSize();
  for (size_t i = 0; i < view_count; ++i)
  {
    if (m_views[i]->isClosed())
    {
      if (m_pendingClose == nullptr)
      {
        m_pendingClose = m_views[i];
        m_textField    = new TextField(
            "Enter new file name", m_pendingClose->getCanvas().getName(),
            layout::LayoutBox(20_cm, 6_cm, layout::Align::Center));
        m_textField->onParentUpdate(getLayoutBox());
      }
      m_views[i]->stayOpen();
    }
  }
}

void EditorView::onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                   plug::EHC&                        context)
{
  if (context.stopped)
  {
    return;
  }

  if (m_textField != nullptr)
  {
    if (event.key_id == plug::KeyCode::Enter)
    {
      context.stopped = true;
      if (m_pendingClose != nullptr)
      {
        m_pendingClose->getCanvas().setName(m_textField->getText());
        m_editorState.saveCanvas(&m_pendingClose->getCanvas());
        closePending();
      }
      else if (m_hasPendingOpen)
      {
        m_editorState.openCanvas(m_textField->getText());
        addCanvasView(new CanvasView(m_editorState.getTools(),
                                     *m_editorState.getAllCanvases().back(),
                                     layout::LayoutBox(15_cm, 15_cm)));
        m_hasPendingOpen = false;
      }
      else
      {
        m_activeView->getCanvas().setName(m_textField->getText());
      }
      delete m_textField;
      m_textField = nullptr;
    }
    else if (event.key_id == plug::KeyCode::Escape)
    {
      context.stopped = true;
      m_pendingClose  = nullptr;
      delete m_textField;
      m_textField = nullptr;
    }
    return;
  }

  if (event.ctrl && event.key_id == plug::KeyCode::F)
  {
    context.stopped             = true;
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

  if (event.ctrl && event.key_id == plug::KeyCode::O)
  {
    context.stopped = true;
    m_textField    = new TextField(
        "Enter name of file to open", "",
        layout::LayoutBox(20_cm, 6_cm, layout::Align::Center));
    m_textField->onParentUpdate(getLayoutBox());
    m_hasPendingOpen = true;
  }

  if (event.ctrl && event.key_id == plug::KeyCode::N)
  {
    context.stopped = true;
    m_editorState.newCanvas(names[m_editorState.getCanvasCount()], 500, 500);
    addCanvasView(new CanvasView(m_editorState.getTools(),
                                 *m_editorState.getAllCanvases().back(),
                                 layout::LayoutBox(15_cm, 15_cm)));
  }

  if (event.ctrl && event.key_id == plug::KeyCode::S)
  {
    if (m_activeView != nullptr)
    {
      context.stopped = true;
      m_textField    = new TextField(
          "Enter new file name", m_activeView->getCanvas().getName(),
          layout::LayoutBox(20_cm, 6_cm, layout::Align::Center));
      m_textField->onParentUpdate(getLayoutBox());
    }
  }

  if (event.key_id == plug::KeyCode::Tab)
  {
    context.stopped = true;
    if (m_views.getSize() == 0)
    {
      return;
    }

    size_t new_idx = (m_activeViewIdx + 1) % m_views.getSize();
    setActiveView(m_views[new_idx]);
  }

  if (event.key_id == plug::KeyCode::Enter)
  {
    context.stopped             = true;
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
    context.stopped = true;
    m_filterSelector.deselect();
  }
}

} // namespace gui
