#include "Tool/BrushTool.h"

const BrushTool::BrushData BrushTool::s_pluginData = BrushData();

void BrushTool::onMove(const Vec2d& position)
{
  if (m_isDrawing)
  {
    drawLine(m_lastPos, position);
    m_lastPos = position;
  }
}

void BrushTool::onMainButton(const plug::ControlState& state,
                             const Vec2d&              position)
{
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

void BrushTool::drawLine(const Vec2d& start, const Vec2d& end) const
{
  if (m_activeCanvas == nullptr)
  {
    return;
  }
  assert(m_colorPalette != nullptr);

  const size_t      point_count = 20;
  const plug::Color brush_color = m_colorPalette->getFGColor();

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

  m_activeCanvas->draw(array);
}

