#include <iostream>
#include <assert.h>

class EverythingIsAFile {
public:
    virtual int write(char* buf, size_t size) = 0;
    virtual int read(char* buf, size_t size) = 0;
};

class file_: public EverythingIsAFile {
public:
    file_(const std::string name): name_(name) {};
    int write(char* buf, size_t size) override {
        std::cout << "writing at current offset " << std::endl;
        return 0;
    };

    int read(char* buf, size_t size) override {
        std::cout << "reading at current offset " << std::endl;
        return 0;
    };

    int seek(int offset) {
        std::cout << "updating current offset to " << offset << std::endl;
        return 0;
    }

private:
    std::string name_;
};

// It is incorrect to inherit socket_ from file_
class socket_: public EverythingIsAFile {
public:
    int write(char* buf, size_t size) override {
        std::cout << "sending data out on socket " << std::endl;
        return 0;
    };

    int read(char* buf, size_t size) override {
        std::cout << "recv'ing data on socket " << std::endl;
        return 0;
    };
};

void seek_and_write(file_ f, int offset, char* buf, size_t size)
{
    f.seek(offset);
    f.write(buf, size);
}

void send_on_socket(socket_ s, char* buf, size_t size)
{
    s.write(buf, size);
}

int main() {

    char buf[1024];
    file_ f("test.dat");
    seek_and_write(f, 10, buf, 1024);

    socket_ sk;
    send_on_socket(sk, buf, 1024);

    return 0;
}