#include "Photoshop.h"

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "Common/GUI/Widget.h"
#include "EditorState.h"
#include "GUI/Button.h"
#include "GUI/CanvasView.h"
#include "GUI/EditorView.h"
#include "GUI/ToolSelector.h"
#include "GUI/WidgetContainer.h"
#include "Impl/EventManager/SfmlEventManager.h"
#include "Impl/RenderTarget/SfmlRenderTarget/RenderTarget.h"
#include "Impl/TransformStack.h"
#include "LogHelpers.h"

using namespace layout;

Logger Photoshop::s_logger = Logger("Photoshop");

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
  LOG_ASSERT(s_logger, width > 0, "Width is zero!");
  LOG_ASSERT(s_logger, height > 0, "Height is zero!");

  s_logger.LOG_INFO(ContentType::TEXT, "Opened Photoshop window %zux%zu", width,
                    height);

  // m_editorState.getTools().addTool(new BrushTool(10));
  m_editorState.newCanvas("Untitled.png", 1000, 1000);

  s_logger.LOG_TRACE(ContentType::TEXT, "Created Photoshop instance");
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

  s_logger.LOG_TRACE(ContentType::TEXT, "Destroyed Photoshop instance");
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
      LayoutBox(15_cm, 15_cm, Align::Free)));

  root->addWidget(editor_view);

  m_widgetTree = root;
  m_widgetTree->onParentUpdate(root_box);

  s_logger.LOG_TRACE(ContentType::TEXT, "Initialized Photoshop GUI");
}

void Photoshop::runMainLoop(void)
{
  TransformStack   stack;
  SfmlEventManager event_manager(m_window, stack);
  SfmlRenderTarget wrapped_rt(m_window);
  const sf::Color bg_color(50, 50, 50);

  /* TODO: remove stub */
  m_editorState.getFilters().setLastFilter(
      m_editorState.getFilters().getFilterCount() - 1);

  /* TODO: remove stub */
  m_editorState.getTools().setActiveTool(
      m_editorState.getTools().getToolCount() - 1);

  s_logger.LOG_TRACE(ContentType::TEXT, "Entering Photoshop main loop");
  while (m_window.isOpen())
  {
    event_manager.sendEvents(m_widgetTree);
    if (!m_window.isOpen())
    {
      break;
    }

    m_window.clear(bg_color);
    m_widgetTree->draw(stack, wrapped_rt);
    m_window.display();
  }
  s_logger.LOG_TRACE(ContentType::TEXT, "Leaving Photoshop main loop");
}
