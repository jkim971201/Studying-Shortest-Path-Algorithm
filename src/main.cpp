#include <iostream>
#include <string>
#include <stdio.h>

#include "Painter.h"
#include "Grid.h"

using namespace gui;

int main(int argc, char* argv[]) 
{
	int width  = 32;
	int height = 32;

	int startX = 0;
	int startY = 0;

	int goalX = width - 1;
	int goalY = height - 1;

	Grid grid(width, height, startX, startY, goalX, goalY);

  QApplication app(argc, argv);
  Painter vis(&app, &grid);
  vis.openWindow();

	return 0;
}
