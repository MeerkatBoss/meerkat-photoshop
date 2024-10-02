/**
 * @file EditorState.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-29
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EDITOR_STATE_H
#define __EDITOR_STATE_H

#include "Canvas/Canvas.h"
#include "Common/DynArray.h"
#include "Filter/FilterPalette.h"
#include "LogHelpers.h"
#include "Tool/ColorPalette.h"
#include "Tool/ToolPalette.h"

class EditorState
{
public:
  EditorState(void) :
      m_activeCanvas(nullptr), m_canvases(), m_tools(), m_colors()
  {
    m_tools.setColorPalette(m_colors);
  }
  EditorState(const EditorState& other) = delete;
  EditorState& operator=(const EditorState& other) = delete;

  ~EditorState(void)
  {
    for (size_t i = 0; i < m_canvases.getSize(); ++i)
    {
      saveCanvas(m_canvases[i]);
      delete m_canvases[i];
    }
  }

  void loadPlugins(const char* plugdir);

  void saveCanvas(const Canvas* canvas);

  void saveActiveCanvas(void)
  {
    if (m_activeCanvas != nullptr)
    {
      saveCanvas(m_activeCanvas);
    }
  }

  Canvas*       getActiveCanvas(void) { return m_activeCanvas; }
  const Canvas* getActiveCanvas(void) const { return m_activeCanvas; }

  size_t getCanvasCount(void) const { return m_canvases.getSize(); }

  const DynArray<Canvas*>& getAllCanvases(void) const { return m_canvases; }

  void setActiveCanvas(Canvas* canvas);

  FilterPalette& getFilters(void) { return m_filters; }

  ToolPalette& getTools(void) { return m_tools; }

  ColorPalette& getColors(void) { return m_colors; }

  void newCanvas(const char* name, size_t width, size_t height)
  {
    Canvas* canvas = new Canvas(name, width, height);
    m_canvases.pushBack(canvas);
    setActiveCanvas(canvas);
  }

  void openCanvas(const char* filename);

private:
  static Logger s_logger;

  plug::Plugin* getPluginFromDir(const char* dirpath);

  Canvas*           m_activeCanvas;
  DynArray<Canvas*> m_canvases;
  FilterPalette     m_filters;
  ToolPalette       m_tools;
  ColorPalette      m_colors;
};

#endif /* EditorState.h */
