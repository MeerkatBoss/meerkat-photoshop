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
#include "Plug/Widget.h"

class Photoshop
{
public:
  Photoshop(size_t width, size_t height);
  ~Photoshop(void);

  void initGUI(void);

  void runMainLoop(void);
private:
  Canvas m_canvas;

  plug::Widget* m_widgetTree;
  sf::RenderWindow m_window;
};

#endif /* Photoshop.h */
