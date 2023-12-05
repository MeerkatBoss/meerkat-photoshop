#include "Export/BrushTool.h"

#include <cassert>
#include <cstdio>

#include "Common/mklog/writers/TextLogWriter.h"

extern "C" plug::Plugin* loadPlugin(void)
{
#ifndef NLOGS
  auto& log_writer = mklog::LogManager::addWriter<mklog::TextLogWriter>()
    .setFile("Plugins/BrushTool/_log.txt");
  if (!log_writer.valid())
  {
    log_writer.setFile("_meerkat_brush_tool_log.txt");
  }

  if (log_writer.valid())
  {
    mklog::LogManager::initLogs();
  }
#endif // NLOGS

  return new BrushTool(10);
}

void BrushTool::onMove(const Vec2d& position)
{
  BaseTool::onMove(position); 
  if (m_isDrawing)
  {
    drawLine(m_lastPos, position);
    m_lastPos = position;
  }
}

void BrushTool::onMainButton(const plug::ControlState& state,
                             const Vec2d&              position)
{
  BaseTool::onMainButton(state, position);

  drawLine(position, position);

  if (state.state == plug::State::Pressed)
  {
    m_isDrawing = true;
    m_lastPos   = position;
  }
  else
  {
    m_isDrawing = false;
  }
}

void BrushTool::drawLine(const Vec2d& start, const Vec2d& end)
{
  if (!hasActiveCanvas())
  {
    return;
  }
  assert(hasColorPalette());

  const size_t      point_count = 20;
  const plug::Color brush_color = getColorPalette().getFGColor();

  const plug::Vec2d direction = end - start;
  plug::VertexArray array(plug::TriangleFan, point_count + 1);

  for (size_t i = 0; i < point_count; ++i)
  {
    const double      angle = i * (2 * M_PI / point_count);
    const plug::Vec2d offset(cos(angle), sin(angle));

    if (dot(offset, direction) < 0)
    {
      array[i] = plug::Vertex{.position   = start + m_radius * offset,
                              .tex_coords = plug::Vec2d(),
                              .color      = brush_color};
    }
    else
    {
      array[i] = plug::Vertex{.position   = end + m_radius * offset,
                              .tex_coords = plug::Vec2d(),
                              .color      = brush_color};
    }
  }
  array[point_count] = array[0];

  getActiveCanvas().draw(array);
}
