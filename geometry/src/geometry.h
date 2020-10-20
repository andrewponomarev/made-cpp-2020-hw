#pragma once
#include <cmath>
#include <vector>
#include <iostream>

const double EPS = 1e-10;

struct Point {
  double x, y;

  Point () : x(0), y(0) {};

  Point(double x, double y) : x(x), y(y) {};

  bool operator== (const Point& another) const {
    return this->equals(another);
  }

  bool operator!= (const Point& another) const {
    return !this->equals(another);
  }

 private:
  bool equals (const Point& p) const {
    return fabs(this->x - p.x) < EPS && fabs(this->y - p.y) < EPS;
  }
};

struct Line {
  double k, b;

  Line(const Point& p1, const Point& p2) {
    this->k = (p1.y - p2.y) / (p1.x - p2.x);
    this->b = (p1.x * p2.y - p2.x * p1.y) / (p1.x - p2.x);
  }

  Line (double k, double b) {
    this->k = k;
    this->b = b;
  }

  Line (Point p, double k) {
    this->k = k;
    this->b = p.y - this->k * p.x;
  }

  bool operator== (const Line& another) const {
    return this->equals(another );
  }

  bool operator!= (const Line& another) const {
    return !this->equals(another);
  }

 private:
  bool equals (const Line& p) const {
    return fabs(this->k - p.k) < EPS && fabs(this->b - p.b) < EPS;
  }
};

double distance(const Point& a, const Point& b);

class Shape {
 public:
  virtual double perimeter () const = 0;
  virtual double area () const = 0;

  virtual void rotate (Point center, double angle) = 0;
  virtual void reflex (Point center) = 0;
  virtual void reflex (Line axis) = 0;
  virtual void scale (Point center, double coefficient) = 0;

  bool operator==(const Shape& another) {
    return this->equals(another);
  }
  bool operator!=(const Shape& another) {
    return !this->equals(another);
  }

 protected:
  virtual bool equals (const Shape& another) = 0;
};

class Polygon : public Shape {
 protected:
  std::vector<Point> vertices;

 public:
  Polygon (std::vector<Point> points) {
    this->vertices = points;
  }

  unsigned verticesCount () const {
    return this->vertices.size();
  }

  std::vector<Point> getVertices() const {
    std::vector<Point> v(this->vertices);
    return v;
  }

  double perimeter() const override {
    double p = 0;
    for (auto i = 0; i < this->verticesCount(); ++i) {
      p += distance(this->vertices[i], this->vertices[(i + 1) % this->verticesCount()]);
    }
    return p;
  }

  double area () const override {
    double a = 0;
    for (int i = 1; i < this->verticesCount() - 1; ++i) {
      a += (1. / 2) * fabs((this->vertices[i].x - this->vertices[0].x) *
          (this->vertices[i + 1].y - this->vertices[0].y) -
          (this->vertices[i + 1].x - this->vertices[0].x) *
              (this->vertices[i].y - this->vertices[0].y));
    }
    return a;
  }

  void rotate (Point center, double angle) override {
    double x1, y1;
    for (auto i = 0; i < this->verticesCount(); ++i) {
      vertices[i].x -= center.x;
      vertices[i].y -= center.y;
    }
    for (auto i = 0; i < this->verticesCount(); ++i) {
      x1 = vertices[i].x * std::cos(angle) - vertices[i].y * std::sin(angle);
      y1 = vertices[i].x * std::sin(angle) + vertices[i].y * std::cos(angle);
      vertices[i].x = x1;
      vertices[i].y = y1;
    }
    for (auto i = 0; i < this->verticesCount(); ++i) {
      vertices[i].x += center.x;
      vertices[i].x += center.y;
    }
  }

  void reflex (Point center) override {
    this->scale(center, 1.);
  }

  void reflex (Line axis) override{
    double k = axis.k;
    double b = axis.b;
    double t;
    double x1, y1;
    for (unsigned i = 0; i < this->verticesCount(); ++i) {
      t =  this->vertices[i].y - k * this->vertices[i].x;
      y1 = (k * b - t) / (1 + k * k);
      x1 = (y1 - t) / k;
      this->vertices[i].x = 2 * x1 - this->vertices[i].x;
      this->vertices[i].y = 2 * y1 - this->vertices[i].y;
    }
  }

  void scale (Point center, double coefficient) override {
    for (size_t i = 0; i < this->verticesCount(); i++) {
      this->vertices[i].x = center.x + coefficient * (this->vertices[i].x - center.x);
      this->vertices[i].y = center.y + coefficient * (this->vertices[i].y - center.y);
    }
  }

