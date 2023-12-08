#include "Common/BaseTool.h"

#include "Common/LogHelpers.h"

size_t BaseTool::s_idCounter = 0;

Logger BaseTool::s_logger = Logger("Tool");

bool BaseTool::s_isLoaded = false;

BaseTool::BaseTool(const char* name, const char* icon_path) :
    m_refCount(1),
    m_toolId(++s_idCounter),
    m_data(name, icon_path),
    m_colors(nullptr),
    m_canvas(nullptr)
{
  s_logger.LOG_DEBUG(Content::TEXT,
                     "Created tool \"%s\" (id %zu) with texture path \"%s\"",
                     name, m_toolId, icon_path);
}

BaseTool::~BaseTool()
{
  s_logger.LOG_DEBUG(Content::TEXT, "Destroyed tool \"%s\" (id %zu)",
                     m_data.getName(), m_toolId);
}

bool BaseTool::loadTool(void)
{
  LOG_ASSERT(s_logger, !s_isLoaded, "Tool '%s' is already loaded",
             m_data.getName());
  if (s_isLoaded)
  {
    return false;
  }

  s_isLoaded = true;
  return true;
}

[[maybe_unused]] static const char* guidToString(plug::PluginGuid guid);

plug::Plugin* BaseTool::tryGetInterface(size_t interface_id)
{
  s_logger.LOG_TRACE(
      Content::TEXT,
      "Requested interface \"%s\" (GUID %zu) from tool \"%s\" (id %zu)",
      guidToString(plug::PluginGuid(interface_id)), interface_id,
      m_data.getName(), m_toolId);

  if (interface_id == size_t(plug::PluginGuid::Plugin))
  {
    s_logger.LOG_TRACE(
        Content::TEXT,
        "Returned interface \"Plugin\" (GUID %zu) from tool \"%s\" (id %zu)",
        interface_id, m_data.getName(), m_toolId);

    addReference();
    return this;
  }
  if (interface_id == size_t(plug::PluginGuid::Tool))
  {
    s_logger.LOG_TRACE(
        Content::TEXT,
        "Returned interface \"Tool\" (GUID %zu) from tool \"%s\" (id %zu)",
        interface_id, m_data.getName(), m_toolId);

    addReference();
    return this;
  }

  return nullptr;
}

void BaseTool::addReference(void)
{
  ++m_refCount;
  s_logger.LOG_DEBUG(Content::TEXT,
                     "Added reference to tool \"%s\" (id %zu), refCount=%zu",
                     m_data.getName(), m_toolId, m_refCount);
}

void BaseTool::release(void)
{
  LOG_ASSERT(
      s_logger, m_refCount > 0,
      "Attempted release with zero reference count for tool \"%s\" (id %zu)",
      m_data.getName(), m_toolId);

  if (m_refCount == 0)
  {
    return;
  }

  --m_refCount;
  s_logger.LOG_DEBUG(Content::TEXT,
                     "Released reference to tool \"%s\" (id %zu), refCount=%zu",
                     m_data.getName(), m_toolId, m_refCount);
  if (m_refCount == 0)
  {
    s_logger.LOG_TRACE(
        Content::TEXT,
        "Reference counter reached zero for tool \"%s\" (id %zu)",
        m_data.getName(), m_toolId);
    delete this;
  }
}

const plug::PluginData* BaseTool::getPluginData(void) const
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Requested plugin data from tool \"%s\" (id %zu)",
                     m_data.getName(), m_toolId);
  return &m_data;
}

static const char* guidToString(plug::PluginGuid guid)
{
  switch (guid)
  {
  case plug::PluginGuid::Plugin:
    return "Plugin";
    break;
  case plug::PluginGuid::Filter:
    return "Filter";
    break;
  case plug::PluginGuid::Tool:
    return "Tool";
    break;
  default:
    return "Unknown";
    break;
  }
}

void BaseTool::setColorPalette(plug::ColorPalette& palette)
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Set color palette %p for tool \"%s\" (id %zu)", &palette,
                     m_data.getName(), m_toolId);
  m_colors = &palette;
}

void BaseTool::setActiveCanvas(plug::Canvas& canvas)
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Set active canvas %p for tool \"%s\" (id %zu)", &canvas,
                     m_data.getName(), m_toolId);
  m_canvas = &canvas;
}

