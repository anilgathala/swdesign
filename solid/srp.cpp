#include <iostream>
#include <string>

//
// Single Responsibility Principle (SRP)
//

// a simple file interface
class IFile {
public:
    virtual int file_read(char *buf, int len) = 0;
    virtual int file_write(char *buf, int len) = 0;
};

// a simpel file class
class File: public IFile {
public:
    File(const std::string name, size_t size);
    int file_read(char *buf, int len) override;
    int file_write(char *buf, int len) override;
    size_t size();
     // Violates SRP!
    // now this is a violation of SRP
    // if we want to change the way hash is calculated,
    // we'll have to change the File class
    uint32_t hash();
private:
    std::string name_;
    // circular buffer of size "size_"
    char *buf_;
    size_t size_;
    // current ptr
    // this is where next read, write happens
    size_t ptr_;
};

File::File(std::string name, size_t size): name_(name), size_(size), ptr_(0)
{
    buf_ = (char *) malloc(sizeof(char)*size);
    memset(buf_, 0, size);
}

int File::file_read(char* buf, int len)
{
    // read 'len' bytes from (buf_ + ptr_)
    std::cout << "read " << len << " bytes from " << name_ << std::endl; 
    return 0;
}

int File::file_write(char* buf, int len)
{
    // write 'len' bytes from 
    std::cout << "wrote " << len << " bytes to " << name_ << std::endl; 
    return 0;
}

// Violates SRP!
uint32_t File::hash()
{
    // calculate hash for this file
    uint32_t sum = 0;
    for (size_t i = 0; i < size_; i++) {
        sum += buf_[i];
    }
    sum *= 2;
    std::cout << "returning hash: " << sum << " for file " << name_ << std::endl;
    return sum;
}

size_t File::size()
{
    return size_;
}

// Fix for SRP violation!
class Hasher {
public:
    uint32_t hash(File *f);
};

uint32_t Hasher::hash(File *f)
{
    // calculate hash for this file
    size_t sz = f->size();
    char *buf = (char *)malloc(sizeof(char)*sz);
    f->file_read(buf, sz);
    uint32_t sum = 0;
    for (size_t i = 0; i < sz; i++) {
        sum += buf[i];
    }
    sum *= 2;
    return sum;
}

int main()
{
    const size_t size = 1024;
    char buf[size];
    memset(buf, 1, size);
    File atxt("a.txt", size);
    atxt.file_write(buf, 64);
    atxt.file_write((buf+64), 64);
    atxt.file_read(buf, 64);
    uint32_t h1 = atxt.hash();
    std::cout << "h1 = " << h1 << std::endl;

    Hasher hasher;
    uint32_t h2 =hasher.hash(&atxt);
    std::cout << "h2 = " << h2 << std::endl;
    
    return 0;
}