/**
 * @file AssetManager.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-07
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __ASSETS_ASSET_MANAGER_H
#define __ASSETS_ASSET_MANAGER_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

class AssetManager
{
public:
  static const sf::Font&    getDefaultFont(void);
  static const sf::Texture& getButtonTexture(void);

private:
  static sf::Font s_defaultFont;
  static bool     s_isFontLoaded;

  static sf::Texture s_buttonTexture;
  static bool        s_isButtonTextureLoaded;
};

#endif /* AssetManager.h */
