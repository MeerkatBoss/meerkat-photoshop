#include "Canvas/Canvas.h"

#include <SFML/Config.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Canvas/SelectionMask.h"
#include "Impl/RenderTarget/SfmlRenderTarget/RenderTarget.h"

Logger Canvas::s_logger = Logger("Canvas");

Canvas::Canvas(const char* name, size_t width, size_t height) :
    m_cacheState(Valid),
    m_name(name),
    m_width(width),
    m_height(height),
    m_mask(width, height),
    m_rawTexture(new plug::Texture(width, height))
{
  m_mask.fill(true);
  m_renderTexture.create(width, height);
  m_renderTexture.clear(sf::Color::White);
  s_logger.LOG_DEBUG(Content::TEXT, "Created canvas '%s' %zux%zu at %p", name,
                     width, height, this);
}

Canvas::~Canvas(void)
{
  delete m_rawTexture;
  s_logger.LOG_DEBUG(Content::TEXT, "Destroyed canvas '%s' %zux%zu at %p",
                     m_name, m_width, m_height, this);
}

void Canvas::draw(const plug::VertexArray& vertex_array)
{
  s_logger.LOG_TRACE(Content::TEXT,
                     "Drawing plain vertex array on canvas '%s' at %p", m_name,
                     this);

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
  s_logger.LOG_TRACE(Content::TEXT,
                     "Drawing textured vertex array on canvas '%s' at %p",
                     m_name, this);

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
  s_logger.LOG_DEBUG(
      Content::TEXT, "Resized canvas '%s' at %p from (%zu, %zu) to (%zu, %zu)",
      m_name, this, m_width, m_height, size_t(size.x), size_t(size.y));

  const size_t new_width(size.x);
  const size_t new_height(size.y);

  /* Update render texture */
  sf::Texture texture = m_renderTexture.getTexture();
  sf::Sprite  sprite(texture);
  m_renderTexture.create(new_width, new_height);
  m_renderTexture.clear(sf::Color::White);
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

  s_logger.LOG_TRACE(Content::TEXT, "Updating raw cache in canvas '%s' at %p",
                     m_name, this);

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
  s_logger.LOG_TRACE(Content::TEXT,
                     "Updating render cache in canvas '%s' at %p", m_name,
                     this);

  sf::Texture texture;
  texture.create(m_width, m_height);
  texture.update((const sf::Uint8*)m_rawTexture->data);
  sf::Sprite sprite(texture);

  const_cast<Canvas*>(this)->m_renderTexture.draw(sprite);
}
