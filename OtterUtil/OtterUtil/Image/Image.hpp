#pragma once
#include "stl.hpp"
#include "Otter.hpp"

namespace cv { class Mat; }

namespace otter {

enum OutOfBoundsSampleMethod
{
    Black,
    White,
    Wrap,
};

class Image
{
public:
    
    Image() = delete;
    Image(const std::string& path,
          const std::shared_ptr<Rectangle> fitToBounds,
          bool moveToMid,
          bool fillInBothDimensions,
          OutOfBoundsSampleMethod sampleMethod = OutOfBoundsSampleMethod::Black);
    
    void save(const std::string& path);
    
    // methods in plotter space
    double getWidth() const;
    double getHeight() const;
    Vec2 getSize() const;
    std::shared_ptr<Rectangle> getBounds();
    
    Color getPixel(const Vec2& coord) const;
    void setPixel(const Vec2& coord, const Color& color);

    // methods in pixel space
    Color getPixelInOriginalImage(const otter::Vec2i& pixel) const;
    Color getPixelInOriginalImage(const Vec2& coord) const;
    void setPixelInOriginalImage(const otter::Vec2i& pixel, const Color& color);

    int getWidthOfOriginalImage() const;
    int getHeightOfOriginalImage() const;

private:
    
    cv::Mat* image;
    
    double scaling = 1.0;
    Vec2 offset = Vec2(0, 0);
    const std::shared_ptr<Rectangle> fitToBounds;
    OutOfBoundsSampleMethod sampleMethod;
    
    otter::Vec2i getPixelCoord(const Vec2& vector) const;
};

}
