/**
 * @file ColorPalette.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-28
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_COLOR_PALETTE_H
#define __TOOL_COLOR_PALETTE_H

#include "Common/Plug/Tool/ColorPalette.h"

class ColorPalette : public plug::ColorPalette
{
public:
  explicit ColorPalette(plug::Color foreground = plug::Color(0, 0, 0),
                        plug::Color background = plug::Color(255, 255, 255)) :
    m_foregroundColor(foreground), m_backgroundColor(background)
  {
  }

  virtual plug::Color getFGColor(void) const override
  {
    return m_foregroundColor;
  }

  virtual plug::Color getBGColor(void) const override
  {
    return m_backgroundColor;
  }

  virtual void setFGColor(plug::Color new_fg_color) override
  {
    m_foregroundColor = new_fg_color;
  }

  virtual void setBGColor(plug::Color new_bg_color) override
  {
    m_backgroundColor = new_bg_color;
  }

  virtual ~ColorPalette() override {}

private:
  plug::Color m_foregroundColor, m_backgroundColor;
};

#endif /* ColorPalette.h */
