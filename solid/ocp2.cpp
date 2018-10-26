#include <iostream>
#include <chrono>
#include <thread>

//
// Open Closed Principle (Example 2)
//

class IShape {
public:
    virtual double area() = 0;
};

class Rectangle: public IShape {
public:
    Rectangle(int h, int w): height_(h), weight_(w)
    {};
    double area() override {
        return (height_ * weight_);
    }
private:
    double height_;
    double weight_;
};

class Square: public IShape {
public:
    Square(int s): side_(s)
    {};
    double area() override {
        return (side_ * side_);
    }
private:
    double side_;
};

class AreaCalculator {
public:
    // OCP: Rather than coding this method for a specific shape
    //      we're coding for all shapes.
    //      Now this class is open for extension but closed for modification
    double calculate_area(IShape& shape) {
        return shape.area();
    }
};

int main()
{
    Rectangle master_bedroom(14, 12);
    Square guest_room(10);

    AreaCalculator calculator;

    double total_area = 0;
    total_area += calculator.calculate_area(master_bedroom);
    total_area += calculator.calculate_area(guest_room);

    std::cout << " total area of the house: " << total_area << std::endl;

    return 0;
}