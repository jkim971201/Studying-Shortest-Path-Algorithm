#pragma once

#include <vector>

class Grid
{
	public:

		Grid(int width, int height, int xS, int yS, int xT, int yT)
			: width_  (  width),
			  height_ ( height),
				xS_     (     xS),
				yS_     (     yS),
				xT_     (     xT),
				yT_     (     yT)
		{}

		int width()  const { return width_;  }
		int height() const { return height_; }

		int xS() const { return xS_; }
		int yS() const { return yS_; }
		int xT() const { return xT_; }
		int yT() const { return yT_; }

	private:

		int width_;
		int height_;

		int xS_;
		int yS_;
		int xT_;
		int yT_;
};
