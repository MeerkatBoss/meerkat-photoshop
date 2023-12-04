#include "EditorState.h"

#include <dirent.h>
#include <dlfcn.h>

#include "Canvas/Canvas.h"
#include "Impl/Util/Sfml.h"
#include "LogHelpers.h"

Logger EditorState::s_logger = Logger("Editor State");

typedef plug::Plugin* plug_loader_t(void);

static bool isSharedObject(const char* filename)
{
  const size_t name_len = strlen(filename);
  return strcmp(".so", filename + name_len - 3) == 0;
}

// TODO: code cleanup

plug::Plugin* EditorState::getPluginFromDir(const char* dirpath)
{
  constexpr size_t MAX_PATH               = 1024;
  static char      FULLPATH[MAX_PATH + 1] = "";

  s_logger.LOG_TRACE(Content::TEXT, "Searching for plugins in '%s'", dirpath);

  const size_t base_len = strnlen(dirpath, MAX_PATH + 1);
  LOG_ASSERT(s_logger, base_len <= MAX_PATH, "Base path too long");
  if (base_len > MAX_PATH)
  {
    return nullptr;
  }

  DIR* plugdir = opendir(dirpath);
  LOG_ASSERT(s_logger, plugdir != NULL,
      "Failed to open plugin directory '%s'", dirpath);
  if (plugdir == NULL)
  {
    return nullptr;
  }

  struct dirent* child = NULL;
  bool found = 0;
  while (!found && (child = readdir(plugdir)))
  {
    if (child->d_type != DT_REG || !isSharedObject(child->d_name))
    {
      continue;
    }
    
    const size_t filename_len = strnlen(child->d_name, 256);
    LOG_ASSERT(s_logger, base_len + filename_len <= MAX_PATH, "Filename too long");
    if (base_len + filename_len > MAX_PATH)
    {
      return nullptr;
    }

    strncpy(FULLPATH, dirpath, base_len);
    strncat(FULLPATH, child->d_name, filename_len);

    s_logger.LOG_TRACE(Content::TEXT, "Found plugin object '%s'", FULLPATH);

    found = true;
  }
  closedir(plugdir);

  if (!found)
  {
    s_logger.LOG_TRACE(Content::TEXT, "No plugins in '%s'", FULLPATH);
    return nullptr;
  }

  void* plugin = dlopen(FULLPATH, RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);

  LOG_ASSERT(s_logger, plugin != NULL,
      "Failed to load shared object '%s': %s", FULLPATH, dlerror());
  if (plugin == NULL)
  {
    return nullptr;
  }

  plug_loader_t* loader = (plug_loader_t*) dlsym(plugin, "loadPlugin");
  dlclose(plugin);

  LOG_ASSERT(s_logger, loader != NULL, "Failed to find symbol 'loadPlugin': %s",
      dlerror());
  if (loader == NULL)
  {
    return nullptr;
  }

  plug::Plugin* loaded = loader();

  LOG_ASSERT(s_logger, loaded != nullptr, "Failed to load plugin from '%s'",
      FULLPATH);
  s_logger.LOG_INFO(Content::TEXT, "Loaded plugin '%s'",
      loaded->getPluginData()->getName());

  return loaded;
}

void EditorState::loadPlugins(const char* dirpath)
{
  constexpr size_t MAX_PATH               = 1024;
  static char      FULLPATH[MAX_PATH + 1] = "";

  s_logger.LOG_TRACE(Content::TEXT, "Loading plugins from '%s'", dirpath);

  const size_t base_len = strnlen(dirpath, MAX_PATH + 1);
  LOG_ASSERT(s_logger, base_len <= MAX_PATH, "Base path too long");
  if (base_len > MAX_PATH)
  {
    return;
  }

  DIR* plugdir = opendir(dirpath);
  LOG_ASSERT(s_logger, plugdir != NULL, "Failed to open plugin directory '%s'",
      dirpath);
  if (!plugdir)
  {
    return;
  }

  struct dirent* child = NULL;
  while ((child = readdir(plugdir)))
  {
    if (child->d_type != DT_DIR)
    {
      continue;
    }
    size_t child_len = strnlen(child->d_name, 256);
    LOG_ASSERT(s_logger, base_len + child_len + 1 <= MAX_PATH,
               "Full path too long");
    if (base_len + child_len + 1 > MAX_PATH)
    {
      continue;
    }

    strncpy(FULLPATH, dirpath, MAX_PATH);
    strncat(FULLPATH, child->d_name, child_len);
    strncat(FULLPATH, "/", 1);

    plug::Plugin* plugin = getPluginFromDir(FULLPATH);
    if (plugin == nullptr)
    {
      continue;
    }

    plug::Tool* tool = static_cast<plug::Tool*>(
        plugin->tryGetInterface(size_t(plug::PluginGuid::Tool)));
    if (tool != nullptr)
    {
      s_logger.LOG_INFO(Content::TEXT, "Loaded tool '%s'",
          tool->getPluginData()->getName());
      m_tools.addTool(tool);
    }

    plugin->release();

    // TODO: load filters
  }
  closedir(plugdir);
}

void EditorState::setActiveCanvas(Canvas* canvas)
{
  for (size_t i = 0; i < m_canvases.getSize(); ++i)
  {
    if (canvas == m_canvases[i])
    {
      m_activeCanvas = canvas;
      m_tools.setActiveCanvas(*m_activeCanvas);
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
  setActiveCanvas(opened);
}

void EditorState::saveCanvas(const Canvas* canvas)
{
  size_t    width  = canvas->getSize().x;
  size_t    height = canvas->getSize().y;
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
