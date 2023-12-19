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

#include "Common/GUI/Widget.h"
#include "DynArray.h"
#include "EditorState.h"
#include "GUI/CanvasView.h"
#include "GUI/ColorSelector.h"
#include "GUI/FilterSelector.h"
#include "GUI/ToolSelector.h"

namespace gui
{

class EditorView : public Widget
{
public:
  EditorView(EditorState& editor_state, const plug::LayoutBox& layout_box);

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

protected:
  virtual void onTick(const plug::TickEvent& event,
                      plug::EHC&             context) override;

  virtual void onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                 plug::EHC& context) override;

private:
  EditorState& m_editorState;

  FilterSelector        m_filterSelector;
  ToolSelector          m_toolSelector;
  ColorSelector         m_colorSelector;
  DynArray<CanvasView*> m_views;
  CanvasView*           m_activeView;
  size_t                m_activeViewIdx;
};

} // namespace gui

#endif /* EditorView.h */
