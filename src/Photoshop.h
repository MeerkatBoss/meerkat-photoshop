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
#include "EditorState.h"
#include "LogHelpers.h"
#include "Common/Plug/Widget.h"

class Photoshop
{
public:
  Photoshop(size_t width, size_t height);
  ~Photoshop(void);

  void initGUI(void);

  void runMainLoop(void);

private:
  static Logger s_logger;

  EditorState      m_editorState;
  plug::Widget*    m_widgetTree;
  sf::RenderWindow m_window;
};

#endif /* Photoshop.h */
