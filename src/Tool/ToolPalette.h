/**
 * @file ToolPalette.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-28
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_TOOL_PALETTE_H
#define __TOOL_TOOL_PALETTE_H

#include <cassert>

#include "DynArray.h"
#include "Plug/Tool.h"

class ToolPalette
{
public:
  ToolPalette(void) : m_tools(), m_activeTool(nullptr) {}

  ~ToolPalette(void)
  {
    for (size_t i = 0; i < m_tools.getSize(); ++i)
    {
      delete m_tools[i];
    }
  }

  void setColorPalette(plug::ColorPalette& palette)
  {
    for (size_t i = 0; i < m_tools.getSize(); ++i)
    {
      m_tools[i]->setColorPalette(palette);
    }
  }

  void setActiveCanvas(plug::Canvas& canvas)
  {
    for (size_t i = 0; i < m_tools.getSize(); ++i)
    {
      m_tools[i]->setActiveCanvas(canvas);
    }
  }

  void addTool(plug::Tool* tool)
  {
    m_tools.pushBack(tool);
    if (m_activeTool == nullptr)
      m_activeTool = m_tools.back();
  }

  size_t getToolCount(void) const { return m_tools.getSize(); }

  const plug::Tool& getTool(size_t tool_idx) const
  {
    assert(tool_idx < getToolCount());
    return *m_tools[tool_idx];
  }

  void setActiveTool(size_t tool_idx)
  {
    assert(tool_idx < getToolCount());
    m_activeTool->onCancel();
    m_activeTool = m_tools[tool_idx];
  }

  plug::Tool& getActiveTool(void) {
    assert(m_activeTool != nullptr);
    return *m_activeTool;
  }

private:
  DynArray<plug::Tool*> m_tools;
  plug::Tool*           m_activeTool;
};

#endif /* ToolPalette.h */
