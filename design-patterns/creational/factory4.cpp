// Factory pattern
// Principle #3: when you have an interface and 2 or more base classes, having a factory
//               to create these objects allows for additional decopling between client class
//               and these different base classes
//
// link: https://www.youtube.com/watch?v=BPkYkyVWOaw
//

#include <iostream>
#include <string>

class IService {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};

class DBService: public IService {
public:

    DBService(std::string db_name, std::string login, std::string key) {}

    void start() {
        std::cout << "connected to postgres sql..." << std::endl;
    }

    void stop() {
        std::cout << "postgres connection closed " << std::endl;
    }

    std::string sql_request(std::string sql_query) {
        std::cout << "querying postgres " << std::endl;
        return "RESULTS!";
    }
};

class HTTPReqHandler: public IService {
public:

    HTTPReqHandler(std::string rest_end_point) {}

    void start() {
        std::cout << "rest end pt up..." << std::endl;
    }

    void stop() {
        std::cout << "rest end pt shutdown" << std::endl;
    }
};

class ServiceFactory {
public:
    static HTTPReqHandler* get_http_req_handler() {
        return new HTTPReqHandler("www.resources.com/req");
    }

    static DBService* get_db_service() {
        return new DBService("postgres", "guest", "guest");
    }
};

// the client code doesn't have to know the specifics of obj creation
// -- less coupling
// when constructor of DBService changes, client_code doesn't have to
void client_code()
{
    // start services
    IService* db_service = ServiceFactory::get_db_service();
    db_service->start();

    IService* req_handler = ServiceFactory::get_http_req_handler();
    req_handler->start();

    //...

}

int main()
{
    client_code();
    return 0;
}