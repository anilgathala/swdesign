// strategy design pattern (dynamic strategy)
// The behavior of a class/object can be changed dynamically
//

#include <iostream>
#include <string>
#include <vector>

class IMove {
public:
    virtual void move() = 0;
};

class Crawler: public IMove {
public:
    void move() {
        std::cout << "move slow - craaaawler!" << std::endl;
    }
};

class Runner: public IMove {
public:
    void move() {
        std::cout << "move fast!" << std::endl;
    }
};

class Hopper: public IMove {
public:
    void move() {
        std::cout << "move... move... move..." << std::endl;
    }
};

class Robo {
private:
    IMove* move_strategy_;
public:
    Robo (IMove* move_strategy): move_strategy_(move_strategy) {}
    void set_strategy(IMove* move_strategy) { 
        move_strategy_ = move_strategy;
    }
    void move() {
        move_strategy_->move();
    }
};

void client_code(std::vector<Robo*> robo_lst)
{
    for (auto x: robo_lst) {
        x->move();
    }
}

int main()
{
    std::vector<Robo*> robo_lst;

    robo_lst.push_back(new Robo(new Crawler()));
    robo_lst.push_back(new Robo(new Runner()));
    robo_lst.push_back(new Robo(new Hopper()));
    client_code(robo_lst);

    std::cout << "******* dynamic strategy: " << std::endl;
    Robo* infant = new Robo(new Crawler());
    std::cout << " infant crawls: ";
    infant->move();
    std::cout << " after a few years.. " << std::endl;
    Robo* grown_up = infant;
    std::cout << " grown up moves: ";
    grown_up->set_strategy(new Hopper());
    grown_up->move();
}