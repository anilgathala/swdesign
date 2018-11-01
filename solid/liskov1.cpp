#include <iostream>
#include <assert.h>


///
/// Liskov principle
///

// bad design:
// Rectangle with setWidth, getWidth
// Square inherits Rectangle
// client_fn() breaks when you pass Square instead of Rectangle
// as there is an assert in client_fn() to check area right after setWidth and getWidth

// a solution:
// have base class / interface Shape with minimal stuff in it - getArea(); Draw()
// have both Rectangle and Square derive from it
// as behavior wise Rectangle and Square are different & do not share a "is a" relationship due to behavior

class IShape {
public:
    virtual double get_area() = 0;
    virtual void draw() = 0;
};

class Rectangle: public IShape {
public:
    Rectangle(int h, int w): height_(h), width_(w) {};
    double get_area() override {
        return (height_ * width_);
    }
    virtual void draw() override {
        std::cout << "drawing rectangle.." << std::endl;
    }
    void set_height(int h) {
        height_ = h;
    }
    void set_width(int w) {
        width_ = w;
    }

    int get_height() {  return height_; }
    int get_width() {  return width_; }
public:
    int height_;
    int width_;
};

class BadSquare: public Rectangle {
public:
    BadSquare(int side): Rectangle(side, side) {};
};

void expand_and_draw(Rectangle& r, int scale_factor = 2) {
    /// as you can notice this cannot take a "badSuqare" obj, the assert will fail
    int old_height = r.get_height();
    int old_width = r.get_width();

    r.set_height(scale_factor * old_height);
    r.draw();

    std::cout << "old_height: " << old_height << " old_width: " << old_width << std::endl;
    std::cout << "h = " << r.get_height() << " w = " << r.get_width() << std::endl;

    assert(r.get_height() == (old_height * scale_factor));
    assert(r.get_width() == old_width);

    std::cout << "after assert!" << std::endl;

     // reset it
    // r.set_height(old_height);
}

int main()
{
    std::cout << "Liskov Principle!" << std::endl;
    Rectangle r1(10, 12);
    BadSquare s1(10);

    expand_and_draw(r1);
    expand_and_draw(s1);

    // expansion breaks basic property of square
    assert(s1.get_height() == s1.get_width());

    // liskov2.cpp for better design

    return 0;
}