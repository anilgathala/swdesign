// Builder design pattern: [first principle]
// - when you need to build an object which cannot be built with a single constructor call
// - i.e., object needs to be built in stages
// - in this case, create a builder and provide interfaces to add/build up incrementally
// - builder provides a "get_object()" interface which returns the incrementally constructed obj
// - Examples:
// -- CacheKey in a CDN
// ----- protocol, domain name, path, query, tags (relevant headers)
// -- HtmlElement in an html doc (a recursive structure which can have a list of items within)
// --- text, paragraph, list, etc.


#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

class CacheKey {
public:
    CacheKey(std::string ns, std::string path):
        ns_(ns),
        path_(path),
        immutable_(false)
        {}

    void set_ns(std::string ns) {
        ns_ = ns;
    }

    void set_path(std::string path) {
        path_ = path;
    }
    
    void add_query(std::string key, std::string val) {
        if (immutable_) {
            return;
        }
        query_map_[key] = val;
    }

    void add_header(std::string key, std::string val) {
        if (immutable_) {
            return;
        }
        header_map_[key] = val;
    }

    void set_immutable() {
        immutable_ = true;
    }

    uint64_t get_hash() const {
        return ((ns_.length() + path_.length()) % 1024);
    }

    friend std::ostream& operator<<(std::ostream& os, const CacheKey& cache_key); 

    class CacheKeyBuilder {
    public:
        CacheKeyBuilder() {
            cache_key_ = new CacheKey("", "");
        }

        CacheKeyBuilder set_ns(std::string ns) {
            cache_key_->set_ns(ns);
            return *this;
        }

        CacheKeyBuilder set_path(std::string path) {
            cache_key_->set_path(path);
            return *this;
        }

        CacheKeyBuilder add_query(std::string key, std::string val) {
            cache_key_->add_query(key, val);
            return *this;
        }

        CacheKeyBuilder add_header(std::string key, std::string val) {
            cache_key_->add_header(key, val);
            return *this;
        }

        CacheKey* get_cache_key() {
            cache_key_->set_immutable();
            return cache_key_;
        }

    private:
        CacheKey *cache_key_;
    };

private:
    std::string ns_;
    std::string path_;
    std::unordered_map<std::string, std::string> query_map_;
    std::unordered_map<std::string, std::string> header_map_;
    bool immutable_; // once set to true, no changes can be made
};

std::ostream& operator<<(std::ostream& os, const CacheKey& cache_key) {
    os << "CacheKey: Begin" << std::endl
        << " ns: " << cache_key.ns_ << std::endl
        << " path: " << cache_key.path_ << std::endl
        << " immutable_: " << cache_key.immutable_ << std::endl;
    
    if (cache_key.query_map_.size()) {
        os << " query params: ";
        for (auto q: cache_key.query_map_) {
            os << " " << q.first << " = " << q.second << "; ";
        }
    }

    if (cache_key.header_map_.size()) {
        os << std::endl << " headers: ";
        for (auto hdr: cache_key.header_map_) {
            os << " " << hdr.first << " = " << hdr.second << "; ";
        }
    }

    os << std::endl
        << " hash: " << cache_key.get_hash()
        << std::endl
        << "CacheKey: End";
    
    return os;
}

int main()
{   
    CacheKey::CacheKeyBuilder key_builder;
    CacheKey *cache_key_00 = key_builder.set_ns("www.runforyourlife.com")
                                .set_path("images/")
                                .add_query("jpeg", "1")
                                .add_query("wide", "1")
                                .add_header("stale", "ok")
                                .add_header("mobile", "0")
                                .get_cache_key();

    std::cout << *cache_key_00 << std::endl;
    return 0;
}

