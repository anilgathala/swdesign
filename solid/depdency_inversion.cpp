#include <iostream>
#include <assert.h>

class chip8224 {
public:
    int copy_to_hw(char* buf, size_t size) {
        std::cout << "8224: copied data to hw" << std::endl;
        return 0;
    }

    int copy_from_hw(char* buf, size_t size) {
        std::cout << "8224: copied data from hw" << std::endl;
        return 0;
    }

};

class WiFiDriver {
public:
    int send(char *buf, size_t size) {
        return hw_chip_.copy_to_hw(buf, size);
    }
    int recv(char *buf, size_t size) {
        return hw_chip_.copy_from_hw(buf, size);
    }
private:
    // this is pretty bad, depdending in low-level components
    chip8224 hw_chip_;
};

int main() {
    char buf[1024];
    WiFiDriver driver;
    driver.send(buf, 1024);
    //...
    driver.recv(buf, 1024);
    return 0;
}