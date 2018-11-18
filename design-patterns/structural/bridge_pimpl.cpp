// Bridge design pattern [principle #1]
// PIMPL:
// "Pointer to implementation" or "pImpl" is a C++ programming technique 
// that removes implementation details of a class from its object representation by placing them 
// in a separate class, accessed through an opaque pointer. [Link: https://en.cppreference.com/w/cpp/language/pimpl]
//

// The benefits of pimpl idiom:
// -- actual implementation (most of it) is hidden behind the pimpl pointer, hidden from the client code (less coupling)
// -- client code doesn't have to include headers needed by implementation (implementation's cpp file can include those headers)
// -- modifying data members of implementation does not impact client binary compatiability

// Example: track api calls and store them for further analysis

#include <iostream>
#include <string>

// header exposed to the client
enum ApiType {
    CameraOn,
    AccessLocation,
    SendOverNetwork
};

// header exposed to the client
class IApiTracker {
public:
    virtual void report(ApiType api, std::string api_string) = 0;
};

// header exposed to the client
class ApiTracker: public IApiTracker {
public:
    class ApiTrackerImpl;
    void report(ApiType api, std::string api_string);
private:
    ApiTrackerImpl *pimpl;
};

// separate cpp file, not exposed to client
// #include <some_heavy_header> --> client code doesn't have to include this
class ApiTracker::ApiTrackerImpl {
public:
    void report(ApiType api, std::string api_string)
    {
        std::cout << " storing api log in DB: " << api_string << std::endl;
    }
};

// class ApiTracker::ApiTrackerImpl {
// public:
//     void report(ApiType api, std::string api_string)
//     {
//         std::cout << " sending api over network: " << api_string << std::endl;
//     }
// };

void ApiTracker::report(ApiType api, std::string api_string) {
        pimpl->report(api, api_string);
}

int main()
{
    // client code just knows the ApiTracker/report() api and is independent of impl details
    ApiTracker api_tracker;
    api_tracker.report(AccessLocation, "api: getLocation, timestamp: 34309565");
    api_tracker.report(AccessLocation, "api: CameraOn, timestamp: 34309799");
    return 0;
}
