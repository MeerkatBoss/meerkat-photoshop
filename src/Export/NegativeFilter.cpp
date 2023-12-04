#include "Export/NegativeFilter.h"

#include "BaseFilter.h"
#include "Common/mklog/writers/TextLogWriter.h"

extern "C" plug::Plugin* loadPlugin(void)
{
#ifndef NLOGS
  auto& log_writer =
      mklog::LogManager::addWriter<mklog::TextLogWriter>().setFile(
          "Plugins/NegativeFilter/_log.txt");
  if (!log_writer.valid())
  {
    log_writer.setFile("_meerkat_negative_filter_log.txt");
  }

  if (log_writer.valid())
  {
    mklog::LogManager::initLogs();
  }
#endif // NLOGS

  return new NegativeFilter();
}

static uint8_t clampChannel(size_t value) { return value > 255 ? 255 : value; }

void NegativeFilter::applyFilter(plug::Canvas& canvas) const
{
  BaseFilter::applyFilter(canvas);

  const size_t width  = size_t(canvas.getSize().x);
  const size_t height = size_t(canvas.getSize().y);

  for (size_t x = 0; x < width; ++x)
  {
    for (size_t y = 0; y < height; ++y)
    {
      const plug::Color color = canvas.getPixel(x, y);
      canvas.setPixel(x, y,
                      plug::Color(clampChannel(255 - color.red),
                                  clampChannel(255 - color.green),
                                  clampChannel(255 - color.blue), color.alpha));
    }
  }
}
