/**
 * @file Canvas.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-24
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __CANVAS_CANVAS_H
#define __CANVAS_CANVAS_H

#include <SFML/Graphics/RenderTexture.hpp>
#include <cassert>

#include "Canvas/SelectionMask.h"
#include "Common/Plug/Canvas/Canvas.h"

class Canvas : public plug::Canvas
{
public:
  Canvas(const char* name, size_t width, size_t height) :
      m_cacheState(Valid),
      m_name(name),
      m_width(width),
      m_height(height),
      m_mask(width, height),
      m_rawTexture(new plug::Texture(width, height))
  {
    m_renderTexture.create(width, height);
  }

  virtual ~Canvas(void) override
  {
    delete m_rawTexture;
  }

  virtual void draw(const plug::VertexArray& vertex_array) override;

  virtual void draw(const plug::VertexArray& vertex_array,
                    const plug::Texture&     texture) override;

  virtual void setSize(const plug::Vec2d& size) override;

  virtual plug::Vec2d getSize(void) const override
  {
    return plug::Vec2d{m_width, m_height};
  }

  virtual plug::SelectionMask& getSelectionMask(void) override
  {
    return m_mask;
  }

  virtual plug::Color getPixel(size_t x, size_t y) const override
  {
    assert(x < m_width);
    assert(y < m_height);
    if (m_cacheState == InvalidRawTexture)
    {
      copyRenderToRaw();
    }
    return m_rawTexture->data[y * m_width + x];
  }

  virtual void setPixel(size_t x, size_t y, const plug::Color& color) override
  {
    assert(x < m_width);
    assert(y < m_height);
    if (m_cacheState == InvalidRawTexture)
    {
      copyRenderToRaw();
    }
    m_rawTexture->data[y * m_width + x] = color;
    m_cacheState                      = InvalidRenderTexture;
  }

  virtual const plug::Texture& getTexture(void) const override
  {
    if (m_cacheState == InvalidRawTexture)
    {
      copyRenderToRaw();
    }
    return *m_rawTexture;
  }

  void setName(const char* new_name) { m_name = new_name; }

  const char* getName(void) const { return m_name; }

private:
  void copyRenderToRaw(void) const;
  void copyRawToRender(void) const;

  enum CacheState
  {
    InvalidRenderTexture,
    InvalidRawTexture,
    Valid
  };

  mutable CacheState m_cacheState;

  const char* m_name;
  size_t m_width;
  size_t m_height;

  SelectionMask     m_mask;
  sf::RenderTexture m_renderTexture;
  plug::Texture*    m_rawTexture;
};

#endif /* Canvas.h */
