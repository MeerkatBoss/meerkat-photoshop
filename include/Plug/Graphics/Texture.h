/**
 * @file Texture.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief Texture data structure
 *
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __PLUG_GRAPHICS_TEXTURE_H
#define __PLUG_GRAPHICS_TEXTURE_H

#include <cstddef>

#include "Plug/Color.h"

namespace plug
{

/**
 * @brief Array of colors (stored on the CPU)
 *
 */
struct Texture
{
  Texture(size_t width, size_t height, const Color* data) :
      data(data), width(width), height(height)
  {
  }

  Texture(size_t width, size_t height) :
      data((Color*)new Color[width * height]), width(width), height(height)
  {
  }

  /**
   * @brief Image data
   *
   */
  const Color* const data;

  /**
   * @brief Image width (in pixels)
   *
   */
  const size_t width;

  /**
   * @brief Image height (in pixels)
   *
   */
  const size_t height;
};

} // namespace plug

#endif /* Texture.h */
