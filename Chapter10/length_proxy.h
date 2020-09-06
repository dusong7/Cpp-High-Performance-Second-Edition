#pragma once

#include <string>
#include <cmath>

class LengthProxy {
public:
  LengthProxy(float x, float y) : squared_{x * x + y * y} {}
  bool operator==(const LengthProxy& other) const = default;
  auto operator<=>(const LengthProxy& other) const = default;
  auto operator<=>(float len) const { 
    return squared_ <=> len*len; 
  }

  // Implicit cast to float
  operator float() const { return std::sqrt(squared_); }

private:
  float squared_{};
};


class Vec2D {
public:
  Vec2D() = default;
  Vec2D(float x, float y) : x_{x}, y_{y} {}
  auto length_slow() const {
    auto squared = x_ * x_ + y_ * y_;
    return std::sqrt(squared);
  }
  auto length_squared() const {
    auto squared = x_ * x_ + y_ * y_;
    return squared;
  }
  auto length() const {
    return LengthProxy{x_, y_};
  }

  float x_{};
  float y_{};
};


// Non-optimized version for benchmarking
class Vec2DSlow {
public:
  Vec2DSlow() = default;
  Vec2DSlow(float x, float y) : x_{x}, y_{y} {}
  auto length() const {
    auto squared = x_ * x_ + y_ * y_;
    return std::sqrt(squared);
  }
  float x_{};
  float y_{};
};
