// Null Object design pattern
// client code does something like: get an obj of a class and then use it.
// However the object that is "get" upon, may not always exist - in that case,
// rather than passing null or nullptr, we pass a mock/null object which is a no-op class
//


#include <iostream>
#include <sys/types.h>

class IFileEWriter {
public:
    virtual int write(std::string) = 0;

};

// real class
class LogFileWriter: public IFileEWriter {
private:
    int fd;
public:
    LogFileWriter(std::string filename) {
        // open file, update fd
        fd = 10 + (std::rand() / RAND_MAX) * 100;
    }
    int write(std::string) {
        // write buf to fd
        std::cout << "logging on fd = " << fd << std::endl;
        return 0;
    }

};

// mock class
class NullObjectFileWriter: public IFileEWriter {
public:
    NullObjectFileWriter(std::string filename) {
    }
    int write(std::string) {
        std::cout << "skipping logs " << std::endl;
        return 0;
    }
};

IFileEWriter* get_logger()
{
    bool out_of_disk_space = true;
    if (out_of_disk_space) {
        return new NullObjectFileWriter("dummy");
    }
    return new LogFileWriter("/opt/service/log/log.info");
}

// advantages:
// client code doesn't have to check if "logger" is null, as that'll never be the case
// when using this design pattern
void client_code(IFileEWriter* logger)
{
    // ... do something useful
    logger->write("current cpu load: 25%");
}

int main()
{
    IFileEWriter* logger = get_logger();
    client_code(logger);
    return 0;
}