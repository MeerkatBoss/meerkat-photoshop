#include "Photoshop.h"

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "EditorState.h"
#include "GUI/Button.h"
#include "GUI/CanvasView.h"
#include "GUI/EditorView.h"
#include "GUI/ToolSelector.h"
#include "GUI/Widget.h"
#include "GUI/WidgetContainer.h"
#include "Impl/EventManager/SfmlEventManager.h"
#include "Impl/LayoutBox/LayoutBox.h"
#include "Impl/RenderTarget/SfmlRenderTarget/RenderTarget.h"
#include "Impl/TransformStack.h"
#include "Tool/BrushTool.h"

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
    m_editorState(),
    m_widgetTree(nullptr),
    m_window(sf::VideoMode(width, height), "Photoshop", sf::Style::Close)
{
  m_editorState.getTools().addTool(new BrushTool(10));
  m_editorState.newCanvas("Untitled.png", 1000, 1000);
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
  gui::WidgetContainer* root =
      new gui::WidgetContainer(LayoutBox(100_per, 100_per, Align::Center));

  gui::EditorView* editor_view = new gui::EditorView(
      m_editorState, LayoutBox(100_per, 100_per, Align::Center));
  editor_view->addCanvasView(new gui::CanvasView(
      m_editorState.getTools(), *m_editorState.getActiveCanvas(),
      LayoutBox(15_cm, 15_cm, Align::Center)));

  root->addWidget(editor_view);

  m_widgetTree = root;
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
