/**
 * @file BaseFilter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __COMMON_BASE_FILTER_H
#define __COMMON_BASE_FILTER_H

#include "Common/Plug/Filter.h"
#include "LogHelpers.h"

class BaseFilter : public plug::Filter
{
  class FilterData : public plug::PluginData
  {
  public:
    FilterData(const char* name, const char* icon_path) :
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
  BaseFilter(const char* name, const char* icon_path);

  virtual ~BaseFilter() override;

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
   * plug::Filter implementation
   *
   ****************************************************************************/

  virtual void applyFilter(plug::Canvas &canvas) const override;

private:
  static size_t s_idCounter;

  static Logger s_logger;

  const size_t     m_filterId;
  const FilterData m_data;

  size_t m_refCount;
};

#endif /* BaseFilter.h */
