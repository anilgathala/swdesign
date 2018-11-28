// facade design pattern:
// Facade pattern hides the complexities of the system and provides an interface to the client using which the client can access the system. 
// link: https://www.tutorialspoint.com/design_pattern/facade_pattern.htm
// link: https://www.geeksforgeeks.org/facade-design-pattern-introduction/
//

#include <iostream>
#include <string>
#include <vector>

class ICache {
public:
    virtual int store(std::string key, std::vector<char> bytes) = 0;
    virtual int fetch(std::string key, std::vector<char> & bytes) = 0;
    virtual size_t get_capacity() = 0;
    virtual size_t get_size() = 0; // current size - sum total of objs cached
};

class HotObjectCache: public ICache {
private:
    size_t capacity_;
    size_t size_;
public:
    HotObjectCache(size_t capacity): capacity_(capacity), size_(0) {}
    int store(std::string key, std::vector<char> bytes) {
        if (size_ + bytes.size() > capacity_) {
            return -1;
        }
        std::cout << "HotObjCache: storing obj in memory: " << key << " of size  = " <<  bytes.size() << std::endl;
        size_ += bytes.size();
        return 0; // success
    }


    int fetch(std::string key, std::vector<char> & bytes) {
        if (bytes.size() > capacity_) {
            return -1;
        }
        size_t fake_obj_size = 1024;
        std::cout << "HotObjCache: fetching obj from memory: " << key << " of size  = " <<  fake_obj_size << std::endl;
        return 0; // success
    }

    size_t get_capacity()
    {
        return capacity_;
    }

    size_t get_size()
    {
        return size_;
    }
};

class DiskCache: public ICache {
private:
    size_t capacity_;
    size_t size_;
public:
    DiskCache(size_t capacity): capacity_(capacity), size_(0) {}
    int store(std::string key, std::vector<char> bytes) {
        if (size_ + bytes.size() > capacity_) {
            return -1;
        }
        std::cout << "storing obj on disk: " << key << " of size  = " <<  bytes.size() << std::endl;
        size_ += bytes.size();
        return 0; // success
    }


    int fetch(std::string key, std::vector<char> & bytes) {
        size_t fake_obj_size = 1024;
        std::cout << "fetching obj from disk: " << key << " of size  = " <<  fake_obj_size << std::endl;
        return 0; // success
    }

    size_t get_capacity()
    {
        return capacity_;
    }

    size_t get_size()
    {
        return size_;
    }
};

class OriginFetchService {
public:
    int fetch(std::string key, std::vector<char> & bytes) {
        size_t fake_obj_size = 1024;
        std::cout << "fetching obj from server: " << key << " of size  = " <<  fake_obj_size << std::endl;
        return 0; // success
    }
};

// facade class: hides the complexity of hot obj cache, disk cache and ofetch from client
class SmartCache: public ICache {
private:
    HotObjectCache *hot_obj_cache_;
    DiskCache *disk_cache_;
    OriginFetchService *ofetch_service_;
public:
    SmartCache(size_t hot_obj_capacity, size_t disk_cache_capacity) {
        hot_obj_cache_ = new HotObjectCache(hot_obj_capacity);
        disk_cache_ = new DiskCache(disk_cache_capacity);
        ofetch_service_ = new OriginFetchService();
    }

    int store(std::string key, std::vector<char> bytes) {
        // check hot obj cache
        int res = hot_obj_cache_->store(key, bytes);
        if (res == 0) {
            return 0;
        }
        // check disk cache
        res = disk_cache_->store(key, bytes);
        if (res == 0) {
            return 0;
        }
        return -1; // success
    }


    int fetch(std::string key, std::vector<char> & bytes) {
        // check hot obj cache
        int res = hot_obj_cache_->fetch(key, bytes);
        if (res == 0) {
            return 0;
        }
        // check disk cache
        res = disk_cache_->fetch(key, bytes);
        if (res == 0) {
            return 0;
        }

        return ofetch_service_->fetch(key, bytes);
    }

    size_t get_size()
    {
        return 0;
    }

    size_t get_capacity()
    {
        return 0;
    }


};

int main()
{
    std::vector<char> send_bytes(1024, 1);
    std::vector<char> recv_bytes(1024, 1);
    SmartCache smart_cache(128, 1024);
    smart_cache.store("sample", send_bytes);
    smart_cache.fetch("sample", recv_bytes);

    return 0;
}