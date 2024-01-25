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



class AStar
{
  public:

    AStar(const Grid* grid)
      : grid_ (grid)
    {
			w_  = grid_->width();
			h_  = grid_->height();
			xS_ = grid_->xS();
			xT_ = grid_->xT();
			yS_ = grid_->yS();
			yT_ = grid_->yT();
		}

		std::vector<Point> run();

  private:


		int w_;
		int h_;

		int xS_;
		int yS_;
		int xT_;
		int yT_;

    const Grid* grid_;
};

std::vector<Point>
AStar::run()
{
	//int getID = [&] (int x, int y) { return x + y * w_; };
	//std::function<int(const Point&)> getID = [&] (const Point& pt) { return pt.x + pt.y * w_; };

	int lx = 0;
	int ly = 0;
	int ux = w_ - 1;
	int uy = h_ - 1;
	int numPoint = w_ * h_;

	int curX = xS_;
	int curY = yS_;

	printf("Start : (%d, %d)\n", xS_, yS_);

	std::vector<float> costG(numPoint, std::numeric_limits<float>::max());
	std::vector<float> costF(numPoint, std::numeric_limits<float>::max());
	std::map<int, int> cameFrom;

	auto getID     = [&] (const Point& pt) { return pt.x + pt.y * w_; };
	auto getIDbyXY = [&] (int x, int y) { return x + y * w_; };
	auto cmp = [&] (const Point& a, const Point& b) {	return costF[getID(a)] < costF[getID(b)];	};


	Point start( curX, curY );
	float startH = grid_->getHeuristicCost(start);

	costG[getID(start)] = 0.0;
	costF[getID(start)] = startH;

	std::set<Point, decltype(cmp)> openSet_(cmp);
	std::set<int> closeSet_;

	openSet_.insert( start );

  auto ifExist = [&] (const Point& newPoint) 
	{
		for(auto& n : openSet_)
		{
			if(n.x == newPoint.x && n.y == newPoint.y) 
				return true;
		}
		return false;
	};

	int iter = 0;
	int maxIter = 100;
	while(!openSet_.empty() && iter < maxIter)
	{
		auto curPointItr = openSet_.begin();
		const Point curPoint = *curPointItr; // must be a copy
		int curPointID = getID( curPoint );
		curX = curPoint.x;
		curY = curPoint.y;
		closeSet_.insert( curPointID );
	
		printf("\n");
		printf("Iter[%03d] Cur : (%02d, %02d)\n", iter, curX, curY);

		if(curPoint.x == xT_ && curPoint.y == yT_)
		{
			printf("Finish : cur (%02d, %02d) goal (%02d, %02d)\n", curPoint.x, curPoint.y, xT_, yT_);
			break;
		}
		openSet_.erase( curPointItr );

		Point newPoint;
		for(int i = 0; i < 4; i++)
		{
			if( i == 0 )      // UP
			{
				if(curY + 1 <= uy && closeSet_.count( getIDbyXY(curX, curY + 1) == 0)) 
					newPoint = Point(curX, curY + 1);
				else					     
					continue;
			}
			else if( i == 1 ) // DOWN
			{
				if(curY - 1 >= ly && closeSet_.count( getIDbyXY(curX, curY - 1) == 0)) 
					newPoint = Point(curX, curY - 1);
				else					     
					continue;
			}
			else if( i == 2 ) // RIGHT
			{
				if(curX + 1 <= ux && closeSet_.count( getIDbyXY(curX + 1, curY) == 0)) 
					newPoint = Point(curX + 1, curY);
				else					     
					continue;
			}
			else              // LEFT
			{
				if(curX - 1 >= lx && closeSet_.count( getIDbyXY(curX - 1, curY) == 0))
					newPoint = Point(curX - 1, curY);
				else					     
					continue;
			}

			int newPointID = getID(newPoint);
			float newG = costG[curPointID] + grid_->getCost(curPoint, newPoint);

			printf("\n");
			printf("  See  (%02d, %02d)\n", newPoint.x, newPoint.y);
			//printf("  newG = %f + %f\n", costG[curPointID], grid_->getCost(curPoint, newPoint));
			//printf("  oldG = %f\n", costG[newPointID]);
			//printf("  curPoint = (%d, %d)\n", curPoint.x, curPoint.y);
			//printf("  newPoint = (%d, %d)\n", newPoint.x, newPoint.y);

			if(newG < costG[newPointID])
			{
				cameFrom[newPointID] = curPointID;
				float newH = grid_->getHeuristicCost(newPoint);
				costG[newPointID] = newG;
				costF[newPointID] = newG + newH;

				if(!ifExist(newPoint))
				{
					printf("  Push (%02d, %02d) newG : %f newH : %f newF : %f\n", newPoint.x, newPoint.y, newG, newH, newG + newH);
					openSet_.insert(newPoint);
				}
			}
		}

		iter += 1;
	}

	std::vector<Point> path;

	int pathID = getIDbyXY(xT_, yT_);

	for(auto& kv : cameFrom)
	{
		path.push_back( Point(pathID % w_, pathID / w_) );
		pathID = cameFrom[pathID];
	}

	return path;
}

int main(int argc, char* argv[]) 
{
  int width  = 32;
  int height = 32;

  int startX = 0;
  int startY = 0;

  int endX = width - 1;
  int endY = height - 1;

	Point start(startX, startY);
	Point end(endX, endY);

  Grid grid(width, height, start, end);
	std::vector<Point> block;
	block.push_back( Point(0, 3) );
	block.push_back( Point(1, 7) );

	for(auto& pt : block)
		grid.addBlock(pt);

	AStar algo(&grid);
	auto path = algo.run();

  QApplication app(argc, argv);
  Painter vis(&app, &grid);
	vis.highlightPath(path);
	vis.highlightBlock(block);
  vis.openWindow();

  return 0;
}
