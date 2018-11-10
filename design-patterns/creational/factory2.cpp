// Factory Pattern
// principle #1: remove creation logic from client code using factory pattern
// this file has fix for the bad-example-code in factory.cpp
// 
// in this pattern, we create object without exposing creation logic to the client
// and client uses the same common interface to create new type of object
// -- without this, the client code has to change each time we have a new type of object
// link:
// https://www.geeksforgeeks.org/design-patterns-set-2-factory-method/
//

// Problem:
// client code needs an "utility object" to accomplish its processing duties
// However, each time there is a new type of "utility" we need to support - we end up modifying the client code
// 
// Solution:
// First, define an interface for all utility objects and then client code operate on the interface pointer
// Second, in regards to creation of the utility object - abstract that creation method (create_utility_object),
//         make it pure virtual
// third, now out client class is a abstract class. so each time you want to use a new utility object based client
//        you simply extend the abstract-client-class with specific implmenetation of the create_utility_object
// Lastly, have a method which takes an input string/int and creates the appropriate client-class based on the input

#include <iostream>
#include <string>
#include <atomic>

// **************************************
// fix for BAD EXAMPLE in factory.cpp
// **************************************

class ICompressor {
public:
    // compress method: returns compression factor achieved (out_size / in_size)
    virtual double deflate(std::string in_file, std::string out_file) = 0;
    // decompress method: returns deflated size
    virtual size_t inflate(std::string in_file, std::string out_file) = 0;
};

class Zlib: public ICompressor {
public:
    Zlib (int random_seed) {};
    // compress method: returns compression factor achieved (out_size / in_size)
    double deflate(std::string in_file, std::string out_file) {
        std::cout
            << "Zlib: compressed "
            << in_file
            << ", out file: "
            << out_file
            << " compression ratio: 0.75"
            << std::endl;
            return 0.75;
    }
    // decompress method: returns deflated size
    size_t inflate(std::string in_file, std::string out_file) {
        std::cout
            << "Zlib: decompress "
            << in_file
            << ", out file: "
            << out_file
            << " infalted size: 1024 MB"
            << std::endl;
            return 1024;
    }
};

class Brotli: public ICompressor {
public:
    Brotli (int x, int y) {};
    // compress method: returns compression factor achieved (out_size / in_size)
    double deflate(std::string in_file, std::string out_file) {
        std::cout
            << "Brotli: compressed "
            << in_file
            << ", out file: "
            << out_file
            << " compression ratio: 0.80"
            << std::endl;
            return 0.75;
    }
    // decompress method: returns deflated size
    size_t inflate(std::string in_file, std::string out_file) {
        std::cout
            << "Brotli: decompress "
            << in_file
            << ", out file: "
            << out_file
            << " infalted size: 512 MB"
            << std::endl;
            return 512;
    }
};

class FileTransferAgent {
public:
    FileTransferAgent(): next(0) {}

    virtual ICompressor * create_compressor() = 0;

    void send(std::string file) {

        ICompressor *compressor = create_compressor();
        std::string outfile = "tmp/" + std::to_string(next++) + ".fta";
        create_temp_file(outfile);
        
        compressor->deflate(file, outfile);

        tx(outfile);

        cleanup_file(outfile);

    }
    void create_temp_file(std::string outfile) {
        std::cout << "create temp file: "  << outfile << std::endl;
    }
    // it's silly we do not have a destination
    // let's just assume we have a single destination
    void tx(std::string file) {
        std::cout << "sending file " << file << " over the network" << std::endl;
    }
    void cleanup_file(std::string file) {
        std::cout << "file: "  << file << " clean up done!" << std::endl;
    }

private:
    std::atomic_int64_t next;
};

class ZlibFTA: public FileTransferAgent {
public:
    ICompressor * create_compressor() {
        return new Zlib(78253);
    }
};

class BrotliFTA: public FileTransferAgent {
public:
    ICompressor * create_compressor() {
        return new Brotli(1024, 512);
    }
};


const std::string ZLIB = "zlib";
const std::string BROTLI = "brotli";

int main()
{   

    ZlibFTA * zlib_fta = new ZlibFTA();
    BrotliFTA * brotli_fta = new BrotliFTA();
    // it's silly we do not have a destination, let's just assume we a have a single destination
    std::cout << " --------- " << std::endl;
    zlib_fta->send("power_consumption_data.txt");
    std::cout << " --------- " << std::endl;
    brotli_fta->send("cpu_load_metrics.txt");
    return 0;
}