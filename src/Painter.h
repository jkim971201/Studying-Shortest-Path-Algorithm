#pragma once

#include <filesystem>
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QScreen>
#include <unordered_map>
#include <vector>

#include "Grid.h"

namespace gui
{

class GCell
{
  public: 
  
		GCell() {}

    GCell(int lx, int ly, int w, int h)
      : lx_ (lx),
        ly_ (ly),
        w_  ( w),
        h_  ( h)
    {
			rect_ = QRectF(lx, ly, w, h); 
		}

    // Getters
    const QRectF* rect() const { return &rect_; }

		int lx() const { return lx_;    }
		int ly() const { return lx_;    }
		int  w() const { return w_;     }
		int  h() const { return h_;     }

  private:

		int lx_;
		int ly_;
		int w_;
		int h_;

    QRectF rect_;
};

class Painter : public QWidget
{
  public:

    // w : window width
    // h : window height
    // color : background color
    Painter(QApplication* app, Grid* grid);

    // APIs
    void openWindow();

    // Setters
    void setRectFillColor(QColor color) { rectFillColor_ = color; }
    void setRectLineColor(QColor color) { rectLineColor_ = color; }

  protected:

    void paintEvent(QPaintEvent* event);
    // This function is not for users

  private:

    QApplication* app_;
    QColor baseColor_;
    QColor rectFillColor_;
    QColor rectLineColor_;

    Grid* grid_;

    std::vector<std::vector<GCell>> gCells_;

    void drawLine (QPainter* painter, QPointF p1, QPointF p2, QColor lineColor = Qt::black, int thickness = 1);
    void drawRect (QPainter* painter, const QRectF& rect, QColor rectColor = Qt::white, QColor rectLineColor = Qt::black, int thickness = 1);
    void drawRect (QPainter* painter, const QRectF* rect, QColor rectColor = Qt::white, QColor rectLineColor = Qt::black, int thickness = 1);
    void drawRect (QPainter* painter, int lx, int ly, int w, int h);
    void drawGCell(QPainter* painter, const GCell& gCell, QColor rectColor, QColor rectLineColor, int thickness);

    int w_;
    int h_;
    int offset_;

    // Router Database Information
    int   numGCellX_;
    int   numGCellY_;
    float gridWidth_;
    float gridHeight_;
    float gCellWidth_;
    float gCellHeight_;

    std::vector<std::vector<GCell>> gCellInsts_;
};

}
