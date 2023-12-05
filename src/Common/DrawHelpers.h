/**
 * @file DrawHelpers.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __COMMON_DRAW_HELPERS_H
#define __COMMON_DRAW_HELPERS_H

#include "Common/Math.h"
#include "Common/Plug/Graphics.h"

void getLineVertexArray(const Vec2d& start, const Vec2d& end, double radius,
                        const plug::Color& color, plug::VertexArray& array);

class CatmullRomSpline
{
public:
  CatmullRomSpline(const Vec2d& p0, const Vec2d& p1, const Vec2d& p2,
                   const Vec2d& p3) :
      p0(p0), p1(p1), p2(p2), p3(p3)
  {
  }

  Vec2d operator()(double t_value) const;

private:
  Vec2d p0, p1, p2, p3;
};

#endif /* DrawHelpers.h */
