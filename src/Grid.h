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

class Grid
{
  public:

    Grid(int width, int height, const Point& src, const std::vector<Point>& sink)
      : width_  (  width),
        height_ ( height),
        source_ (    src)
    {
      for(auto p : sink)
        sink_.push_back(p);
    }

    int  loc2id(int  x, int  y) const { return x + width_ * y; }
    void id2loc(int v, int& x, int& y) const 
    { 
      x = v % width_;
      y = v / width_;
    }

    void addBlock(Point p)      { block_.push_back(p);             }
    void addBlock(int x, int y) { block_.push_back( Point(x, y) ); }
    
    bool isBlock(int x, int y) const
    { 
      for(auto blk : block_)
      {
        if(x == blk.x && y == blk.y)
          return true;
      }
      return false;
    }
  

    int width()  const { return width_;  }
    int height() const { return height_; }

    const Point& source()            const { return source_; }
    const std::vector<Point>& sink() const { return sink_;   }

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

    float getCost(int curX, int curY, int nextX, int nextY) const
    {
      for(auto& blk : block_)
      {
        if(nextX == blk.x && nextY == blk.y)
          return 1000000.0;
      }

      float diffX = static_cast<float>(nextX - curX);
      float diffY = static_cast<float>(nextY - curY);

      return std::abs(diffX) + std::abs(diffY);
    }

  private:

    int width_;
    int height_;

    Point source_;
    std::vector<Point> sink_;
    std::vector<Point> block_;
};
