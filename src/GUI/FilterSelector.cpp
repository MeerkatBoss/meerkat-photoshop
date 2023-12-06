#include "GUI/FilterSelector.h"

#include "GUI/Widget.h"
#include "Impl/LayoutBox/LayoutBox.h"

namespace gui
{

Logger FilterSelector::s_logger = Logger("Filter Selector");

FilterSelector::FilterSelector(FilterPalette&         palette,
                               const plug::LayoutBox& layout_box) :
    Widget(layout_box), m_palette(palette), m_selected(nullptr), m_options()
{
  s_logger.LOG_DEBUG(Content::TEXT,
                     "Created filter selector at %p with filter palette %p",
                     this, &palette);
}

FilterSelector::~FilterSelector(void)
{
  const size_t option_count = m_options.getSize();
  for (size_t i = 0; i < option_count; ++i)
  {
    delete m_options[i];
  }

  s_logger.LOG_DEBUG(Content::TEXT, "Destroyed filter selector at %p", this);
}

void FilterSelector::draw(plug::TransformStack& stack,
                          plug::RenderTarget&   target)
{
  if (m_options.getSize() < m_palette.getFilterCount())
  {
    updateFilterList();
  }
  const plug::Color bg_color(70, 70, 80);
  plug::VertexArray array(plug::TriangleStrip, 4);

  auto [tl, tr, bl, br] = getRect(getLayoutBox());

  array[0] = plug::Vertex{
      .position = stack.apply(tl), .tex_coords = Vec2d(), .color = bg_color};
  array[1] = plug::Vertex{
      .position = stack.apply(tr), .tex_coords = Vec2d(), .color = bg_color};
  array[2] = plug::Vertex{
      .position = stack.apply(bl), .tex_coords = Vec2d(), .color = bg_color};
  array[3] = plug::Vertex{
      .position = stack.apply(br), .tex_coords = Vec2d(), .color = bg_color};
  target.draw(array);

  stack.enter(Transform(getLayoutBox().getPosition()));

  const size_t option_count = m_options.getSize();
  for (size_t i = 0; i < option_count; ++i)
  {
    m_options[i]->draw(stack, target);
  }

  if (m_selected)
  {
    plug::Widget* config = m_selected->getWidget();
    if (config != nullptr)
    {
      config->draw(stack, target);
    }
  }

  stack.leave();
}

void FilterSelector::onEvent(const plug::Event& event, plug::EHC& context)
{
  if (m_selected)
  {
    plug::Widget* config = m_selected->getWidget();
    if (config != nullptr)
    {
      config->onEvent(event, context);
    }
  }

  Widget::onEvent(event, context);
}

void FilterSelector::onMousePressed(const plug::MousePressedEvent& event,
                                    plug::EHC&                     context)
{
  if (context.stopped || context.overlapped)
  {
    return;
  }

  if (covers(context.stack, event.pos))
  {
    if (m_options.getSize() < m_palette.getFilterCount())
    {
      updateFilterList();
    }

    context.overlapped = true;

    context.stack.enter(Transform(getLayoutBox().getPosition()));

    const size_t option_count = m_options.getSize();
    for (size_t i = 0; i < option_count; ++i)
    {
      if (m_options[i]->covers(context.stack, event.pos))
      {
        m_selected = m_options[i]->getFilter();
        plug::Widget* widget = m_selected->getWidget();
        if (widget != nullptr)
        {
          widget->onParentUpdate(getLayoutBox());
        }
        break;
      }
    }

    context.stack.leave();
  }
}

void FilterSelector::updateFilterList(void)
{
  size_t old_count = m_options.getSize();
  size_t new_count = m_palette.getFilterCount();

  for (size_t i = old_count; i < new_count; ++i)
  {
    addFilterOption(&m_palette.getFilter(i));
  }

  if (m_selected != nullptr)
  {
    plug::Widget* widget = m_selected->getWidget();
    if (widget != nullptr)
    {
      widget->onParentUpdate(getLayoutBox());
    }
  }
}

void FilterSelector::onParentUpdate(const plug::LayoutBox& parent_box)
{
  Widget::onParentUpdate(parent_box);

  const size_t option_count = m_options.getSize();
  for (size_t i = 0; i < option_count; ++i)
  {
    m_options[i]->onParentUpdate(getLayoutBox());
  }
}

void FilterSelector::addFilterOption(plug::Filter* filter)
{
  const size_t option_count = m_options.getSize();
  const size_t length_mm    = 5 + 15 * option_count;

  layout::LayoutBox box(
      100_per, layout::Length(10 + length_mm, layout::Unit::Millimeter),
      layout::Align::TopCenter);

  box.setPadding(layout::Length(length_mm, layout::Unit::Millimeter), 0_px,
                 5_mm, 5_mm);

  m_options.pushBack(new FilterOption(filter, box));
  m_options.back()->onParentUpdate(getLayoutBox());
}

} // namespace gui
