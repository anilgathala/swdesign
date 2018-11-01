#include <iostream>
#include <assert.h>

class ILogger  {
public:
    virtual void log(std::string key, std::string val) = 0;
};

class ICookieManager  {
public:
    virtual void setCookie(std::string key, std::string val) = 0;
    virtual std::string getCookie() = 0;
};

class CookieEmulator: public ICookieManager, public ILogger {
public:
    void setCookie(std::string key, std::string val) override {
        std::cout << " setCookie " << std::endl;
    }
    std::string getCookie() override {
        std::cout << " getCookie " << std::endl;
        return "cookie=gold";
    }
    void log(std::string key, std::string val) override {
        std::cout << " logging cookie:  " << key << ", " << val << std::endl;
    }
};

// doesn't need the "log" method
class BrowserCookie: public ICookieManager {
public:
    void setCookie(std::string key, std::string val) override {
        std::cout << " setCookie " << std::endl;
    }
    std::string getCookie() override {
        std::cout << " getCookie " << std::endl;
        return "cookies=gold";
    }

};


int main() {
    BrowserCookie browser_cookie_mgr;
    CookieEmulator emu_cookie_mgr;
    browser_cookie_mgr.setCookie("age", "120");

    emu_cookie_mgr.setCookie("emuAge", "12");
    emu_cookie_mgr.log("emuAge", "12");
    
    return 0;
}