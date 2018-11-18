// Bridge design pattern [principle #2]
// -- there are two class hierarchies based on two interfaces
// -- Example:
// ---- Shape (baseclass), circle, rectangle
// ---- Renderer (baseclass), vectorRenderer, RastorizedRenderer
// ---- the draw() of all Shapes need a renderer
// ---- in this pattern, the base class of the user hierarchy (Shape) will
// ------- store a reference to the base class of the second hierarchy
// ---- now we can initialize a shape (rectangle or circle) by passing the right renderer and the code would work
// ---- this way, changes in specific implementation of one hierarchy doesn't impact the other.

#include <iostream>
#include <string>
 
const bool config_debug_on = true;
const int log_prod_level = 1; // only log *one* and below
const int log_staging_level = 2; // only log *two* and below
const int log_dev_level = 3; // only log *one* and below

class ILogger {
public:
    virtual void print(std::string msg, int log_level) = 0;
};

class Logger: public ILogger {
public:
    Logger(int log_level, std::string log_prefix): log_level_(log_level), log_prefix_(log_prefix) {}
     void print(std::string msg, int log_level)
     {
         if (log_level > log_level_) {
             return;
         }
         std::cout << log_prefix_ << ": " << msg << std::endl;
     }
private:
    int log_level_;
    std::string log_prefix_;
};

class IService {
protected:
    ILogger& logger_;
    IService(ILogger& logger): logger_(logger) {}
public:

    virtual std::string handle_request(std::string req) = 0;
    virtual void garbage_collect() = 0;
    virtual void get_metrics() = 0;
};

class ProxyService: public IService {

public:
    ProxyService(ILogger& logger): IService(logger) {}
    std::string handle_request(std::string req) {
        int err_code = 0; //fetch from origin and update err_code
        logger_.print("Proxy: req succeded!", log_dev_level);
        if (err_code < 0) {
            std::string msg = " req failed with err code: " + std::to_string(err_code);
            logger_.print(msg, log_prod_level);
            return "Bad request 400";
        }
        return "OK 200";
    }
    void garbage_collect() {
        logger_.print("Proxy: doing gc!", log_dev_level);
        logger_.print("Proxy: freed 128 KB!", log_dev_level);
    }
    void get_metrics() {
        logger_.print("Proxy: sending metrics!", log_dev_level);
    }
};

class DatabaseService: public IService {
public:
    DatabaseService(ILogger& logger): IService(logger) {}
    std::string handle_request(std::string req) {
        int err_code = 0; // talk to DB and store error code here
        if (err_code < 0) {
            std::string msg = " db connection failed with err code: " + std::to_string(err_code);
            logger_.print(msg, log_prod_level);
            return "SQL DB Connection failed!";
        }
        logger_.print("DBService: sql req succeeded!", log_dev_level);
        return "SQL OK!";
    }

    void garbage_collect() {
        logger_.print("DBService: doing gc!", log_dev_level);
        logger_.print("DBService: freed 8 KB!", log_dev_level);
    }
    void get_metrics() {
        logger_.print("DBService: sending metrics!", log_dev_level);
    }
};

int main()
{
    Logger prod_logger(log_prod_level, "production");
    
    DatabaseService *db;
    ProxyService *proxy;
    // Not a great way to control log level really! (as we cannot control this at run-time)
    // hey, but it works to demonstrate the bridge pattern :-)
    if (config_debug_on) {
        Logger dev_logger(log_dev_level, "dev");
        db = new DatabaseService(dev_logger);
        proxy = new ProxyService(dev_logger);
    } else {
        Logger prod_logger(log_prod_level, "dev");
        db = new DatabaseService(prod_logger);
        proxy = new ProxyService(prod_logger);
    }

    db->handle_request("db request");
    proxy->handle_request("proxy req");
    
    db->garbage_collect();
    db->get_metrics();

    proxy->get_metrics();
    proxy->garbage_collect();

    return 0;
}