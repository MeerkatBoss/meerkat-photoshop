#include "Photoshop.h"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "Impl/LayoutBox/LayoutBox.h"
#include "GUI/Widget.h"

Photoshop::Photoshop(size_t width, size_t height) :
  m_widgetTree(nullptr),
  m_window(sf::VideoMode(width, height),
           "Photoshop",
           sf::Style::Titlebar)
{
}

Photoshop::~Photoshop(void)
{
  if (m_widgetTree != nullptr)
  {
    delete m_widgetTree;
  }
  if (m_window.isOpen())
  {
    m_window.close();
  }
}

void Photoshop::initGUI(void)
{
  m_widgetTree = new gui::Widget(LayoutBox(20_cm, 20_cm));
}
