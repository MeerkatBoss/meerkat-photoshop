#include "EditorState.h"

#include "Canvas/Canvas.h"
#include "Impl/Util/Sfml.h"

void EditorState::setActiveCanvas(Canvas* canvas)
{
  for (size_t i = 0; i < m_canvases.getSize(); ++i)
  {
    if (canvas == m_canvases[i])
    {
      m_activeCanvas = canvas;
      return;
    }
  }

  assert(false && "Failed to set active canvas: canvas is not registered");
}

void EditorState::openCanvas(const char* filename)
{
  sf::Image image;

  bool loaded = image.loadFromFile(filename);
  assert(loaded);

  size_t width  = image.getSize().x;
  size_t height = image.getSize().y;

  Canvas* opened = new Canvas(filename, width, height);
  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      opened->setPixel(x, y, getPlugColor(image.getPixel(x, y)));
    }
  }

  m_canvases.pushBack(opened);
  m_activeCanvas = opened;
}

void EditorState::saveCanvas(const Canvas* canvas)
{
  size_t width = canvas->getSize().x;
  size_t height = canvas->getSize().y;
  sf::Image image;
  image.create(width, height);

  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      image.setPixel(x, y, getSFMLColor(canvas->getPixel(x, y)));
    }
  }

  image.saveToFile(canvas->getName());
}
