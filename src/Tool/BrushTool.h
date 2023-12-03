/**
 * @file BrushTool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-28
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_BRUSH_TOOL_H
#define __TOOL_BRUSH_TOOL_H

#include "Math.h"
#include "Plug/Tool.h"
#include "Tool/BaseTool.h"

class BrushTool : public BaseTool
{
public:
  BrushTool(double radius) :
      BaseTool("Brush Tool", nullptr),
      m_radius(radius),
      m_isDrawing(false),
      m_lastPos()
  {
  }

  virtual ~BrushTool() override{}

  virtual void onMove(const Vec2d& position) override;

  virtual void onMainButton(const plug::ControlState& state,
                            const Vec2d&              position) override;

  virtual void onSecondaryButton(const plug::ControlState&,
                                 const Vec2d&) override{};

  virtual plug::Widget* getWidget() override { return nullptr; }

private:
  void drawLine(const Vec2d& start, const Vec2d& end);

  double m_radius;

  bool  m_isDrawing;
  Vec2d m_lastPos;
};

#endif /* BrushTool.h */
