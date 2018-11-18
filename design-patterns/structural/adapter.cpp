// Adapter design pattern [principle #1]
// client code has an interface, but the impl-class available has a slightly different interface
// well, write an adapter class which provides required interface and uses available impl-class underneath

#include <iostream>

// this is the interface that the client has
class IProfiler {
public:
    virtual void report_cpu_load(double load) = 0;
};

class LibPorfiler {
public:
    void report_cpu_load(uint64_t timestamp, double load) {
        std::cout << "reporting " << " ts: " << timestamp << ", cpu load: " << load << std::endl;
    }
};

class AdapterPorfiler: public IProfiler {
public:
    void report_cpu_load(double load) {
        uint64_t timestamp = 2334455;
        lib_profiler_.report_cpu_load(timestamp, load);
    }
private:
    LibPorfiler lib_profiler_;
};

// this code cannot use LibProfiler directly (incompatiability)
void client_code(IProfiler* prof)
{
    // sleep 10 seconds
    // wake up and check cpu load, report
    double current_cpu_load = 0.88;
    prof->report_cpu_load(current_cpu_load);
}


int main()
{
    IProfiler *prof = new AdapterPorfiler();
    client_code(prof);

    return 0;
}