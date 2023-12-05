/**
 * @file FilterSelector.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __GUI_FILTER_SELECTOR_H
#define __GUI_FILTER_SELECTOR_H

#include "Common/GUI/Widget.h"
#include "Filter/FilterPalette.h"
#include "LogHelpers.h"
#include "Math.h"

namespace gui
{

class FilterSelector : public Widget
{
public:
  FilterSelector(FilterPalette& palette);

  virtual ~FilterSelector(void) override;

  virtual void draw(plug::TransformStack& stack, plug::RenderTarget& target) override
  {
    /* TODO: Draw filter selection menu */
    /* TODO: Draw filter configuration widget */
  }

private:
  static Logger s_logger;

  FilterPalette& m_palette;
};

} // namespace gui

#endif /* FilterSelector.h */
