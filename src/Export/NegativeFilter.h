/**
 * @file NegativeFilter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-04
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EXPORT_NEGATIVE_FILTER_H
#define __EXPORT_NEGATIVE_FILTER_H

#include "Common/BaseFilter.h"

extern "C" plug::Plugin* loadPlugin(void);

class NegativeFilter : public BaseFilter
{
public:
  NegativeFilter() :
    BaseFilter("Negative", nullptr)
  {
  }

  virtual void applyFilter(plug::Canvas &canvas) const override;

  virtual plug::Widget *getWidget(void) override { return nullptr; }
};

#endif /* NegativeFilter.h */
