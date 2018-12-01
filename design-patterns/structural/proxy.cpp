// Proxy design pattern
// In proxy pattern, a class represents functionality of another class.
// This type of design pattern comes under structural pattern.
// In proxy pattern, we create object having original object to interface its functionality to outer world.
//

#include <iostream>
#include <string>

class IService {
public:
    virtual int request(std::string query, std::string& response) = 0;
};

class DatabaseService: public IService {
public:
    int request(std::string query, std::string& response) {
        std::cout << " processing query: " << query << std::endl;
        response = "SUCCESS";
        return 0;
    }
};

// a proxy class with more capability while maintaining same API
// - essentially a wrapper around the original class with more functionality
class SecureDatabaseService: public IService {
private:
    std::string credentials_;
    DatabaseService db_;
public:
    SecureDatabaseService(std::string credentials): credentials_(credentials) {}
    int request(std::string query, std::string& response) {
        if (credentials_ == "gooduser") {
            return db_.request(query, response);
        }
        return -1;
    }
};

// this also works!
// void client_code(IService& service) {
//     std::string response;
//     service.request("select * from goldmines;", response);
//     return;
// }

void client_code(IService* service) {
    std::string response;
    service->request("select * from goldmines;", response);
    return;
}


int main()
{
    std::cout << " database query: " << std::endl;
    DatabaseService db;
    client_code(&db);
    // client_code(db);

    // secure
    std::cout << " secure database query: " << std::endl;
    SecureDatabaseService secure_db("gooduser");
    client_code(&secure_db);
    //client_code(secure_db);


    return 0;
}