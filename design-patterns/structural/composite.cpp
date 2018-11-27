// Composite design pattern:
// Compose objects into tree structure to represent part-whole hierarchies.
// Composite lets client treat individual objects and compositions of objects uniformly.
// link: https://www.geeksforgeeks.org/composite-design-pattern/
//
// Example:
// Developer and Manager are leafs of slightly diff types.
// CompanyDirectory is composite object
// all the above classes implement from a baseclass which has "ShowDetails()" method
// client can call "ShowDetails()" on both leaf and composite objects
//

#include <iostream>
#include <sys/types.h>
#include <vector>

// commong interface for both leaf and composite objects
// the interface used by client code
class Bytes {
public:
    virtual size_t size() = 0;
};

// leaf class
class Header: public Bytes {
public:
    size_t size() {
        return 20;
    }
};

// leaf class
class Payload: public Bytes {
public:
    size_t size() {
        return 1024;
    }
};

// composite class
class Packet: public Bytes {
private:
    Header hdr_;
    Payload payload_;
public:
    Packet(Header hdr, Payload payload): hdr_(hdr), payload_(payload) {}
    Packet() {}
    Packet(Packet& p) {
        hdr_ = p.get_hdr();
        payload_ = p.get_payload();
    }
    Packet(Packet* p) {
        hdr_ = p->get_hdr();
        payload_ = p->get_payload();
    }
    size_t size() {
        return hdr_.size() + payload_.size();
    }
    Header get_hdr() {
        return hdr_;
    }

    Payload get_payload() {
        return payload_;
    }
};

// even higher-level compiste class
class Stream: public Bytes {
private:
    std::vector<Packet*> packets_;
public:
    size_t size() {
        size_t count = 0;
        for(auto pkt: packets_) {
            count += pkt->size();
        }
        return count;
    }

    void add_packet(Packet* p) {
        packets_.push_back(p);
    }

};

// client code: would like to handle/process both leaf and composite objects alike
void estimate_traffic_size(Bytes *b)
{
    std::cout << " size = " << b->size() << std::endl;
}

int main()
{

    Stream stream;
    stream.add_packet(new Packet());
    stream.add_packet(new Packet());
    stream.add_packet(new Packet());

    Packet pkt;

    // traffic has a stream and an individual pkt
    estimate_traffic_size(&stream);
    estimate_traffic_size(&pkt);


    return 0;
}