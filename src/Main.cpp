#include <cstdio>

#include "Photoshop.h"
#include "mklog/LogManager.h"
#include "mklog/LogMessage.h"
#include "mklog/LogRoute.h"
#include "mklog/LogRoutingRule.h"
#include "mklog/Logger.h"
#include "mklog/writers/HtmlLogWriter.h"
#include "mklog/writers/StderrLogWriter.h"
#include "mklog/writers/TextLogWriter.h"
#include "LogHelpers.h"

void setupLogs()
{
  using mklog::LogManager; // TODO: Load config from file
  using mklog::LogRoute;
  using mklog::MessageSeverity;

  LogRoute minSeverityInfo =
      LogRoute::makeRoute<mklog::SeverityRoutingRule>(
          /* minSeverity = */ MessageSeverity::INFO);

  LogManager::addWriter<mklog::StderrLogWriter>()
      .useAnsiColors()
      .setRoute(minSeverityInfo);

  auto& textLogs = LogManager::addWriter<mklog::TextLogWriter>().setFile(
      ".log/log.txt");
  if (!textLogs.valid())
  {
    textLogs.setFile("log.txt");
  }

  auto& htmlLogs = LogManager::addWriter<mklog::HtmlLogWriter>().setFile(
      ".log/log.html");
  if (!htmlLogs.valid())
  {
    htmlLogs.setFile("log.html");
  }

  LogManager::initLogs();
}

int main()
{
  setupLogs();

  mklog::Logger logger("main");

  logger.LOG_INFO(ContentType::TEXT, "Started logs");

  Photoshop app(1800, 1200);
  app.loadPlugins("Plugins/");
  app.initGUI();
  app.runMainLoop();

  logger.LOG_INFO(ContentType::TEXT, "Finishing program");
  return 0;
}