void BaseTool::onMove([[maybe_unused]] const Vec2d& position)
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Cursor moved to (%lg, %lg) for tool \"%s\" (id %zu)",
                     position.x, position.y, m_data.getName(), m_toolId);
}

void BaseTool::onMainButton([[maybe_unused]] const plug::ControlState& state,
                            [[maybe_unused]] const Vec2d&              position)
{
  s_logger.LOG_TRACE(
      Content::TEXT, "Main button %s at (%lg, %lg) for tool \"%s\" (id %zu)",
      state.state == plug::State::Pressed ? "pressed" : "released", position.x,
      position.y, m_data.getName(), m_toolId);
}

void BaseTool::onSecondaryButton(
    [[maybe_unused]] const plug::ControlState& state,
    [[maybe_unused]] const Vec2d&              position)
{
  s_logger.LOG_TRACE(
      Content::TEXT,
      "Secondary button %s at (%lg, %lg) for tool \"%s\" (id %zu)",
      state.state == plug::State::Pressed ? "pressed" : "released", position.x,
      position.y, m_data.getName(), m_toolId);
}

void BaseTool::onModifier1([[maybe_unused]] const plug::ControlState& state)
{
  s_logger.LOG_TRACE(Content::TEXT, "Modifier #1 %s for tool \"%s\" (id %zu)",
                     state.state == plug::State::Pressed ? "pressed"
                                                         : "released",
                     m_data.getName(), m_toolId);
}

void BaseTool::onModifier2([[maybe_unused]] const plug::ControlState& state)
{
  s_logger.LOG_TRACE(Content::TEXT, "Modifier #2 %s for tool \"%s\" (id %zu)",
                     state.state == plug::State::Pressed ? "pressed"
                                                         : "released",
                     m_data.getName(), m_toolId);
}

void BaseTool::onModifier3([[maybe_unused]] const plug::ControlState& state)
{
  s_logger.LOG_TRACE(Content::TEXT, "Modifier #3 %s for tool \"%s\" (id %zu)",
                     state.state == plug::State::Pressed ? "pressed"
                                                         : "released",
                     m_data.getName(), m_toolId);
}

void BaseTool::onConfirm(void)
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Confirmed application of tool \"%s\" (id %zu)",
                     m_data.getName(), m_toolId);
}

void BaseTool::onCancel(void)
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Canceled application of tool \"%s\" (id %zu)",
                     m_data.getName(), m_toolId);
}

bool BaseTool::hasActiveCanvas(void) const
{
  if (m_canvas == nullptr)
  {
    s_logger.LOG_TRACE(Content::TEXT,
                       "No active canvas set for tool \"%s\" (id %zu)",
                       m_data.getName(), m_toolId);
    return false;
  }
  else
  {
    s_logger.LOG_TRACE(Content::TEXT,
                       "Active canvas %p for tool \"%s\" (id %zu)", m_canvas,
                       m_data.getName(), m_toolId);
    return true;
  }
}

plug::Canvas& BaseTool::getActiveCanvas(void)
{
  LOG_ASSERT(s_logger, m_canvas != nullptr,
             "No active canvas set for tool \"%s\" (id %zu)", m_data.getName(),
             m_toolId);

  s_logger.LOG_TRACE(Content::TEXT,
                     "Requested active canvas %p for tool \"%s\" (id %zu)",
                     m_canvas, m_data.getName(), m_toolId);

  return *m_canvas;
}

bool BaseTool::hasColorPalette(void) const
{
  if (m_colors == nullptr)
  {
    s_logger.LOG_TRACE(Content::TEXT,
                       "No color palette set for tool \"%s\" (id %zu)",
                       m_data.getName(), m_toolId);
    return false;
  }
  else
  {
    s_logger.LOG_TRACE(Content::TEXT,
                       "Color palette %p for tool \"%s\" (id %zu)", m_colors,
                       m_data.getName(), m_toolId);
    return true;
  }
}

plug::ColorPalette& BaseTool::getColorPalette(void)
{
  LOG_ASSERT(s_logger, m_colors != nullptr,
             "No color palette set for tool \"%s\" (id %zu)", m_data.getName(),
             m_toolId);

  s_logger.LOG_TRACE(Content::TEXT,
                     "Requested color palette %p for tool \"%s\" (id %zu)",
                     m_colors, m_data.getName(), m_toolId);

  return *m_colors;
}
