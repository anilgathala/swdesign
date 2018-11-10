// Abstract Factory pattern
// Factory of factories
// link: https://www.youtube.com/watch?v=BPkYkyVWOaw
//
// when you have many many objects beign created, better to have a factory for each class of objects
// so to allow client code to be generic - get_factory(), factory->get_object(), work with object
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

class IServiceFactory {
public:
    virtual IService* get_object() = 0;
};


class HTTPReqHandlerFactory: public IServiceFactory {
public:
    IService* get_object() {
        return new HTTPReqHandler("www.resources.com/req");
    }
};

class DBServiceFactory: public IServiceFactory {
public:
    IService* get_object() {
        return new DBService("postgres", "guest", "guest");
    }
};

const std::string db_service = "postgres";
const std::string http_req_handler = "rest";

class AbstractServiceFactory {
public:
    static IServiceFactory* get_factory(std::string name) {
        if (name == db_service) {
            return new DBServiceFactory();
        } else if (name == http_req_handler) {
            return new HTTPReqHandlerFactory();
        } else {
            return nullptr;
        }
    }
};

// now this code is completely oblivious of the details of each service / service factory
// **** least coupling ***
void launch_service(IServiceFactory* service_factory)
{
    IService *service = service_factory->get_object();
    service->start();
}

// the client code doesn't have to know the specifics of obj creation
// -- less coupling
// when constructor of DBService changes, client_code doesn't have to
void client_code()
{
    // get factory and pass it to launch_Service (generic code to launch service)
    IServiceFactory* db_factory = AbstractServiceFactory::get_factory(db_service);
    launch_service(db_factory);

    IServiceFactory* req_handler_factory = AbstractServiceFactory::get_factory(http_req_handler);
    launch_service(req_handler_factory);

}

int main()
{
    client_code();
    return 0;
}