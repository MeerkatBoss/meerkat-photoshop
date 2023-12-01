/**
 * @file EditorView.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-29
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_EDITOR_VIEW_H
#define __GUI_EDITOR_VIEW_H

#include "DynArray.h"
#include "EditorState.h"
#include "GUI/CanvasView.h"
#include "GUI/ToolSelector.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"

namespace gui
{

class EditorView : public Widget
{
public:
  EditorView(EditorState& editor_state, const plug::LayoutBox& layout_box) :
      Widget(layout_box),
      m_editorState(editor_state),
      m_toolSelector(editor_state.getTools()),
      m_views(),
      m_activeView(nullptr)
  {
  }

  virtual ~EditorView(void) override
  {
    const size_t view_count = m_views.getSize();
    for (size_t i = 0; i < view_count; ++i)
    {
      delete m_views[i];
    }
  }

  void addCanvasView(CanvasView* canvas_view);

  void setActiveView(CanvasView* canvas_view);

  void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

  void onEvent(const plug::Event& event, plug::EHC& context) override;

  void onParentUpdate(const plug::LayoutBox& parent_box) override;

  /* TODO: Allow creating and opening canvases with hotkeys and menus */ 

private:
  EditorState& m_editorState;

  ToolSelector          m_toolSelector;
  DynArray<CanvasView*> m_views;
  CanvasView*           m_activeView;
};

} // namespace gui

#endif /* EditorView.h */
