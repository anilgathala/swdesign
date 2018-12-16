// tempalte design pattern
// basic inheritance
// compare this with strategy pattern

#include <iostream>
#include <vector>
#include <string>

class IRobo {
public:
    virtual std::string name() = 0;
    virtual void move() = 0;
};

class KidRobo: public IRobo {
private:
    std::string name_;
public:
    KidRobo(std::string name): name_(name) {}
    std::string name() { return name_; }
    void move() {
        std::cout << "craaawling!" << std::endl;
    }
};

class GrownupRobo: public IRobo {
private:
    std::string name_;
public:
    GrownupRobo(std::string name): name_(name) {}
    std::string name() { return name_; }
    void move() {
        std::cout << "Walking!" << std::endl;
    }
};

class TeenagerRobo: public IRobo {
private:
    std::string name_;
public:
    TeenagerRobo(std::string name): name_(name) {}
    std::string name() { return name_; }
    void move() {
        std::cout << "Hoping!" << std::endl;
    }
};

// client code doesn't know specific implementations of behind interfaces
void print(std::vector<IRobo*>& family) {
    for (auto member: family) {
        std::cout << member->name() << " ";
        member->move();
        std::cout << std::endl;
    }
}

int main()
{
    std::vector<IRobo*> family;
    family.push_back(new GrownupRobo("Mom"));
    family.push_back(new GrownupRobo("Dad"));
    family.push_back(new TeenagerRobo("Big Brother"));
    family.push_back(new KidRobo("Toddler"));

    print(family);
}