// Adapter design pattern [principle #2]
// this pattern can be used when we want to create one aggregated interface
// example: MediaPlayer can incorporate Mp3player and VlcPlayer classes
//

#include <iostream>
#include <string>

enum CompressionType {
    ZLIB,
    BROTLI
};

class ICompressor {
public:
    // compress
    virtual void deflate(std::string in_file, std::string out_file) = 0;
    // decompress
    virtual void inflate(std::string in_file, std::string out_file) = 0;
};


class ZlibCompressor: public ICompressor {
public:
    // compress
    void deflate(std::string in_file, std::string out_file) {
        std::cout << "Zlib: deflate.. " << std::endl;
    }
    // decompress
    void inflate(std::string in_file, std::string out_file) {
        std::cout << "Zlib: inflate.. " << std::endl;
    }
};

class BrotliCompressor: public ICompressor {
public:
    // compress
    void deflate(std::string in_file, std::string out_file) {
        std::cout << "Brotli: deflate.. " << std::endl;
    }
    // decompress
    void inflate(std::string in_file, std::string out_file) {
        std::cout << "Brotli: inflate.. " << std::endl;
    }
};


class AdapterCompressor {
public:
    AdapterCompressor(CompressionType compression_type) : compression_type_(compression_type)
    {
        if (compression_type == ZLIB) {
            compressor_ = new ZlibCompressor();
        } else {
            compressor_ = new BrotliCompressor();
        }
    }

    CompressionType get_compression_type()
    {
        return compression_type_;
    }

    void deflate(std::string in_file, std::string out_file) {
         compressor_->deflate(in_file, out_file);
    }
    // decompress
    void inflate(std::string in_file, std::string out_file) {
        compressor_->inflate(in_file, out_file);
    }
private:
    CompressionType compression_type_;
    ICompressor* compressor_;
};

void client_code(AdapterCompressor * compressor)
{
    //...
    compressor->deflate("data.log", "compressed_data00.log");
    //....
    compressor->inflate("compressed_data00.log", "data00.log");
}

int main()
{
    AdapterCompressor *c1 = new AdapterCompressor(BROTLI);
    client_code(c1);

    //...

    AdapterCompressor *c2 = new AdapterCompressor(ZLIB);
    client_code(c2);
    return 0;
}