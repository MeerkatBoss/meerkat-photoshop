#include "Canvas/Canvas.h"

#include <SFML/Config.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Canvas/SelectionMask.h"
#include "Impl/RenderTarget/SfmlRenderTarget/RenderTarget.h"

void Canvas::draw(const plug::VertexArray& vertex_array)
{
  if (m_cacheState == InvalidRenderTexture)
  {
    copyRawToRender();
  }

  SfmlRenderTarget target(m_renderTexture);
  target.draw(vertex_array);
  m_cacheState = InvalidRawTexture;
}

void Canvas::draw(const plug::VertexArray& vertex_array,
                  const plug::Texture&     texture)
{
  if (m_cacheState == InvalidRenderTexture)
  {
    copyRawToRender();
  }

  SfmlRenderTarget target(m_renderTexture);
  target.draw(vertex_array, texture);
  m_cacheState = InvalidRawTexture;
}

void Canvas::setSize(const plug::Vec2d& size)
{
  if (m_cacheState == InvalidRenderTexture)
  {
    copyRawToRender();
  }
  const size_t new_width(size.x);
  const size_t new_height(size.y);

  /* Update render texture */
  sf::Texture texture = m_renderTexture.getTexture();
  sf::Sprite  sprite(texture);
  m_renderTexture.create(new_width, new_height);
  m_renderTexture.draw(sprite);

  /* Resize raw texture, but discard data */
  delete m_rawTexture;
  m_rawTexture = new plug::Texture(new_width, new_height);

  /* Raw texture is left invalid */
  m_cacheState = InvalidRawTexture;

  /* Update selection mask */
  SelectionMask new_mask(new_width, new_height);
  for (size_t x = 0; x < new_width && x < m_width; ++x)
  {
    for (size_t y = 0; y < new_height && y < m_height; ++y)
    {
      new_mask.setPixel(x, y, m_mask.getPixel(x, y));
    }
  }
  m_mask = new_mask;

  /* Update size */
  m_width  = new_width;
  m_height = new_height;
}

void Canvas::copyRenderToRaw(void) const
{
  if (m_cacheState != InvalidRawTexture)
  {
    return;
  }

  const_cast<Canvas*>(this)->m_renderTexture.display();
  sf::Image image = m_renderTexture.getTexture().copyToImage();
  for (size_t x = 0; x < m_width; ++x)
  {
    for (size_t y = 0; y < m_height; ++y)
    {
      m_rawTexture->setPixel(x, y, getPlugColor(image.getPixel(x, y)));
    }
  }
  m_cacheState = Valid;
}

void Canvas::copyRawToRender(void) const
{
  if (m_cacheState != InvalidRenderTexture)
  {
    return;
  }

  sf::Texture texture;
  texture.create(m_width, m_height);
  texture.update((const sf::Uint8*)m_rawTexture->data);
  sf::Sprite sprite(texture);

  const_cast<Canvas*>(this)->m_renderTexture.draw(sprite);
}
