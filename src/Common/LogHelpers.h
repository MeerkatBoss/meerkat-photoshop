/**
 * @file LogHelpers.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __LOG_HELPERS_H
#define __LOG_HELPERS_H

#include <cstdlib>

#include "Common/mklog/Logger.h"

using mklog::Logger;
using ContentType = mklog::MessageContentType;
using Content     = mklog::MessageContentType;
using MessageFd   = mklog::LogManager::MessageFd;

#ifndef NLOGS

#ifndef NASSERT

#define LOG_ASSERT(logger, condition, ...)                                 \
  do                                                                       \
  {                                                                        \
    if (!(condition))                                                      \
    {                                                                      \
      logger.LOG_FATAL(ContentType::TEXT,                                  \
                       "Failed assertion '" #condition "': " __VA_ARGS__); \
      abort();                                                             \
    }                                                                      \
  } while (0)

#else

#define LOG_ASSERT(logger, condition, ...)                                   \
  do                                                                         \
  {                                                                          \
    if (!(condition))                                                        \
    {                                                                        \
      logger.LOG_WARNING(ContentType::TEXT,                                  \
                         "Failed assertion '" #condition "': " __VA_ARGS__); \
    }                                                                        \
  } while (0)

#endif // NASSERT

#else

#ifndef NASSERT

#define LOG_ASSERT(logger, condition, ...) assert((condition))

#else

#define LOG_ASSERT(...)

#endif // NASSERT

#endif // NLOG

#endif /* LogHelpers.h */
