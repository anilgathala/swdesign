// Command design pattern
// There is a generic command handler which takes a variety of commands (maintaining them in a wait list for example)
// and executes them (cmd.execute())
// Command is an object that is constructed with its input and when execute is called its going to execute and
// change some global state
// Example #1: bank account, transaction_executor takes commands, example commands: withdraw, deposit
// Example #2: stock, broker, example commmands: sellorder, buyorder
//

#include <iostream>
#include <string>
#include <vector>

class ICommand {
public:
    virtual void execute() = 0;
};

class PurgeCommand: public ICommand {
private:
    std::string url_;
public:
    PurgeCommand(std::string url): url_(url) {}
    void execute() {
        std::cout << "purging page from cache: " << url_ << std::endl;
    } 
};


class StoreCommand: public ICommand {
private:
    std::string url_;
public:
    StoreCommand(std::string url): url_(url) {}
    void execute() {
        std::cout << "storing page in cache: " << url_ << std::endl;
    } 
};


class FetchCommand: public ICommand {
private:
    std::string url_;
public:
    FetchCommand(std::string url, char* buf, int & size): url_(url) {}
    void execute() {
        // update buf and size
        std::cout << "fetching page from cache: " << url_ << std::endl;
    } 
};

class CacheCommandHandler: public ICommand {
private:
    std::vector<ICommand*> cmd_list_;
public:
    void add_cmd(ICommand* cmd) {
        cmd_list_.push_back(cmd);
    }

    void execute() {
        for (auto cmd: cmd_list_) {
            cmd->execute();
        }
    }
};



int main()
{
    int size = 1024;
    char buf[1024];
    StoreCommand store("www.foo.com/index.html");
    FetchCommand fetch("www.foo.com/index.html", buf, size);
    PurgeCommand purge("www.foo.com/index.html");

    CacheCommandHandler handler;
    handler.add_cmd(&store);
    handler.add_cmd(&fetch);
    handler.add_cmd(&purge);

    handler.execute();

    return 0;
}