 protected:
  bool equals (const Shape& another) override {
    const Polygon* a = dynamic_cast<const Polygon*>(&another);
    if (this->verticesCount() != a->verticesCount()) {
      return false;
    }
    unsigned size = this->verticesCount();
    unsigned i = 0;
    while (i < size && this->vertices[0] != a->vertices[i]) {
      i++;
    }
    if (i == size) {
      return false;
    }
    for (auto j = 0; j < size; ++j) {
      if (this->vertices[j] != a->vertices[(i + j) % size] &&
          this->vertices[j] != a->vertices[(i - j + size) % size]) {
        return false;
      }
    }
    return true;
  }
};

class Ellipse : public Shape {
 protected:
  Point f1, f2;
  double r;

 public:
  Ellipse(Point f1, Point f2, double r) : f1(f1), f2(f2), r(r) {};

  std::pair<Point, Point> focuses() const {
    return std::make_pair(f1, f2);
  };

  double eccentricity() const {
    return distance(this->f1, this->f2) / this->r;
  };

  double radius() const {
    return this->r / 2;
  }

  Point center() const {
    Point c((this->f1.x + this->f2.x) / 2, (this->f1.y + this->f2.y) / 2);
    return c;
  };

  double perimeter() const override {
    double a = this->r / 2;
    return 4 * a * std::comp_ellint_2(eccentricity());
  };

  double area() const override {
    double dist = distance(this->f1, this->f2);
    double b = std::sqrt(this->r *  this->r - dist * dist);
    return M_PI * this->r * b / 4;
  };

  void rotate(Point center, double angle) override {
    this->f1.x = center.x + (this->f1.x - center.x) * cos(angle) - (this->f1.y - center.y) * sin(angle);
    this->f1.y = center.y + (this->f1.y - center.y) * cos(angle) + (this->f1.x - center.x) * sin(angle);
    this->f2.x = center.x + (this->f2.x - center.x) * cos(angle) - (this->f2.y - center.y) * sin(angle);
    this->f2.y = center.y + (this->f2.y - center.y) * cos(angle) + (this->f2.x - center.x) * sin(angle);
  };

  void reflex(Point center) override {
    this->scale(center, 1.);
  };

  void reflex(Line axis) override {
    double t;
    double x1, y1;
    t = this->f1.y - axis.k * this->f1.x;
    y1 = (axis.k * axis.b - t) / (1 + axis.k * axis.k);
    x1 = (y1 - t) / axis.k;
    this->f1.x = 2 * x1 - this->f1.x;
    this->f1.y = 2 * y1 - this->f1.y;

    t = this->f2.y - axis.k * this->f2.x;
    y1 = (axis.k * axis.b - t) / (1 + axis.k * axis.k);
    x1 = (y1 - t) / axis.k;
    this->f2.x = 2 * x1 - this->f2.x;
    this->f2.y = 2 * y1 - this->f2.y;
  };

  void scale(Point center, double coefficient) override {
    this->f1.x = (this->f1.x - center.x) * coefficient + center.x;
    this->f1.y = (this->f1.y - center.y) * coefficient + center.y;
    this->f2.x = (this->f2.x - center.x) * coefficient + center.x;
    this->f2.y = (this->f2.y - center.y) * coefficient + center.y;
    this->r *= coefficient;
  };

 protected:
  bool equals(const Shape& another) override {
    const Ellipse* a = dynamic_cast<const Ellipse*>(&another);
    if (((this->f1 == a->f1) && (this->f2 == a->f2)) && (fabs(this->r - a->r) < EPS))
      return true;
    if (((this->f1 == a->f2) && (this->f2 == a->f1)) && (fabs(this->r - a->r) < EPS))
      return true;
    return false;
  }
};

class Circle : public Ellipse {
 public:
  Circle(Point center, double radius) : Ellipse(center, center, 2 * radius) {};
};

class Rectangle : public Polygon {

  std::vector<Point> calcVertices(Point p1, Point p3, double k) {
    k = std::max(k, 1 / k);
    double x3 = p3.x - p1.x;
    double y3 = p3.y - p1.y;
    double angle = std::atan(k);
    double x2 = x3 * std::cos(angle) - y3 * std::sin(angle);
    double y2 = x3 * std::sin(angle) + y3 * std::cos(angle);
    x2 /= k * k + 1;
    y2 /= k * k + 1;
    x2 += p1.x;
    y2 += p1.y;
    Point p2(x2, y2);
    Point p4(p3.x + p1.x - x2, p3.y + p1.y - y2);
    std::vector<Point> res = {p1, p2, p3, p4};
    return res;
 }

