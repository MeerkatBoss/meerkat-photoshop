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
#include "Sprite/CompositeSprite.h"

namespace gui
{

class Titlebar : public Widget
{
public:
  Titlebar(const char* name, const plug::LayoutBox& layout_box);

  virtual ~Titlebar(void) override { delete[] m_name; }

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override;

  void setName(const char* name);

  const char* getName(void) const { return m_name; }

  virtual void onParentUpdate(const plug::LayoutBox& parent_box) override
  {
    Widget::onParentUpdate(parent_box);
    setName(m_name);
  }

private:
  char* m_name;

  CompositeSprite m_sprite;
};

} // namespace gui

#endif /* Titlebar.h */
