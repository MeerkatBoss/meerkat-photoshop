#include "Filter/FilterPalette.h"

#include "LogHelpers.h"

Logger FilterPalette::s_logger = Logger("Filter Palette");

FilterPalette::FilterPalette(void) : m_filters(), m_lastFilter(nullptr)
{
  s_logger.LOG_DEBUG(Content::TEXT, "Created FilterPalette at %p", this);
}

FilterPalette::~FilterPalette(void)
{
  const size_t filter_count = m_filters.getSize();
  for (size_t i = 0; i < filter_count; ++i)
  {
    s_logger.LOG_TRACE(Content::TEXT, "Releasing filter '%s'",
                       m_filters[i]->getPluginData()->getName());
    m_filters[i]->release();
  }
  s_logger.LOG_DEBUG(Content::TEXT, "Destroyed FilterPalette at %p", this);
}

void FilterPalette::addFilter(plug::Filter* filter)
{
  m_filters.pushBack(filter);

  s_logger.LOG_DEBUG(Content::TEXT, "Added filter '%s' to palette at %p",
                     filter->getPluginData()->getName(), this);
}

size_t FilterPalette::getFilterCount(void) const
{
  s_logger.LOG_TRACE(Content::TEXT, "Filter palette %p has %zu filters", this,
                     m_filters.getSize());

  return m_filters.getSize();
}

const plug::Filter& FilterPalette::getFilter(size_t filter_idx) const
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Requested filter %zu/%zu from filter palette %p",
                     filter_idx, getFilterCount(), this);

  LOG_ASSERT(s_logger, filter_idx < getFilterCount(),
             "Index out of bounds (%zu/%zu)", filter_idx, getFilterCount());

  return *m_filters[filter_idx];
}

void FilterPalette::setLastFilter(size_t filter_idx)
{
  LOG_ASSERT(s_logger, filter_idx < getFilterCount(),
             "Index out of bounds (%zu/%zu)", filter_idx, getFilterCount());

  m_lastFilter = m_filters[filter_idx];

  s_logger.LOG_DEBUG(Content::TEXT,
                     "Set last filter '%s' for filter palette %p",
                     m_lastFilter->getPluginData()->getName(), this);
}

bool FilterPalette::hasLastFilter(void) const
{
  if (m_lastFilter == nullptr)
  {
    s_logger.LOG_TRACE(Content::TEXT, "Filter palette %p has no last filter",
                       this);
    return false;
  }
  else
  {
    s_logger.LOG_TRACE(Content::TEXT, "Filter palette %p has last filter '%s'",
                       this, m_lastFilter->getPluginData()->getName());
    return true;
  }
}

const plug::Filter& FilterPalette::getLastFilter(void) const
{
  LOG_ASSERT(s_logger, m_lastFilter != nullptr,
             "Filter palette %p has no last filter", this);

  s_logger.LOG_TRACE(Content::TEXT, "Filter palette %p has last filter '%s'",
                     this, m_lastFilter->getPluginData()->getName());

  return *m_lastFilter;
}
