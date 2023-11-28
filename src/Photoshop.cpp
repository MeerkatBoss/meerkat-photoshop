#include "Photoshop.h"

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "GUI/CanvasView.h"
#include "Impl/EventManager/SfmlEventManager.h"
#include "GUI/Button.h"
#include "GUI/Widget.h"
#include "Impl/LayoutBox/LayoutBox.h"
#include "Impl/RenderTarget/SfmlRenderTarget/RenderTarget.h"
#include "Impl/TransformStack.h"

class DebugController : public gui::ButtonController
{
public:
  virtual void onClick(size_t button_id) override
  {
    printf("Clicked button #%zu\n", button_id);
  }

  virtual void onRelease(size_t button_id) override
  {
    printf("Released button #%zu\n", button_id);
  }
};

DebugController g_debugController;

Photoshop::Photoshop(size_t width, size_t height) :
    m_canvas(1000, 1000),
    m_widgetTree(nullptr),
    m_window(sf::VideoMode(width, height), "Photoshop", sf::Style::Close)
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
  LayoutBox root_box(Length(m_window.getSize().x, Unit::Pixel),
                     Length(m_window.getSize().y, Unit::Pixel));

  /*
  m_widgetTree =
      new gui::Button(g_debugController, LayoutBox(5_cm, 5_cm, Align::Center));
  */
  m_widgetTree =
    new gui::CanvasView(m_canvas, LayoutBox(15_cm, 15_cm, Align::Center));

  m_widgetTree->onParentUpdate(root_box);
}

void Photoshop::runMainLoop(void)
{
  TransformStack   stack;
  SfmlEventManager event_manager(m_window, stack);
  SfmlRenderTarget wrapped_rt(m_window);

  while (m_window.isOpen())
  {
    event_manager.sendEvents(m_widgetTree);
    if (!m_window.isOpen())
    {
      break;
    }

    m_widgetTree->draw(stack, wrapped_rt);
    m_window.display();
  }
}
