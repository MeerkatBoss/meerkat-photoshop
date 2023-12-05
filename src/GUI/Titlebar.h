/**
 * @file Titlebar.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_TITLEBAR_H
#define __GUI_TITLEBAR_H

#include <SFML/Graphics/RenderTexture.hpp>
#include "Common/GUI/Widget.h"

namespace gui
{

class Titlebar : public Widget
{
public:
  Titlebar(const char* name, const plug::LayoutBox& layout_box) :
    Widget(layout_box),
    m_name(name)
  {
    m_renderTexture.create(200, 1500);
  }

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override;

  void setName(const char* name) { m_name = name; }

private:
  const char* m_name;
  sf::RenderTexture m_renderTexture;
};

} // namespace gui

#endif /* Titlebar.h */
