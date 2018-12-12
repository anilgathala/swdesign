// memento design pattern
// This pattern is used to restore state

#include <iostream>
#include <string>
#include <vector>

class Change {
public:
    Change(): filename("dummy"), num_lines_changed(0) {}
    Change(std::string file, int lines): filename(file), num_lines_changed(lines) {}
    std::string filename;
    int num_lines_changed;
};

class Memento {
public:
    Change change_;
    int64_t timestamp_;
    Memento(Change change, int64_t timestamp): change_(change), timestamp_(timestamp) {} 

    void set_change(Change c) {
        change_ = c;
    }
    Change get_change() {
        return change_;
    }
};

// tracks current change, until it is commited
class Originator {
private:
    Change change_;
public:

    void set_change(Change c) {
        change_ = c;
    }
    
    Change get_change() {
        return change_;
    }

    Memento* save_change_to_memento() {
        int64_t current_ts = 12234543;
        return new Memento(change_, current_ts);
    }

    Change get_change_from_memento(Memento* m) {
        change_ = m->get_change();
        return m->get_change();
    }

    void update_change(Memento* m) {
        change_ = m->get_change();
    }
};

// maintains list of all commited changes / mementos
// user can revert to any older version of the 
class CareTaker {
public:
    std::vector<Memento*> memento_list_;
    void add(Memento* m) {
        memento_list_.push_back(m);
    }

    Memento* get(int index) {
        return memento_list_[index];
    }

    void reset_head(int index) {
        // reset head to a prior change
        memento_list_.resize(index+1);
    }

    void display() 
    {
        for (int i = 0; i < memento_list_.size(); i++) {
            std::cout << "[" << i << "]: " << memento_list_[i]->get_change().filename << std::endl;
        }
    }
};

// mini Git
class Repo {
private:
    // originator stores the current change that is "added"
    Originator originator_;
    // stores list of all "commited" changes
    // a change once commit'd will get into this list
    CareTaker caretaker_;
public:
    void add(std::string filename, int num_lines_modified) {
        originator_.set_change(Change(filename, num_lines_modified));
    }

    void commit() {
        caretaker_.add(originator_.save_change_to_memento());
    }

    void log() {
        caretaker_.display();
    }

    void reset_head(int index) {
        caretaker_.reset_head(index);
        originator_.update_change(caretaker_.get(index));
    }
};


int main()
{
    Repo git;

    git.add("main.cpp", 12);
    git.commit();
    git.add("main.cpp", 16);
    git.commit();
    std::cout << " log (initial): " << std::endl;
    git.log();

    // go back to an older change
    git.reset_head(0);
    std::cout << " log (after reset head to index 0): " << std::endl;
    git.log();

    

    return 0;
}