#include "Image.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

namespace otter {

Image::Image(const string& path, const shared_ptr<Rectangle> fitToBounds, bool moveToMid, bool fillInBothDimensions, OutOfBoundsSampleMethod sampleMethod)
    : fitToBounds(fitToBounds)
    , sampleMethod(sampleMethod)
{
    image = new Mat(imread(path, IMREAD_COLOR));
    Rectangle imageBounds = Rectangle(Vec2(0, 0), Vec2(getWidthOfOriginalImage(), getHeightOfOriginalImage()));
    Rectangle normalizedBounds = Rectangle(imageBounds);
    normalizedBounds.normalize(fitToBounds, moveToMid, fillInBothDimensions);
    
    this->offset = normalizedBounds.bottomLeft;
    this->scaling = normalizedBounds.getWidth() / imageBounds.getWidth();
}

void Image::save(const string& path)
{
    imwrite(path, *image);
}


Color Image::getPixelInOriginalImage(const otter::Vec2i& pixelCoordIn) const
{
    otter::Vec2i pixelCoord = pixelCoordIn;
    if (pixelCoord.x < 0 || pixelCoord.x > getWidthOfOriginalImage() - 1)
    {
        switch (sampleMethod) {
            case OutOfBoundsSampleMethod::Black:
                return Color(0.0);
            case OutOfBoundsSampleMethod::White:
                return Color(1.0);
            case OutOfBoundsSampleMethod::Wrap:
                pixelCoord.x = pixelCoord.x % getWidthOfOriginalImage();
                break;
        }
    }
    if (pixelCoord.y < 0 || pixelCoord.y > getHeightOfOriginalImage() - 1)
    {
        switch (sampleMethod) {
            case OutOfBoundsSampleMethod::Black:
                return Color(0.0);
            case OutOfBoundsSampleMethod::White:
                return Color(1.0);
            case OutOfBoundsSampleMethod::Wrap:
                pixelCoord.y = pixelCoord.y % getWidthOfOriginalImage();
                break;
        }
    }
    const Vec3b& pixel = image->at<Vec3b>(image->rows - pixelCoord.y - 1, pixelCoord.x);
    return Color(pixel[2] / 255.0, pixel[1] / 255.0, pixel[0] / 255.0);
}

Color Image::getPixelInOriginalImage(const Vec2& coord) const
{
    return getPixelInOriginalImage(coord.round());
}

void Image::setPixelInOriginalImage(const otter::Vec2i& pixelCoord, const Color& color)
{
    Vec3b& pixel = image->at<Vec3b>(image->rows - pixelCoord.y - 1, pixelCoord.x);
    pixel.val[0] = round(color.b * 255.0);
    pixel.val[1] = round(color.g * 255.0);
    pixel.val[2] = round(color.r * 255.0);
}

int Image::getWidthOfOriginalImage() const
{
    return image->cols;
}

int Image::getHeightOfOriginalImage() const
{
    return image->rows;
}



double Image::getWidth() const
{
    return fitToBounds->getWidth();
}

double Image::getHeight() const
{
    return fitToBounds->getHeight();
}

Vec2 Image::getSize() const
{
    return fitToBounds->getSize();
}

shared_ptr<Rectangle> Image::getBounds()
{
    return Rectangle::create(fitToBounds);
}

Color Image::getPixel(const Vec2& coord) const
{
    return getPixelInOriginalImage(getPixelCoord(coord));
}

void Image::setPixel(const Vec2& coord, const Color& color)
{
    setPixelInOriginalImage(getPixelCoord(coord), color);
}



otter::Vec2i Image::getPixelCoord(const Vec2& vector) const
{
    return otter::Vec2i((vector.x - offset.x) / scaling, (vector.y - offset.y) / scaling);
}

}
