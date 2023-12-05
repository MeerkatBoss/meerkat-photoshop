#include "GUI/FilterSelector.h"

#include "GUI/Widget.h"
#include "Impl/LayoutBox/LayoutBox.h"

namespace gui
{

Logger FilterSelector::s_logger = Logger("Filter Selector");

FilterSelector::FilterSelector(FilterPalette& palette) :
    Widget(LayoutBox()), m_palette(palette)
{
  s_logger.LOG_DEBUG(Content::TEXT,
                     "Created filter selector at %p with filter palette %p",
                     this, &palette);
}

FilterSelector::~FilterSelector(void)
{
  s_logger.LOG_DEBUG(Content::TEXT, "Created filter selector at %p", this);
}

} // namespace gui
