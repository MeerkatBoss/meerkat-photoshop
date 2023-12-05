#include "Common/DrawHelpers.h"

#include "LogHelpers.h"

static Logger s_logger = Logger("Draw Helpers");

void getLineVertexArray(const Vec2d& start, const Vec2d& end, double radius,
                        const plug::Color& color, plug::VertexArray& array)
{
  LOG_ASSERT(s_logger, array.getSize() > 1, "Vertex array too small for line");

  if (array.getSize() <= 1)
  {
    return;
  }

  if (array.getSize() == 2)
  {
    array.setPrimitive(plug::Lines);
    array[0] = plug::Vertex{
        .position = start, .tex_coords = plug::Vec2d{}, .color = color};
    array[1] = plug::Vertex{
        .position = end, .tex_coords = plug::Vec2d{}, .color = color};
    return;
  }

  const size_t      point_count = array.getSize() - 1;
  const plug::Vec2d direction   = end - start;

  for (size_t i = 0; i < point_count; ++i)
  {
    const double      angle = i * (2 * M_PI / point_count);
    const plug::Vec2d offset(cos(angle), sin(angle));

    if (dot(offset, direction) < 0)
    {
      array[i] = plug::Vertex{.position   = start + radius * offset,
                              .tex_coords = plug::Vec2d(),
                              .color      = color};
    }
    else
    {
      array[i] = plug::Vertex{.position   = end + radius * offset,
                              .tex_coords = plug::Vec2d(),
                              .color      = color};
    }
  }
  array[point_count] = array[0];

  array.setPrimitive(plug::TriangleFan);
}

__attribute__((always_inline))
static double getCatmullRomKnot(Vec2d cur_point, Vec2d next_point,
                                double prev_knot)
{
  const double distance = (next_point - cur_point).length();

  return sqrt(distance) + prev_knot;
}

__attribute__((always_inline))
static Vec2d interpolate(Vec2d start, Vec2d end, double start_knot,
                         double end_knot, double t_value)
{
  const double knot_diff = end_knot - start_knot;

  // clang-format off
  return start * ((t_value - start_knot) / knot_diff) +
         end   * ((end_knot - t_value)   / knot_diff);
  // clang-format on
}

Vec2d CatmullRomSpline::operator()(double t_value) const
{
  // Knot values
  const double t0 = 0;
  const double t1 = getCatmullRomKnot(p0, p1, t0);
  const double t2 = getCatmullRomKnot(p1, p2, t1);
  const double t3 = getCatmullRomKnot(p2, p3, t2);

  const double t_norm = t_value * t1 + (1 - t_value) * t2;

  // First level of interpolation
  const Vec2d lerp01 = interpolate(p0, p1, t0, t1, t_norm);
  const Vec2d lerp12 = interpolate(p1, p2, t1, t2, t_norm);
  const Vec2d lerp23 = interpolate(p2, p3, t2, t3, t_norm);

  // Second level of interpolation
  const Vec2d lerp012 = interpolate(lerp01, lerp12, t0, t2, t_norm);
  const Vec2d lerp123 = interpolate(lerp12, lerp23, t1, t2, t_norm);

  // Third level of interpolation
  const Vec2d res = interpolate(lerp012, lerp123, t1, t2, t_norm);

  return res;
}
