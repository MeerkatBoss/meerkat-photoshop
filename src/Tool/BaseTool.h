/**
 * @file BaseTool.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __TOOL_BASE_TOOL_H
#define __TOOL_BASE_TOOL_H

#include "LogHelpers.h"
#include "Math.h"
#include "Plug/Tool.h"

class BaseTool : public plug::Tool
{
  class ToolData : public plug::PluginData
  {
  public:
    ToolData(const char* name, const char* icon_path) :
        m_name(name), m_iconPath(icon_path)
    {
    }

    virtual const char* getName(void) const override { return m_name; }

    virtual const char* getTexturePath(void) const override
    {
      return m_iconPath;
    }

  private:
    const char* m_name;
    const char* m_iconPath;
  };

public:
  BaseTool(const char* name, const char* icon_path);

  virtual ~BaseTool() override;

  /*****************************************************************************
   *
   * plug::Plugin implementation
   *
   ****************************************************************************/

  virtual plug::Plugin* tryGetInterface(size_t interface_id) override;

  virtual void addReference(void) override;

  virtual void release(void) override;

  virtual const plug::PluginData* getPluginData(void) const override;

  /*****************************************************************************
   *
   * plug::Tool implementation
   *
   ****************************************************************************/

  virtual void setColorPalette(plug::ColorPalette& palette) override;

  virtual void setActiveCanvas(plug::Canvas& canvas) override;

  virtual void onMove(const Vec2d& position) override;

  virtual void onMainButton(const plug::ControlState& state,
                            const Vec2d&              position) override;

  virtual void onSecondaryButton(const plug::ControlState& state,
                                 const Vec2d&              position) override;

  virtual void onModifier1(const plug::ControlState& state) override;

  virtual void onModifier2(const plug::ControlState& state) override;

  virtual void onModifier3(const plug::ControlState& state) override;

  virtual void onConfirm(void) override;

  virtual void onCancel(void) override;

protected:
  bool          hasActiveCanvas(void) const;
  plug::Canvas& getActiveCanvas(void);

  bool                hasColorPalette(void) const;
  plug::ColorPalette& getColorPalette(void);

private:
  static size_t s_idCounter;

  static Logger s_logger;

  size_t m_refCount;

  const size_t   m_toolId;
  const ToolData m_data;

  plug::ColorPalette* m_colors;
  plug::Canvas*       m_canvas;
};

#endif /* BaseTool.h */
