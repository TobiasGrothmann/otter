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

/// sample an image from disk
class Image
{
public:
    
    Image() = delete;
    /// read image and define scaling
    /// @param path file path to image
    /// @param fitToBounds bounds to scale image onto
    /// @param moveToMid wether the image should be centered in boulds
    /// @param fillInBothDimensions wether the image should fill both dimensions; otherwise
    /// @param sampleMethod define what kind of value will be returned when sampling out of bounds
    Image(const std::string& path,
          const std::shared_ptr<Rectangle> fitToBounds,
          bool moveToMid,
          bool fillInBothDimensions,
          OutOfBoundsSampleMethod sampleMethod = OutOfBoundsSampleMethod::Black);
    
    /// save image to disk
    /// @param path path to save to
    void save(const std::string& path);
    
    // methods in plotter space
    /// get width in plotter space
    double getWidth() const;
    /// get height in plotter space
    double getHeight() const;
    /// get size in plotter space
    Vec2 getSize() const;
    /// get bounds in plotter space
    std::shared_ptr<Rectangle> getBounds();
    
    /// get pixel color
    /// @param coord coord in plotter space
    Color getPixel(const Vec2& coord) const;
    /// set pixel color
    /// @param coord coord in plotter space
    /// @param color color to set
    void setPixel(const Vec2& coord, const Color& color);

    // methods in pixel space
    /// get pixel color
    /// @param pixel pixel at original image scale
    Color getPixelInOriginalImage(const otter::Vec2i& pixel) const;
    /// get pixel color
    /// @param coord pixel at original image scale; will be rounded to nearest pixel
    Color getPixelInOriginalImage(const Vec2& coord) const;
    /// set pixel color
    /// @param pixel pixel at original image scale
    /// @param color color to set
    void setPixelInOriginalImage(const otter::Vec2i& pixel, const Color& color);
    
    /// get width of image in pixels
    int getWidthOfOriginalImage() const;
    /// get height of image in pixels
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
