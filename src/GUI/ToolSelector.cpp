#include "GUI/ToolSelector.h"

namespace gui
{

void ToolSelector::onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                     plug::EHC&                        context)
{
  if (context.stopped)
  {
    return;
  }

  if (event.alt)
  {
#define TRY_SELECT_TOOL(num)                     \
  if (event.key_id == plug::KeyCode::Num##num && \
      m_palette.getToolCount() > num - 1)        \
  {                                              \
    m_palette.setActiveTool(num - 1);            \
    context.stopped = true;                      \
    return;                                      \
  }

    TRY_SELECT_TOOL(1)
    TRY_SELECT_TOOL(2)
    TRY_SELECT_TOOL(3)
    TRY_SELECT_TOOL(4)
    TRY_SELECT_TOOL(5)
    TRY_SELECT_TOOL(6)
    TRY_SELECT_TOOL(7)
    TRY_SELECT_TOOL(8)
    TRY_SELECT_TOOL(9)

#undef TRY_SELECT_TOOL
  }
}

} // namespace gui
