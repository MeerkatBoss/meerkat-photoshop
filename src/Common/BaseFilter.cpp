#include "Common/BaseFilter.h"

size_t BaseFilter::s_idCounter = 0;

Logger BaseFilter::s_logger = Logger("Filter");

BaseFilter::BaseFilter(const char* name, const char* icon_path) :
    m_filterId(++s_idCounter), m_data(name, icon_path), m_refCount(1)
{
  s_logger.LOG_DEBUG(Content::TEXT,
                     "Created filter \"%s\" (id %zu) with texture path \"%s\"",
                     name, m_filterId, icon_path);
}

BaseFilter::~BaseFilter()
{
  s_logger.LOG_DEBUG(Content::TEXT, "Destroyed filter \"%s\" (id %zu)",
                     m_data.getName(), m_filterId);
}

[[maybe_unused]] static const char* guidToString(plug::PluginGuid guid);

plug::Plugin* BaseFilter::tryGetInterface(size_t interface_id)
{
  s_logger.LOG_TRACE(
      Content::TEXT,
      "Requested interface \"%s\" (GUID %zu) from filter \"%s\" (id %zu)",
      guidToString(plug::PluginGuid(interface_id)), interface_id,
      m_data.getName(), m_filterId);

  if (interface_id == size_t(plug::PluginGuid::Plugin))
  {
    s_logger.LOG_TRACE(
        Content::TEXT,
        "Returned interface \"Plugin\" (GUID %zu) from filter \"%s\" (id %zu)",
        interface_id, m_data.getName(), m_filterId);

    addReference();
    return this;
  }
  if (interface_id == size_t(plug::PluginGuid::Filter))
  {
    s_logger.LOG_TRACE(
        Content::TEXT,
        "Returned interface \"Filter\" (GUID %zu) from filter \"%s\" (id %zu)",
        interface_id, m_data.getName(), m_filterId);

    addReference();
    return this;
  }

  return nullptr;
}

void BaseFilter::addReference(void)
{
  ++m_refCount;
  s_logger.LOG_DEBUG(Content::TEXT,
                     "Added reference to filter \"%s\" (id %zu), refCount=%zu",
                     m_data.getName(), m_filterId, m_refCount);
}

void BaseFilter::release(void)
{
  LOG_ASSERT(
      s_logger, m_refCount > 0,
      "Attempted release with zero reference count for filter \"%s\" (id %zu)",
      m_data.getName(), m_filterId);

  if (m_refCount == 0)
  {
    return;
  }

  --m_refCount;
  s_logger.LOG_DEBUG(
      Content::TEXT,
      "Released reference to filter \"%s\" (id %zu), refCount=%zu",
      m_data.getName(), m_filterId, m_refCount); if (m_refCount == 0)
  {
    s_logger.LOG_TRACE(
        Content::TEXT,
        "Reference counter reached zero for filter \"%s\" (id %zu)",
        m_data.getName(), m_filterId);
    delete this;
  }
}

const plug::PluginData* BaseFilter::getPluginData(void) const
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Requested plugin data from filter \"%s\" (id %zu)",
                     m_data.getName(), m_filterId);
  return &m_data;
}

static const char* guidToString(plug::PluginGuid guid)
{
  switch (guid)
  {
  case plug::PluginGuid::Plugin:
    return "Plugin";
    break;
  case plug::PluginGuid::Tool:
    return "Tool";
    break;
  case plug::PluginGuid::Filter:
    return "Filter";
    break;
  default:
    return "Unknown";
    break;
  }
}

void BaseFilter::applyFilter([[maybe_unused]] plug::Canvas& canvas) const
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Applying filter '%s' (id %zu) to canvas %p",
                     m_data.getName(), m_filterId, &canvas);
}
