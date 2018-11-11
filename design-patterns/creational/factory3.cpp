// Factory pattern
// Principle #2: constructors are inflexible, so use a factory class to create varied forms of an object
//
// Example:
// -- if you want to create Point(x, y) where the pair x, y represents cartesian coordinates
// -- you also sometimes want to create Point(r, theta) where (r, theta) represents polar coordinates
// -- overloading constructor will not help, you need to add another constructor Point(x, y, type)
// -- but that's not very neat as so much of creation logic is exposed to the client
//
// You can create a PointFactory which provides PointFactory::CatersianPoint(x, y) and
// PointFactory::PolarPoint(r, theta); client can call appropriate method on factory class
//


// the example here is a wrapper class over basic unix file
// client code:
//  -- create a file obj based on data file path (read, write)
//  -- create a file obj based on log file path (append only)

// **** use case **** [ there is no inheritance here]
// -- you want to use a customized version of an object
// -- either the client code can create by passing adjusted parameters to constructor, or
// -- we can have factory class provide an api to get this customized object, so client doesn't depend on it

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

const std::string data_file = "/opt/service/data/service.dat";
const std::string log_file = "/var/log/service/service.log";

class UnixFile {
public:
    UnixFile(std::string filename, int flags, int mode, int &result): filename_(filename), fd_(0)
    {
        fd_ = open(filename.c_str(), flags, mode);
        result = (fd_ < 0) ? -1 : 0;
    }

    ssize_t unixfile_write(const void *buf, size_t count) {
        if (fd_ < 0) {
            return 0;
        }
        return write(fd_, buf, count);
    }

    ssize_t unixfile_read(void *buf, size_t count) {
        if (fd_ < 0) {
            return 0;
        }
        return read(fd_, buf, count);
    }

    off_t unixfile_lseek(off_t offset, int whence) {
        if (fd_ < 0) {
            return 0;
        }
        // POSIX makes the following call a no-op for files opened with O_APPEND
        // link: http://man7.org/linux/man-pages/man2/lseek.2.html
        return lseek(fd_, offset, whence);
    }

    // inner class: can access private members of UnixFile
    class Factory {
    public:
        static UnixFile* CreateLogFile() {
            // append only file
            int ret;
            UnixFile* file = new UnixFile(log_file, O_CREAT, O_APPEND, ret);
            if (ret < 0) {
                return nullptr;
            }
            char headers[1024];
            // intialize log file with header (machine ip, timestamp, etc.)
            file->unixfile_write(headers, 1024);
            return file;
        }

        static UnixFile* CreateDataFile() {
            // can seek and write/read anywhere
            int ret;
            UnixFile* file = new UnixFile(data_file, O_CREAT, O_RDWR, ret);
            if (ret < 0) {
                return nullptr;
            }
            return file;
        }
    };

private:
    std::string filename_;
    int fd_;
};


void launch_bg_service()
{
    std::cout << "service running..." << std::endl;
}

void init_service()
{
    // create data file which is read as well as write
    UnixFile* config_file_ptr = UnixFile::Factory::CreateDataFile();
    std::cout << "created service data file..." << std::endl;

    // create append only log file (every write seeks to the end of file internally)
    UnixFile* log_file_ptr = UnixFile::Factory::CreateLogFile();
    std::cout << "created service log file..." << std::endl;

}

int main()
{
    init_service();
    launch_bg_service();
    return 0;
}

