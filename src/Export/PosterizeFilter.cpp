#include "Export/PosterizeFilter.h"

#include "Common/mklog/writers/TextLogWriter.h"

extern "C" plug::Plugin* loadPlugin(void)
{
  PosterizeFilter* filter = new PosterizeFilter();
  if (!filter->loadFilter())
  {
    filter->release();
    return nullptr;
  }
#ifndef NLOGS
  auto& log_writer =
      mklog::LogManager::addWriter<mklog::TextLogWriter>().setFile(
          "Plugins/PosterizeFilter/_log.txt");
  if (!log_writer.valid())
  {
    log_writer.setFile("_meerkat_posterize_filter_log.txt");
  }

  if (log_writer.valid())
  {
    mklog::LogManager::initLogs();
  }
#endif // NLOGS

  return filter;
}

static uint8_t posterizeChannel(uint8_t original, size_t level_count)
{
  uint8_t step = 255 / level_count;
  if (step <= 1 || original == 255)
  {
    return original;
  }

  return original - (original % step) + (step / 2);
}

void PosterizeFilter::applyFilter(plug::Canvas& canvas) const
{
  BaseFilter::applyFilter(canvas);

  const size_t width  = size_t(canvas.getSize().x);
  const size_t height = size_t(canvas.getSize().y);

  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      if (canvas.getSelectionMask().getPixel(x, y))
      {
        const plug::Color color = canvas.getPixel(x, y);
        canvas.setPixel(
            x, y,
            plug::Color(posterizeChannel(color.red, m_colorLevelsCount),
                        posterizeChannel(color.green, m_colorLevelsCount),
                        posterizeChannel(color.blue, m_colorLevelsCount),
                        color.alpha));
      }
    }
  }
}
