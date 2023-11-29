/**
 * @file Photoshop.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-21
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __PHOTOSHOP_H
#define __PHOTOSHOP_H

#include <SFML/Graphics/RenderWindow.hpp>
#include "Canvas/Canvas.h"
#include "Tool/ColorPalette.h"
#include "Tool/ToolPalette.h"
#include "Plug/Widget.h"

class Photoshop
{
public:
  Photoshop(size_t width, size_t height);
  ~Photoshop(void);

  void initGUI(void);

  void runMainLoop(void);
private:
  /* TODO: Extract to EditorState */
  Canvas m_canvas;
  ToolPalette m_tools;
  ColorPalette m_colors;

  plug::Widget* m_widgetTree;
  sf::RenderWindow m_window;
};

#endif /* Photoshop.h */
