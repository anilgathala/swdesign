#include <iostream>


///
/// Liskov principle
///

// bad design:
// have socket inherit from file
// this works as long as only read/write are used
// if a client_fn() uses "seek", then socket would fail

// good deisgn
// unix socket and file inherit from the same base class
// everything_is_a_file
// - this supports only read, write
// - doesn't support lseek


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
    virtual void expand(double scale_factor) = 0;
};

class Rectangle: public IShape {
public:
    Rectangle(int h, int w): height_(h), width_(w) {};
    double get_area() override {
        return (height_ * width_);
    }
    void draw() override {
        std::cout << "drawing rectangle.." << std::endl;
    }
    void expand(double scale_factor) override {
        set_height(height_ * scale_factor);
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

class Square: public IShape {
public:
    Square(int side): side_(side) {};
    double get_area() override {
        return side_ * side_;
    }
    void draw() override {
        std::cout << "drawing square.." << std::endl;
    }

    void expand(double scale_factor) override {
        side_ *= scale_factor;
    }

    void set_side (int s) {
        side_ = s;
    }

    int get_side() {
        return side_;
    }

private:
    int side_;
};

void expand_and_draw(IShape & s, int scale_factor = 2) {
    s.expand(scale_factor);
    s.draw();
}



int main()
{
    std::cout << "Liskov Principle!" << std::endl;
    Rectangle r1(10, 12);
    Square s1(10);

    expand_and_draw(r1);
    expand_and_draw(s1);
    
    return 0;
}
