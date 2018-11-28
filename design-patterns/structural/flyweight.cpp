// Flyweight design pattern:
// Flyweight pattern is primarily used to reduce the number of objects created and to decrease memory footprint and increase performance.
// Link: https://www.tutorialspoint.com/design_pattern/flyweight_pattern.htm
// Link: https://www.geeksforgeeks.org/flyweight-design-pattern/
//

// objects have intrinsic and extrinsic attributes. The intrinsic can be shared across objects, while the extrinsic cannot be.
// In flyweight design pattern, we create a hashmap of <obj_type, obj_with_intrinsic_attributes_preset>
// client grabs references to an object of a particular type.
// Client uses it in read-only mode, unless the hashmap deletes the returned obj.
// for most part, flyweight is useful when objects cached are read-only

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Keyword {
private:
    std::string key_;
    int64_t last_accessed_time_;
public:
    Keyword(std::string key): key_(key) {}
    void set_access_time(int64_t timestamp) {
        last_accessed_time_ = timestamp;
    }
    int64_t get_last_accessed_time() {
        return last_accessed_time_;
    }
};

class KeyWordFactory {
public:
    Keyword* get_keyword(std::string key) {
        auto keyword = new Keyword(key);
        keyword->set_access_time(232354);
        return keyword;
    }
};

// rather than creating the keyword strings/objs again and again, we store one obj per specific keyword
// when a particular keyword repeats in a million searches, the mem savings will be massive
// as we create only one instance of the string

class FlyWeight {
private:
    KeyWordFactory *factory_;
    std::unordered_map<std::string, Keyword*> index_;
    std::unordered_map<Keyword*, std::string> rev_index_;
public:
    FlyWeight() {
        factory_ = new KeyWordFactory();
    }

    Keyword* get_keyword(std::string key) {
        if (index_.find(key) != index_.end()) {
            index_[key]->set_access_time(232343);    
        } else {
            index_[key] = new Keyword(key);
            rev_index_[index_[key]] = key;
        }
        return index_[key];
    }

    int get_key(Keyword* keyword, std::string &key) {
        if (rev_index_.find(keyword) != rev_index_.end()) {
            key = rev_index_[keyword];
            return 0;
        }
        return -1;
    }
};

std::string do_search(FlyWeight &obj_cache, std::vector<Keyword*> keywords) {
    std::cout << " searching for: ";
    for (auto keyword: keywords) {
        std::string key;
        obj_cache.get_key(keyword, key);
        std::cout << key << " ";
    }
    std::cout << std::endl;
    return "results: too many";
}

int main()
{
    FlyWeight obj_cache;
    // search #1: "football worldcup"
    auto football = obj_cache.get_keyword("football");
    auto worldcup = obj_cache.get_keyword("worldcup");

    // send to search engine
    std::vector<Keyword *> keywords;
    keywords.push_back(football);
    keywords.push_back(worldcup);
    do_search(obj_cache, keywords);

    keywords.resize(0);
    // search #2: "worldcup football"
    auto worldcup2 = obj_cache.get_keyword("worldcup");
    auto football2 = obj_cache.get_keyword("football");
    keywords.push_back(worldcup2);
    keywords.push_back(football2);
    
    do_search(obj_cache, keywords);
    
    // both the searches use the same copy of two keywords - "worldcup" and "football"

    return 0;
}
