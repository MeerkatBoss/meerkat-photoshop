/**
 * @file TextField.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-19
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_TEXT_FIELD_H
#define __GUI_TEXT_FIELD_H

#include "GUI/Titlebar.h"
#include "GUI/Widget.h"
#include "Layout/LayoutBox.h"
#include "Sprite/RectangleSprite.h"
#include "Sprite/TextSprite.h"

namespace gui
{

class TextField : public gui::Widget
{
public:
  TextField(const char* header, const char* default_text,
            const plug::LayoutBox& layout_box);

  virtual void draw(plug::TransformStack& stack,
                    plug::RenderTarget&   target) override;

  virtual void onParentUpdate(const plug::LayoutBox& parent_box) override;

  const char* getText(void) const { return m_text; }

protected:
  virtual void onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                                 plug::EHC& context) override;

private:
  static constexpr size_t MAX_TEXT_LEN = 32;

  Titlebar          m_titlebar;
  RectangleSprite   m_background;
  layout::LayoutBox m_fieldLayoutBox;
  RectangleSprite   m_fieldBg;
  TextSprite        m_fieldText;

  size_t m_textLen;
  char m_text[MAX_TEXT_LEN + 1];
};

} // namespace gui

#endif /* TextField.h */
