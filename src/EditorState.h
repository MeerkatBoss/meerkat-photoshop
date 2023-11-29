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
#include "DynArray.h"
#include "Tool/ColorPalette.h"
#include "Tool/ToolPalette.h"

class EditorState
{
public:
  EditorState(void) :
      m_activeCanvas(nullptr), m_canvases(), m_tools(), m_colors()
  {
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

  void saveActiveCanvas(void)
  {
    if (m_activeCanvas != nullptr)
    {
      saveCanvas(m_activeCanvas);
    }
  }

  Canvas*       getActiveCanvas(void) { return m_activeCanvas; }
  const Canvas* getActiveCanvas(void) const { return m_activeCanvas; }

  void setActiveCanvas(Canvas* canvas);

  ToolPalette& getTools(void) { return m_tools; }

  ColorPalette& getColors(void) { return m_colors; }

  void newCanvas(const char* name, size_t width, size_t height)
  {
    m_canvases.pushBack(new Canvas(name, width, height));
  }

  void openCanvas(const char* filename);

private:
  void saveCanvas(const Canvas* canvas);

  Canvas*           m_activeCanvas;
  DynArray<Canvas*> m_canvases;
  ToolPalette       m_tools;
  ColorPalette      m_colors;
};

#endif /* EditorState.h */
