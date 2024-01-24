#include <iostream>
#include <string>
#include <stdio.h>
#include <queue>

#include "Painter.h"
#include "Grid.h"

using namespace gui;

struct Node
{
  int x = 0;
  int y = 0;
	float costG = 0.0;
  float costF = 0.0;

  Node(int _x, int _y)
    : x(_x), y(_y)
  {}

  // To make queue as a min priority
  bool operator<(const Node other) const { return (costF > other.costF); };
};

class AStar
{
  public:

    AStar(Grid* grid)
      : grid_ (grid)
    {}

  private:

    std::priority_queue<Node> queue_;
    Grid* grid_;
};

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
