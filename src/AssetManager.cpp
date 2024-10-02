#include "AssetManager.h"

#include <SFML/Graphics/Texture.hpp>
#include <cassert>

sf::Font AssetManager::s_defaultFont  = sf::Font();
bool     AssetManager::s_isFontLoaded = false;

sf::Texture AssetManager::s_buttonTexture         = sf::Texture();
bool        AssetManager::s_isButtonTextureLoaded = false;

const sf::Font& AssetManager::getDefaultFont(void)
{
  if (s_isFontLoaded)
    return s_defaultFont;

  bool success = s_defaultFont.loadFromFile("assets/RobotoMono-Regular.ttf");
  assert(success);
  s_isFontLoaded = true;

  return s_defaultFont;
}

const sf::Texture& AssetManager::getButtonTexture(void)
{
  if (s_isButtonTextureLoaded)
    return s_buttonTexture;

  bool success = s_buttonTexture.loadFromFile("assets/button_square.png");
  assert(success);
  s_isButtonTextureLoaded = true;

  return s_buttonTexture;
}

