// mediator design pattern
// the mediator object provides a medium/channel for different participant objects to talk to each other
// This pattern is used to reduce the complexity of communication between objects
// 

// This great example is taken from the book, "Design Patterns in Mordern C++"

#include <iostream>
#include <string>
#include <vector>

class ChatRoom;

class User {
public:
    std::string name_;
    ChatRoom* room_;
    std::vector<std::string> chat_log_;
    User(const std::string& name): name_(name), room_(nullptr) {}
    void receive(const std::string& src, const std::string& msg);
    void say(const std::string msg) const;
    void pm(const std::string& dst, const std::string& msg) const;
};

class ChatRoom {
private:
    std::vector<User*> user_list_;
public:
    void join(User* usr);
    void broadcast(const std::string & src, const std::string &msg);
    void message(const std::string & src, const std::string & who, const std::string & msg);
};

void ChatRoom::join (User *usr)
{
    std::string join_msg = usr->name_ + " joins the chat";
    broadcast("room", join_msg);
    usr->room_ = this;
    user_list_.push_back(usr);
}

void ChatRoom::broadcast(const std::string & src, const std::string &msg)
{
    for (auto usr: user_list_) {
        if (usr->name_ != src) {
            usr->receive(src, msg);
        }
    }
}

void ChatRoom::message(const std::string & src, const std::string & who, const std::string & msg)
{
    auto target = find_if(user_list_.begin(), user_list_.end(), [&](const User* u) { return u->name_ == who;});

    if (target != user_list_.end()) {
        (*target)->receive(src, msg);
    }
}


void User::receive(const std::string& src, const std::string& msg)
{
    std::string s = src + ": '" + msg + "' ";
    std::cout << "[" << name_ << "'s chat session] " << s << std::endl;
    chat_log_.push_back(s);
}

void User::say(const std::string msg) const
{
    room_->broadcast(name_, msg);
}

void User::pm(const std::string& dst, const std::string& msg) const
{
    room_->message(name_, dst, msg);
}


int main()
{
    ChatRoom room;

    User john("John");
    User brad("Brad");
    User ash("Ashley");

    room.join(&john);
    room.join(&brad);

    john.say("hi room!");
    brad.say("hey, john!");

    room.join(&ash);
    ash.say("hi everyone!");

    brad.pm("Ashley", "glad you could join us, Ash!");

    return 0;
}