 public:
  Rectangle(Point p1, Point p3, double k) : Polygon(calcVertices(p1, p3, k)) {};

  Point center() {
    Point c((this->getVertices()[0].x + this->getVertices()[2].x) / 2,
              (this->getVertices()[0].y + this->getVertices()[2].y) / 2);
    return c;
  }

  std::pair<Line, Line> diagonals() {
    Line d1 = Line(this->getVertices()[0], this->getVertices()[2]);
    Line d2 = Line(this->getVertices()[1], this->getVertices()[3]);
    return std::make_pair(d1, d2);
  }

};

class Square : public Rectangle {
 public:
  Square(Point p1, Point p3) : Rectangle(p1, p3, 1) {};

  Circle circumscribedCircle() {
    double r = std::sqrt(2) * this->perimeter() / 4;
    Circle c(this->center(), r);
    return c;
  };

  Circle inscribedCircle() {
    Circle c(this->center(), this->perimeter() / 4);
    return c;
  };
};

class Triangle : public Polygon {

 public:
  Triangle(const Point& p1, const Point& p2, const Point& p3) : Polygon({p1, p2, p3}) {};

  Circle circumscribedCircle() {
    double a = distance(this->vertices[0], this->vertices[1]);
    double b = distance(this->vertices[1], this->vertices[2]);
    double c = distance(this->vertices[2], this->vertices[0]);
    double r = a * b * c / (4 * this->area());
    double d = 2 * (this->vertices[0].x * (this->vertices[1].y - this->vertices[2].y) +
        this->vertices[1].x * (this->vertices[2].y - this->vertices[0].y) +
        this->vertices[2].x * (this->vertices[0].y - this->vertices[1].y));
    double x = 0;
    double y = 0;
    for (size_t i = 0; i < 3; ++i) {
      x += (this->vertices[i].x * this->vertices[i].x + this->vertices[i].y * this->vertices[i].y) *
          (this->vertices[(i + 1) % 3].y - this->vertices[(i + 2) % 3].y) / d;
      y += (this->vertices[i].x * this->vertices[i].x + this->vertices[i].y * this->vertices[i].y) *
          (this->vertices[(i + 2) % 3].x - this->vertices[(i + 1) % 3].x) / d;
    }
    Point center(x, y);
    Circle circle(center, r);
    return circle;
  };

  Circle inscribedCircle() {
    double r = 2 * area() / perimeter();
    double b = distance(this->vertices[1], this->vertices[2]);
    double c = distance(this->vertices[2], this->vertices[0]);
    double t = perimeter() / 2 - b;
    Point g(this->vertices[0].x + t * (this->vertices[2].x - this->vertices[0].x) / c,
               this->vertices[0].y + t * (this->vertices[2].y - this->vertices[0].y) / c);
    Line ac(this->vertices[0], this->vertices[2]);
    double k = -1.0 / ac.k;
    double x, y;
    if (g.x <= this->vertices[1].x) {
      x = g.x + r / sqrt(1.0 + k * k);
    } else {
      x = g.x - r / sqrt(1.0 + k * k);
    }
    if (g.y <= this->vertices[1].y) {
      y = g.y + k * r / sqrt(1.0 + k * k);
    } else {
      y = g.y - k * r / sqrt(1.0 + k * k);
    }
    Point center(x, y);
    Circle circle(center, r);
    return circle;
  };

  Point centroid() {
    Point a = this->getVertices()[0];
    Point b = this->getVertices()[1];
    Point c = this->getVertices()[2];
    double x = (a.x + b.x + c.x) / 3;
    double y = (a.y + b.y + c.y) / 3;
    Point center(x, y);
    return center;
  };

  Point orthocenter() {
    Point cc = this->circumscribedCircle().center();
    Point oc = cc;
    for (auto i = 0; i < 3; ++i) {
      oc.x += (this->getVertices()[i].x - cc.x);
      oc.y += (this->getVertices()[i].y - cc.y);
    }
    return oc;
  };

  Line EulerLine() {
    Line l(this->orthocenter(), this->centroid());
    return l;
  };

  Circle ninePointsCircle() {
    Point oc = this->orthocenter();
    Point cc = this->circumscribedCircle().center();
    Point center;
    center.x = (oc.x + cc.x) / 2;
    center.y = (oc.y + cc.y) / 2;
    double r = this->circumscribedCircle().radius() / 2;
    Circle circle(center, r);
    return circle;
  };
};

