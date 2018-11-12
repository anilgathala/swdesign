// Prototype design pattern
// when an application needs to create a lot of instances with different preset configurations, it might help
// to create prototypes for each type and cache them. When app needs an instance,
// we can clone from the copy in prototype cache.
//
// -- application would want to create a lot of instances of a particular class (ResponseBuffers for example)
// -- there are a handful of well-defined states a response buffer is in
// -- (Success, OutofMem, CpuOverload, IoOverload, Unknown)
// -- Each response should contain:
//    --- seq num,
//    --- status code,
//    --- failure code in case of failure
//    --- threshold used to decide failure
// -- last three would be same for a particular kind of failure or of success
// -- we can create prototypes for these, cache them and clone them on demand

#include <iostream>
#include <string>
#include <unordered_map>

enum ErrorType
{
    Success = 0,
    OutOfMemory = 1,
    CpuOverload = 2,
    IoOverlaod = 3,
    RougeClient = 4,
    Unknown = 5
};

class ConfigRegistry
{
  public:
    static int get_current_threshold(ErrorType err_code)
    {
        switch (err_code)
        {
        case Success:
            return 0;
        case OutOfMemory:
            // Obtain from current config (entries can change on reboot)
            return 80; // 80%
        case CpuOverload:
            // Obtain from current config file (entries can change on reboot)
            return 75;
        case IoOverlaod:
            // check current config file (entries can change on reboot)
            return 60;
        case RougeClient:
            // check current config file (entries can change on reboot)
            return 50; // saw too many connections open in a time window
        case Unknown:
            // check current config file (entries can change on reboot)
            return 0;
        }
    }
};

class ResponseBuffer
{
  public:
    ResponseBuffer(int seq_num) : seq_num_(seq_num), success_code_(0){};
    void set_failure_code(ErrorType err_code)
    {
        success_code_ = err_code;
        // expensive operation
        failure_threshold_ = ConfigRegistry::get_current_threshold(err_code);
    }

    void set_seq_num(int seq_num)  {
        seq_num_ = seq_num;
    }

    ResponseBuffer* clone() {
        ResponseBuffer* response = new ResponseBuffer(0);
        response->success_code_ = success_code_;
        response->failure_threshold_ = failure_threshold_;
        return response;
    }

    friend std::ostream& operator<<(std::ostream& os, const ResponseBuffer*);

  private:
    int seq_num_;
    int success_code_;
    int failure_threshold_;
};

std::ostream& operator<<(std::ostream& os, const ResponseBuffer* resp)
{
    os << "seq_num: " << resp->seq_num_
        << " failure_code: " << resp->success_code_
        << " failure_threshold: " << resp->failure_threshold_;
    return os;
}

class PrototypeManager {
public:
    PrototypeManager() {
        for (int e = Success; e <= Unknown; e++) {
            cache_[(ErrorType)e] = new ResponseBuffer(0);
            cache_[(ErrorType)e]->set_failure_code((ErrorType)e);
        }
    }
    ResponseBuffer* get_prototype(ErrorType e) {
        return cache_[e]->clone();
    }
private:
    std::unordered_map<int, ResponseBuffer*> cache_;
};


void send_response(int seq_num, ErrorType err_code)
{
    PrototypeManager proto_mgr;
    ResponseBuffer *resp = proto_mgr.get_prototype(err_code);
    resp->set_seq_num(seq_num);

    std::cout << "sending out response: " << resp << std::endl;
}

int main()
{
    send_response(535, Success);
    // ....
    send_response(536, OutOfMemory);
    //...
    send_response(537, RougeClient);
    return 0;
}
