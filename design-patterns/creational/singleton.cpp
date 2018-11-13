// Singleton design pattern
//
// let there be just one instance of a class (service or database)
// all the client code should get_obj() and operate on that one instance
//

#include <iostream>
#include <string>


class PackageManager {
private:
    PackageManager() {}
public:

static PackageManager* get_instance()
{
    static PackageManager* pkg_mgr = new PackageManager();
    return pkg_mgr;
}

bool install_pkg(std::string pkg_name) {
    std::cout << "installing pkg: " << pkg_name << std::endl;
    return true;
}

bool uninstall_pkg(std::string pkg_name) {
    std::cout << "uninstalling pkg: " << pkg_name << std::endl;
    return true;
}

    // disable copy, move constructors
    PackageManager(const PackageManager&) = delete;
    PackageManager(PackageManager&&) = delete;
    // disable copy, move assignment operators
    PackageManager& operator=(const PackageManager&) = delete;
    PackageManager& operator=(PackageManager&&) = delete;
};

void test_copy_func(PackageManager pkg_mgr)
{
    // do nothing
}


int main()
{
    PackageManager* pkg_mgr = PackageManager::get_instance();
    pkg_mgr->install_pkg("bind9");
    pkg_mgr->uninstall_pkg("xpdf");

    // let's try to copy
    // PackageManager pkg_mgr2 = *pkg_mgr;
    // compilation error:
    // singleton.cpp:47:20: error: call to deleted constructor of 'PackageManager'
    // PackageManager pkg_mgr2 = *pkg_mgr;

    // let's try to pass by value
    // test_copy_func(*pkg_mgr);
    // compilation error:
    // singleton.cpp:40:36: note: passing argument to parameter 'pkg_mgr' here
    // void test_copy_func(PackageManager pkg_mgr)

    // let's try to move
    // PackageManager pkg_mgr2 = std::move(*pkg_mgr);
    // compilation error:
    // singleton.cpp:61:20: error: call to deleted constructor of 'PackageManager'
    // PackageManager pkg_mgr2 = std::move(*pkg_mgr);

    return 0;
}