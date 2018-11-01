#include <iostream>
#include <assert.h>

const std::string chip_number = "8224";

class IChip {
public:
    virtual int copy_to_hw(char* buf, size_t size) = 0;
    virtual int copy_from_hw(char* buf, size_t size) = 0;
};

class chip8224: public IChip {
public:
    int copy_to_hw(char* buf, size_t size) override {
        std::cout << "8224: copied data to hw" << std::endl;
        return 0;
    }

    int copy_from_hw(char* buf, size_t size) override {
        std::cout << "8224: copied data from hw" << std::endl;
        return 0;
    }
};

class chip8226: public IChip {
public:
    int copy_to_hw(char* buf, size_t size) override {
        std::cout << "8226: copied data to hw" << std::endl;
        return 0;
    }

    int copy_from_hw(char* buf, size_t size) override {
        std::cout << "8226: copied data from hw" << std::endl;
        return 0;
    }
};

class HALChip: public IChip {
public:
    HALChip() {
        // check config flags to determine which chip we have
        if (chip_number == "8224") {
            chip = new chip8224();
        } else {
            chip = new chip8226();
        }
    }
    int copy_to_hw(char* buf, size_t size) override {
        return chip->copy_to_hw(buf, size);
    }

    int copy_from_hw(char* buf, size_t size) override {
        return chip->copy_from_hw(buf, size);
    }
private:
    // just depends on an interface, not on specific chip
    IChip *chip;
};

class WiFiDriver {
public:
    WiFiDriver(HALChip chip): chip_(chip) {};
    int send(char *buf, size_t size) {
        return chip_.copy_to_hw(buf, size);
    }
    int recv(char *buf, size_t size) {
        return chip_.copy_from_hw(buf, size);
    }
private:
    HALChip chip_;
};

int main() {
    char buf[1024];
    HALChip chip;
    WiFiDriver driver(chip);
    driver.send(buf, 1024);
    //...
    driver.recv(buf, 1024);
    return 0;
}