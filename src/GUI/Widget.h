/**
 * @file Widget.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @author Solodovnikov Ivan (solodovnikov.ia@phystech.edu)
 * @brief Usable widget template
 * @version 0.1
 * @date 2023-11-17
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GUI_WIDGET_H
#define __GUI_WIDGET_H

#include "Plug/Widget.h"

namespace gui
{

struct Rect
{
  plug::Vec2d tl, tr, bl, br;
};

Rect getRect(const plug::LayoutBox& box);

/**
 * @brief Usable widget
 *
 */
class Widget : public plug::Widget
{
public:
  explicit Widget(const plug::LayoutBox& box) :
    m_box(box.clone())
  {
  }

  Widget(const Widget& widget) :
    m_box(widget.m_box->clone())
  {
  }

  Widget& operator=(const Widget& widget);
  virtual ~Widget() override
  {
    delete m_box;
  }

  void draw(plug::TransformStack& stack, plug::RenderTarget& target) override;

  void onEvent(const plug::Event& event, plug::EHC& context) override;

  void onParentUpdate(const plug::LayoutBox& parent_box) override
  {
    m_box->onParentUpdate(parent_box);
  }

  plug::LayoutBox&       getLayoutBox() override { return *m_box; }
  const plug::LayoutBox& getLayoutBox() const override { return *m_box; }
  void                   setLayoutBox(const plug::LayoutBox& box) override
  {
    if (&box == m_box) { return; }

    delete m_box;
    m_box = box.clone();
  }

protected:
  bool covers(plug::TransformStack& stack,
              const plug::Vec2d&    position) const override;

private:
  plug::LayoutBox* m_box;
};

} // namespace gui

#endif /* Widget.h */
