#include "Common.h"
#include <iostream>
#include <utility>

using namespace std;

class RectangleLimitedShapes : public IShape {
public:

  RectangleLimitedShapes() : high_left_end_(Point{0, 0}), size_(Size{0, 0}) {}

  RectangleLimitedShapes(Point point, Size size, std::shared_ptr<ITexture> texture) : high_left_end_(point),
                                                                                      size_(size),
                                                                                      texture_(std::move(texture)) {}

  void SetPosition(Point point) override {
    high_left_end_ = point;
  }

  Point GetPosition() const override {
    return high_left_end_;
  }

  void SetSize(Size size) override {
    size_ = size;
  }

  Size GetSize() const override {
    return size_;
  }

  void SetTexture(std::shared_ptr<ITexture> texture) override {
    texture_ = texture;
  }

  ITexture *GetTexture() const override {
    return texture_.get();
  }

protected:
  Point high_left_end_;
  Size size_;
  std::shared_ptr<ITexture> texture_;
};

bool IsInImageBounds(const Image &image, const Point &point) {
  if (image.size() > point.y && image[point.y].size() > point.x) {
    return true;
  } else {
    return false;
  }
}

class Rectangle : public RectangleLimitedShapes {
public:

  Rectangle() : RectangleLimitedShapes() {}

  Rectangle(const Rectangle &rectangle) : RectangleLimitedShapes(rectangle.high_left_end_, rectangle.size_,
                                                                 rectangle.texture_) {}

  std::unique_ptr<IShape> Clone() const override {
    return make_unique<Rectangle>(Rectangle(*this));
  }

  void Draw(Image &image) const override {
    for (int x = high_left_end_.x; x < high_left_end_.x + size_.width; ++x) {
      for (int y = high_left_end_.y; y < high_left_end_.y + size_.height; ++y) {
        if (IsInImageBounds(image, Point{x, y})) {
          if (texture_ && IsInImageBounds(texture_->GetImage(), Point{x - high_left_end_.x, y - high_left_end_.y})) {
            image[y][x] = texture_->GetImage()[y - high_left_end_.y][x - high_left_end_.x];
          } else {
            image[y][x] = '.';
          }
        }
      }
    }
  }

};

class Ellipse : public RectangleLimitedShapes {
public:

  Ellipse() : RectangleLimitedShapes() {}

  Ellipse(const Ellipse &ellipse) : RectangleLimitedShapes(ellipse.high_left_end_, ellipse.size_,
                                                           ellipse.texture_) {}

  std::unique_ptr<IShape> Clone() const override {
    return make_unique<Ellipse>(Ellipse(*this));
  }

  void Draw(Image &image) const override {
    for (int x = high_left_end_.x; x < high_left_end_.x + size_.width; ++x) {
      for (int y = high_left_end_.y; y < high_left_end_.y + size_.height; ++y) {
        if (IsInImageBounds(image, Point{x, y}) &&
            IsPointInEllipse(Point{x - high_left_end_.x, y - high_left_end_.y}, size_)) {
          if (texture_ && IsInImageBounds(texture_->GetImage(), Point{x - high_left_end_.x, y - high_left_end_.y})) {
            image[y][x] = texture_->GetImage()[y - high_left_end_.y][x - high_left_end_.x];
          } else {
            image[y][x] = '.';
          }
        }
      }
    }
  }

};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
  switch (shape_type) {
    case ShapeType::Rectangle:
      return make_unique<Rectangle>();
    case ShapeType::Ellipse:
      return make_unique<Ellipse>();
    default:
      return nullptr;
  }
}