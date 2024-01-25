#pragma once

#include <vector>
#include <cmath>

struct Point
{
	int x = 0;
	int y = 0;

	Point() {}

	Point(int _x, int _y)
		: x(_x), y(_y)
	{}
};

//struct Point
//{
//  int x = 0;
//  int y = 0;
//  float costF = 0.0;
//
//	Point() {}
//
//  Point(int _x, int _y, float _costF)
//    : x(_x), y(_y), costF(_costF)
//  {}
//
//  Point(int _x, int _y)
//    : x(_x), y(_y)
//  {}
//
//  // To make queue as a min priority
//  // bool operator<(const Point other) const { return (costF < other.costF); };
//  bool operator==(const Point other) const { return (other.x == x && other.y == y); }
//};

class Grid
{
  public:

    Grid(int width, int height, Point S, Point T)
      : width_  (  width),
        height_ ( height),
        start_  (      S),
        end_    (      T)
    {}

		void addBlock(Point p)      { block_.push_back(p);             }
		void addBlock(int x, int y) { block_.push_back( Point(x, y) ); }

    int width()  const { return width_;  }
    int height() const { return height_; }

		int xS() const { return start_.x; }
		int yS() const { return start_.y; }
		int xT() const { return   end_.x; }
		int yT() const { return   end_.y; }

    const Point& start() const { return start_; }
    const Point&   end() const { return   end_; }

		// Euclidean
//    float getHeuristicCost(int curX, int curY) const
//    {
//      float diffX = static_cast<float>(curX - xT_);
//      float diffY = static_cast<float>(curY - yT_);
//
//      return std::sqrt(diffX * diffX + diffY * diffY);
//    }

		float getCost(const Point& cur, const Point& next) const
		{
			int curX  = cur.x;
			int curY  = cur.y;
			int nextX = next.x;
			int nextY = next.y;

			for(auto& blk : block_)
			{
				if(nextX == blk.x && nextY == blk.y)
					return 1000000.0;
			}

      float diffX = static_cast<float>(nextX - curX);
      float diffY = static_cast<float>(nextY - curY);

      return std::abs(diffX) + std::abs(diffY);
		}

    float getHeuristicCost(const Point& next) const
    {
			int nextX = next.x;
			int nextY = next.y;

      float diffX = static_cast<float>(nextX - end_.x);
      float diffY = static_cast<float>(nextY - end_.y);

      return std::abs(diffX) + std::abs(diffY);
		}

    float getHeuristicCost(int nextX, int nextY) const
    {
      float diffX = static_cast<float>(nextX - end_.x);
      float diffY = static_cast<float>(nextY - end_.y);

      return std::abs(diffX) + std::abs(diffY);
		}


  private:

    int width_;
    int height_;

		Point start_;
		Point   end_;

		std::vector<Point> block_;
};
