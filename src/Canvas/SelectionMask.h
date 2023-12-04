/**
 * @file SelectionMask.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __CANVAS_SELECTION_MASK_H
#define __CANVAS_SELECTION_MASK_H

#include <cassert>

#include "Common/Plug/Canvas/SelectionMask.h"

class SelectionMask : public plug::SelectionMask
{
public:
  SelectionMask(size_t width, size_t height) :
    m_width(width), m_height(height), m_mask(new bool[width * height])
  {
    for (size_t i = 0; i < width * height; ++i)
    {
      m_mask[i] = true;
    }
  }

  virtual ~SelectionMask(void) override
  {
    delete[] m_mask;
  }

  virtual size_t getWidth(void) const override
  {
    return m_width;
  }

  virtual size_t getHeight(void) const override
  {
    return m_height;
  }

  virtual bool getPixel(size_t x, size_t y) const override
  {
    assert(x < getWidth());
    assert(y < getHeight());
    return m_mask[y * m_width + x];
  }

  virtual void setPixel(size_t x, size_t y, bool value) override
  {
    assert(x < getWidth());
    assert(y < getHeight());
    m_mask[y * m_width + x] = value;
  }

  virtual void fill(bool value) override
  {
    for (size_t i = 0; i < m_width * m_height; ++i)
    {
      m_mask[i] = value;
    }
  }

  virtual void invert(void) override
  {
    for (size_t i = 0; i < m_width * m_height; ++i)
    {
      m_mask[i] = !m_mask[i];
    }
  }

private:
  size_t m_width;
  size_t m_height;

  bool* m_mask;
};

#endif /* SelectionMask.h */
