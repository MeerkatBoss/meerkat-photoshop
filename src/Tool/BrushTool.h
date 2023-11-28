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

class BrushTool : public plug::Tool
{
  class BrushData : public plug::PluginData
  {
    virtual const char* getName(void) const override { return "Brush Tool"; }
    virtual const char* getTexturePath(void) const override { return nullptr; }
  };

public:
  BrushTool(double radius) :
      m_colorPalette(nullptr),
      m_activeCanvas(nullptr),
      m_radius(radius),
      m_isDrawing(false),
      m_lastPos()
  {
  }

  virtual ~BrushTool() override{};

  /*****************************************************************************
   *
   * plug::Plugin implementation
   *
   ****************************************************************************/

  virtual Plugin* tryGetInterface(size_t interface_id) override
  {
    if (interface_id == size_t(plug::PluginGuid::Tool))
      return this;
    return nullptr;
  }

  virtual void addReference(void) override
  { /* TODO: replace stub */
  }

  virtual void release(void) override
  { /* TODO: replace stub */
  }

  virtual const plug::PluginData* getPluginData(void) const override
  {
    return &s_pluginData;
  }

  /*****************************************************************************
   *
   * plug::Tool implementation
   *
   ****************************************************************************/

  virtual void setColorPalette(plug::ColorPalette& palette) override
  {
    m_colorPalette = &palette;
  }

  virtual void setActiveCanvas(plug::Canvas& canvas) override
  {
    m_activeCanvas = &canvas;
  }

  virtual void onMove(const Vec2d& position) override;

  virtual void onMainButton(const plug::ControlState& state,
                            const Vec2d&              position) override;

  virtual void onSecondaryButton(const plug::ControlState&,
                                 const Vec2d&) override{};

  virtual void onModifier1(const plug::ControlState&) override {}

  virtual void onModifier2(const plug::ControlState&) override{};

  virtual void onModifier3(const plug::ControlState&) override{};

  virtual void onConfirm(void) override {}

  virtual void onCancel(void) override {}

  virtual plug::Widget* getWidget() override { return nullptr; }

private:
  static const BrushData s_pluginData;

  void drawLine(const Vec2d& start, const Vec2d& end) const;

  const plug::ColorPalette* m_colorPalette;
  plug::Canvas*             m_activeCanvas;

  double m_radius;

  bool  m_isDrawing;
  Vec2d m_lastPos;
};

#endif /* BrushTool.h */
