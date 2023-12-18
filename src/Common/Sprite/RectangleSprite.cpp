#include "Common/Sprite/RectangleSprite.h"

#include <cmath>

#include "Common/Math.h"

static inline double min3(double a, double b, double c)
{
  if (a < b)
  {
    return a < c ? a : c;
  }
  else
  {
    return b < c ? b : c;
  }
}

static void drawRect(plug::Color color, const plug::LayoutBox& layout_box,
                     plug::TransformStack& stack, plug::RenderTarget& target)
{
  const Vec2d size = layout_box.getSize();

  const Vec2d tl(0, 0);
  const Vec2d tr(size.x, 0);
  const Vec2d bl(0, size.y);
  const Vec2d br(size.x, size.y);

  stack.enter(Transform(layout_box.getPosition()));
  plug::VertexArray array(plug::TriangleStrip, 4);
  array[0] = plug::Vertex{
      .position = stack.apply(tl), .tex_coords = Vec2d(), .color = color};
  array[1] = plug::Vertex{
      .position = stack.apply(tr), .tex_coords = Vec2d(), .color = color};
  array[2] = plug::Vertex{
      .position = stack.apply(bl), .tex_coords = Vec2d(), .color = color};
  array[3] = plug::Vertex{
      .position = stack.apply(br), .tex_coords = Vec2d(), .color = color};
  stack.leave();

  target.draw(array);
}

void RectangleSprite::draw(const plug::LayoutBox& layout_box,
                           plug::TransformStack&  stack,
                           plug::RenderTarget&    target) const
{
  const Vec2d  size   = layout_box.getSize();
  const double radius = min3(m_radius, size.x / 2, size.y / 2);

  if (fabs(radius) < 1e-4)
  {
    drawRect(m_color, layout_box, stack, target);
    return;
  }

  const Vec2d tl(radius, radius);
  const Vec2d tr(size.x - radius, radius);
  const Vec2d bl(radius, size.y - radius);
  const Vec2d br(size.x - radius, size.y - radius);

  constexpr size_t point_count = 40;
  constexpr double angle_step  = 2 * M_PI / double(point_count);

  stack.enter(Transform(layout_box.getPosition()));

  plug::VertexArray array(plug::TriangleFan, point_count);
  for (size_t i = 0; i < point_count; ++i)
  {
    const double angle = angle_step * i;
    const Vec2d  offset(radius * cos(angle), radius * sin(angle));
    // clang-format off
    const Vec2d  base = offset.y < 0
                          ? (offset.x < 0 ? tl : tr)
                          : (offset.x < 0 ? bl : br);
    // clang-format on

    array[i] = plug::Vertex{.position   = stack.apply(base + offset),
                            .tex_coords = Vec2d(),
                            .color      = m_color};
  }
  stack.leave();

  target.draw(array);
}
