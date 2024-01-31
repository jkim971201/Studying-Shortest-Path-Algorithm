#include <iostream>
#include <string>
#include <stdio.h>
#include <queue>
#include <set>
#include <limits>
#include <functional>

#include "Painter.h"
#include "Grid.h"

using namespace gui;

class Dijkstra 
{
  public:

    Dijkstra(const Grid* grid)
      : grid_ (grid)
    {
      w_ = grid_->width();
      h_ = grid_->height();
    }

    std::vector<Point> run();

  private:

    int w_;
    int h_;

    const Grid* grid_;

    int  loc2id(int x, int y) const { return x + w_ * y; }
    void id2loc(int v, int& x, int& y) const 
    { 
      x = v % w_;
      y = v / w_;
    }

    const std::vector<int> getNeighbors(int v_id, const std::vector<bool>& closeSet) const
    {
      std::vector<int> neighbors;
      
      int x, y;
      id2loc(v_id, x, y);

      if(x < w_ - 1) 
      {
        int newID = this->loc2id(x + 1, y);
        if( !closeSet[newID] && !grid_->isBlock(x + 1, y) )
          neighbors.push_back( newID ); // Right
      }
      if(x > 0) 
      {
        int newID = this->loc2id(x - 1, y);
        if( !closeSet[newID] && !grid_->isBlock(x - 1, y) )
          neighbors.push_back( newID ); // Left
      }
      if(y < h_ - 1) 
      {
        int newID = this->loc2id(x, y + 1);
        if( !closeSet[newID] && !grid_->isBlock(x, y + 1) )
          neighbors.push_back( newID ); // Up
      }
      if(y > 0) 
      {
        int newID = this->loc2id(x, y - 1);
        if( !closeSet[newID] && !grid_->isBlock(x, y - 1) )
          neighbors.push_back( newID ); // Down
      }

      return neighbors;
    }
};

std::vector<Point>
Dijkstra::run()
{
  // #1. Dijkstra Initialization
  int numGrid = w_ * h_;

  std::vector<float> cost(numGrid, std::numeric_limits<float>::max());
  std::vector<int>   prev(numGrid, -1);
  std::vector<bool>  isExplored(numGrid, false);

  int srcIdx  = loc2id(grid_->source().x, grid_->source().y);

	printf("srcIdx  : %d\n",  srcIdx);

	std::set<int> setPin;
	for(auto sink : grid_->sink())
	{
		int sinkIdx = loc2id(sink.x, sink.y);
		setPin.insert( sinkIdx );
	}

  cost[srcIdx] = 0;
  
  auto extractMin = [&] ()
  {
    int minIdx = -1;
    float minCost = std::numeric_limits<float>::max();

    for(int i = 0; i < numGrid; i++)
    {
			// printf("i : %d cost[i] : %f minCost: %f\n", i, cost[i], minCost);
      if(cost[i] < minCost && isExplored[i] == false)
      {
        minIdx = i;
        minCost = cost[i];
      }
    }

    return minIdx;
  };

	auto checkPin = [&] (int v_id) {return setPin.count( v_id ) != 0;	};

  // #2. Main Dijkstra Loop
  int v_cur;
	int x_cur, y_cur;
	int iter    = 0;
	int maxIter = 2000;
	
	std::set<int> visitedPin;

	int numPinToRoute = grid_->sink().size();
	while(numPinToRoute != 0)
	{
    while( iter++ < maxIter )
    {
      v_cur = extractMin();
  		id2loc(v_cur, x_cur, y_cur);
      isExplored[v_cur] = true;
  
  		printf("Iter[%03d] Cur : (%02d, %02d)\n", iter, x_cur, y_cur);
  
			if( checkPin(v_cur) && visitedPin.count( v_cur ) == 0)
			{
				visitedPin.insert( v_cur );
				printf(" Reach Sink (%02d, %02d)\n", x_cur, y_cur);
				break;
			}

      const std::vector<int> neighbors = getNeighbors( v_cur, isExplored );
  
      for(auto v_new : neighbors)
      {
  			int x_new, y_new;
  			id2loc(v_new, x_new, y_new);
  			float newCost = cost[v_cur] + grid_->getCost(x_cur, y_cur, x_new, y_new);
  
  			printf("                (%02d, %02d)\n", x_new, y_new);
  			if(newCost < cost[v_new])
  			{
  			  cost[v_new] = newCost;
  				prev[v_new] = v_cur;
  			}
      }
    }

		numPinToRoute--;
	}

	std::vector<Point> path;
	for(auto sink : grid_->sink())
	{
	  int x_tracing, y_tracing;
  	int v_tracing = loc2id(sink.x, sink.y);
  	while( v_tracing != srcIdx )
  	{
  		id2loc(v_tracing, x_tracing, y_tracing);
  		printf("Path (%d, %d)\n", x_tracing, y_tracing);
  		path.push_back( Point( x_tracing, y_tracing ) );
  		v_tracing = prev[v_tracing];
  	}
	}

  return path;
}

int main(int argc, char* argv[]) 
{
  int width  = 32;
  int height = 32;

  Point source(0, 0);
  Point sink1(width - 1, height - 1);
  Point sink2(19       , height - 1);
	Point sink3( 5, 28 );

  std::vector<Point> sink;
  sink.push_back(sink1);
  sink.push_back(sink2);
  sink.push_back(sink3);

	sink.push_back( Point(width - 1, 10) );
	sink.push_back( Point(width - 1, 15) );
	sink.push_back( Point(10, 21) );

  Grid grid(width, height, source, sink);

  std::vector<Point> block;
  for(int i = 20; i < width; i++)
  {
    for(int j = 11; j < 15; j++)
      block.push_back( Point(i, j) );
  }

  for(int i = 15; i < width; i++)
    block.push_back( Point(i, 20) );

  for(int i = 0; i < 10; i++)
    block.push_back( Point(i, 10) );

  for(auto& pt : block)
    grid.addBlock(pt);

  Dijkstra algo( &grid );

  auto path = algo.run();

  QApplication app(argc, argv);
  Painter vis(&app, &grid);

  vis.setPath(path);
  vis.setSource(source);
  vis.setSink(sink);
  vis.setBlock(block);
  vis.openWindow();

  return 0;
}
