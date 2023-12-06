/**
 * @file PosterizeFilter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-06
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EXPORT_POSTERIZE_FILTER_H
#define __EXPORT_POSTERIZE_FILTER_H

#include "Common/BaseFilter.h"

extern "C" plug::Plugin* loadPlugin(void);

class PosterizeFilter : public BaseFilter
{
public:
  PosterizeFilter() :
    BaseFilter("Posterize Filter", nullptr),
    m_colorLevelsCount(5)
  {
  }

  virtual void applyFilter(plug::Canvas &canvas) const override;

  virtual plug::Widget *getWidget(void) override { return nullptr; }
private:

  size_t m_colorLevelsCount;
};


#endif /* PosterizeFilter.h */
