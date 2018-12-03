// observer pattern
// there is a subject and a group of observers who are interested in the updates/state-changes of the subject
// typical pub-sub kind of scenario
//

#include <iostream>
#include <vector>

class IObserver {
public:
    virtual void update(int traffic) = 0;
};

class Throttler: public IObserver {
private:
    int traffic_;
    int threshold_;
public:
    Throttler(int threshold): traffic_(0), threshold_(threshold) {}

    void update(int traffic) {
        if (traffic > threshold_) {
            std::cout << "Throttler: dropping pkts" << std::endl;
        } else {
            std::cout << "Throttler: traffic looks good" << std::endl;
        }
    }
};

class MetricsAgent: public IObserver {
private:
    int threshold_;
public:
    MetricsAgent(int threshold): threshold_(threshold) {}
    void update(int traffic) {
        if (traffic > threshold_) {
            std::cout << "MetricsAgent: pager duty sent out" << std::endl;
        } else {
            std::cout << "MetricsAgent: traffic looks good" << std::endl;
        }
    }
};

class LogFileCleaner: public IObserver {
private:
    int threshold_;
public:
    LogFileCleaner(int threshold): threshold_(threshold) {}
    void update(int traffic) {
        if (traffic > threshold_) {
            std::cout << "LogFileCleaner: deleting 25% oldest log files" << std::endl;
        } else {
            std::cout << "LogFileCleaner: traffic looks good" << std::endl;
        }
    }
};

class TrafficBoard {
private:
    int traffic_;
    int notify_threshold_;
    std::vector<IObserver*> observers;
public:
    TrafficBoard(int threshold): traffic_(0), notify_threshold_(threshold) {}
    void add_traffic(int traffic) {
        traffic_ += traffic;
        if (traffic_ > notify_threshold_) {
            notify();
        }
    }

    void del_traffic(int traffic) {
        traffic_ -= traffic;
    }

    void register_observer(IObserver* obs) {
        if (std::find(observers.begin(), observers.end(), obs) == observers.end()) {
            observers.push_back(obs);
        }
    }

    void notify() {
        for (auto obs: observers) {
            obs->update(traffic_);
        }
    }

};



int main(int argc, char const *argv[])
{
    TrafficBoard board(2);
    Throttler traffic_throttler(5);
    MetricsAgent metrics_agent(8);
    LogFileCleaner log_cleaner(3);

    board.register_observer(&traffic_throttler);
    board.register_observer(&metrics_agent);
    board.register_observer(&log_cleaner);

    board.add_traffic(2);
    board.add_traffic(1);
    board.add_traffic(5);
    board.add_traffic(10);

    board.del_traffic(5);

    board.add_traffic(5);


    

    return 0;
}
