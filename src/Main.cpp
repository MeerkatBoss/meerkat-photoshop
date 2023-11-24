#include <cstdio>

#include "Photoshop.h"

int main()
{
  Photoshop app(1080, 720);
  app.initGUI();
  app.runMainLoop();
  return 0;
}
