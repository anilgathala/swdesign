// State design pattern
// Behavior of a class changes based on its state
//

#include <iostream>
#include <unordered_map>

const int pkt_size = 1024;

enum Priority {
    LOW = 0,
    MED = 1,
    HIGH = 2
};

class Stream {
public:
    Stream(int num, Priority p): num_(num), priority_(p) {}
    int num_; // number of packets
    Priority priority_;
};

class TrafficTracker {
private:
    int in_mem_pkts_;
    int threshold_;
public:
    TrafficTracker(int threshold): in_mem_pkts_(0), threshold_(threshold) {}
    void add_traffic(int num) {
        in_mem_pkts_ += num;
    }
    void del_traffic(int num) {
        if (num <= in_mem_pkts_) {
            in_mem_pkts_ -= num;
        } else {
            in_mem_pkts_ = 0;
        }
    }

    int get_in_mem_traffic() {
        return in_mem_pkts_;
    }

    bool low_mem()
    {
        if (in_mem_pkts_ >= threshold_) {
            return true;
        }
        return false;
    }
};

class ITrafficHandler {
public:
    virtual void handle(Stream scalbnf) = 0;
};

class LowMemTrafficHandler: public ITrafficHandler {
private:
    TrafficTracker* tracker_;
public:
    LowMemTrafficHandler(TrafficTracker* tracker): tracker_(tracker) {}
    void handle(Stream s) {
        if (s.priority_ >= HIGH) {
            std::cout << "LowMemTrafficHandler: processing " << s.num_ << " HIGH prio pkts" << std::endl;
            tracker_->del_traffic(s.num_);
        } else {
            std::cout << "LowMemTrafficHandler: skipping " << s.num_ << " LOW/MED prio pkts" << std::endl;
        }
    }
};

class HighMemTrafficHandler: public ITrafficHandler {
private:
    TrafficTracker* tracker_;
public:
    HighMemTrafficHandler(TrafficTracker* tracker): tracker_(tracker) {}
    void handle(Stream s) {
        if (s.priority_ >= LOW) {
            std::cout << "HighMemTrafficHandler: processing " << s.num_ << " pkts" << std::endl;
            tracker_->del_traffic(s.num_);
        }
    }
};

class StateMachine {
private:
    // above threshold = true/false
    // true, LowMemTrafficHandler
    // false, HighMemTrafficHandler
    TrafficTracker* tracker_;
    LowMemTrafficHandler* low_;
    HighMemTrafficHandler* high_;
    std::unordered_map<bool, ITrafficHandler*> state_machine_;
public:
    StateMachine() {
        tracker_ = new TrafficTracker(10);
        low_ = new LowMemTrafficHandler(tracker_);
        high_ = new HighMemTrafficHandler(tracker_);
        state_machine_[true] = low_;
        state_machine_[false] = high_;
    }

    TrafficTracker* get_tracker() {
        return tracker_;
    }

    ITrafficHandler* get_current_state(bool traffic_high) {
        // note: it doesn't matter what the current state is, in this specific example
        return state_machine_[traffic_high];
    }

};

void generic_process_traffic(Stream s, StateMachine* machine) {
    auto tracker = machine->get_tracker();
    tracker->add_traffic(s.num_);

    // change state based on current input / stream
    auto current = machine->get_current_state(tracker->low_mem());

    // execute current state for the input
    current->handle(s);
}

int main()
{
    StateMachine *machine = new StateMachine();
    generic_process_traffic(Stream(5, LOW), machine);
    generic_process_traffic(Stream(15, LOW), machine);
    generic_process_traffic(Stream(25, HIGH), machine);
    generic_process_traffic(Stream(11, MED), machine);
    return 0;
}