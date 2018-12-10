// Interpreter design pattern
// This pattern is used to interpret expressions (of arithmetic or of grammer different kind)
// Ideally, we should have an arithmetic expression, but we here pick a simpler example - to support our laziness
//

// we are evaluating candidates for sports teams
// A.height >= 6 (ft) && A.weight <= 175 (lbs) -- soccer
// A.height <= 5.5 (ft) && A.weifght <= 150 (lbs) -- wrestling


#include <string>
#include <iostream>
#include <vector>

const double min_ht = 5.0;
const double min_wt = 120;

class Candidate {
public:
    Candidate(std::string name, double ht, double wt): name_(name), height_(ht), weight_(wt) {}
    Candidate(const Candidate& other): name_(other.name_), height_(other.height_), weight_(other.weight_) {}
    std::string name_;
    double height_;
    double weight_;
    Candidate(Candidate&& other): name_(other.name_), height_(other.height_), weight_(other.weight_) {}
    Candidate& operator=(const Candidate& other){
         name_ = other.name_;
         height_ = other.height_;
         weight_ = other.weight_;
         return *this;
    }
};

class ISelectionCreterion {
public:
    virtual bool interpret(Candidate & c) = 0;
};

class Soccer: public ISelectionCreterion {
private:
    double height_theshold_;
    double weight_threshold_;
public:
    Soccer(double ht, double wt): height_theshold_(ht), weight_threshold_(wt) {}
    bool interpret(Candidate & c) {
        if (c.height_ <= height_theshold_ && c.height_ >= min_ht && c.weight_ <= weight_threshold_) {
            return true;
        }
        return false;
    }
};

class Wrestling: public ISelectionCreterion {
private:
    double height_theshold_;
    double weight_threshold_;
public:
    Wrestling(double ht, double wt): height_theshold_(ht), weight_threshold_(wt) {}
    bool interpret(Candidate & c) {
        if (c.height_ <= height_theshold_ && c.height_ >= min_ht && c.weight_ <= weight_threshold_ && c.weight_ >= min_wt) {
            return true;
        }
        return false;
    }
};

class Anysport: public ISelectionCreterion {
private:
    std::vector<ISelectionCreterion*> checks;
public:
    void add_check(ISelectionCreterion* chk) {
        checks.push_back(chk);
    }

    bool interpret(Candidate & c) {
        for (auto chk: checks) {
            if (!chk->interpret(c)) {
                return false;
            }
        }
        return true;
    }

};


int main()
{
    Soccer soccer_check(6.5, 175.0);
    Wrestling wrestling_check(5.5, 160);
    Anysport anysport_check;
    anysport_check.add_check(&soccer_check);
    anysport_check.add_check(&wrestling_check);

    std::vector<Candidate> candidates;



    candidates.push_back(Candidate("John", 5.11, 155.0));
    // emplace_back is not available in c++11 sorry :-) (only in c++17)
    candidates.push_back(Candidate("Brad", 6.0, 163.0)); 
    candidates.push_back(Candidate("Ryan", 6.2, 191.0));

    for (auto c:  candidates) {
        std::cout << c.name_ << " can potentially be a great fit for soccer: " << soccer_check.interpret(c) << std::endl;
        std::cout << c.name_ << " can potentially be a great fit for wrestling: " << wrestling_check.interpret(c) << std::endl;
        std::cout << c.name_ << " can potentially be a great fit for anysport: " << anysport_check.interpret(c) << std::endl;
    }
    return 0;
}