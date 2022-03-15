#include "Grid.hpp"

using namespace std;

namespace otter {

Grid::Grid(const Rectangle& bounds, int xs, int ys, Vec2 margin)
    : totalBounds(bounds)
    , xs(xs)
    , ys(ys)
    , margin(margin)
{
}
Grid::Grid(const Vec2& bottomLeft, const Vec2& topRight, int xs, int ys, Vec2 margin)
    : Grid(Rectangle(bottomLeft, topRight), xs, ys, margin)
{
}

Grid::Grid(const shared_ptr<Rectangle>& bounds, int xs, int ys, Vec2 margin)
    : Grid(*bounds, xs, ys, margin)
{
}

Rectangle Grid::getCell(int x, int y) const
{
    const Vec2 n = Vec2(xs, ys);
    const Vec2 index = Vec2(x, y);
    Vec2 cellSize = (totalBounds.getSize() - (margin * (n - 1.0))) / n;
    
    Vec2 offset = Vec2(0, 0);
    Vec2 usedMargin = margin;
    if (square)
    {
        const double minCellSize = fmin(cellSize.x, cellSize.y);
        if (cellSize.x > cellSize.y)
        {
            cellSize.x = minCellSize;
            if (xs > 1)
                usedMargin.x = (totalBounds.getSize().x - (xs * cellSize.x)) / (xs - 1.0);
            else
                offset = Vec2((totalBounds.getSize().x - (xs * cellSize.x)) / 2.0, 0.0);
        }
        else
        {
            cellSize.y = minCellSize;
            if (ys > 1)
                usedMargin.y = (totalBounds.getSize().y - (ys * cellSize.y)) / (ys - 1.0);
            else
                offset = Vec2(0.0, (totalBounds.getSize().y - (ys * cellSize.y)) / 2.0);
        }
    }
    
    const Vec2 bottomLeft = totalBounds.bottomLeft + index * cellSize + index * usedMargin;
    return Rectangle(bottomLeft + offset, bottomLeft + cellSize + offset);
}

void Grid::setUseOnlySquareCells(bool shouldBeSquare)
{
    square = shouldBeSquare;
}

void Grid::correctXSizeToBeSquare()
{
    const Vec2 usedCellSize = (totalBounds.getSize() - Vec2(xs - 1, ys - 1) * margin) / Vec2(xs, ys);
    const double xError = usedCellSize.x - usedCellSize.y;
    const double xErrorTotal = xError * xs;
    totalBounds.bottomLeft.x += xErrorTotal * 0.5;
    totalBounds.topRight.x -= xErrorTotal * 0.5;
}

}
