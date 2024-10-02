/**
 * @file FilterPalette.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __FILTER_FILTER_PALETTE_H
#define __FILTER_FILTER_PALETTE_H

#include "Common/Plug/Filter.h"
#include "DynArray.h"
#include "LogHelpers.h"

class FilterPalette
{
public:
  FilterPalette(void);

  ~FilterPalette(void);

  void addFilter(plug::Filter* filter);

  size_t getFilterCount(void) const;

  plug::Filter& getFilter(size_t filter_idx) const;

  void setLastFilter(size_t filter_idx);

  bool hasLastFilter(void) const;
  
  plug::Filter& getLastFilter(void) const;

private:
  static Logger s_logger;

  DynArray<plug::Filter*> m_filters;
  plug::Filter*           m_lastFilter;
};

#endif /* FilterPalette.h */
