// Builder pattern [Second principle]
// “Aims to separate the construction of a complex object from its representation so that the 
//  same construction process can create different representations.” 
// Link: https://www.geeksforgeeks.org/builder-design-pattern/
// Wiki Link: https://en.wikipedia.org/wiki/Builder_pattern#Crystal
// 
// ** Problem:
// There is an engine which builds a category of objects (e.g., network packets) and does something with them
// ( network driver receives pkts and submits to hw chip)
// These category of objects have a common structure/outline. However, each object may be represented differently.
// (tcp pkt, udp pkt). All network pkts have to build header and payload before leaving host (common).
// However headers are represented for each pkt type.
// How can we have the same construction method (deriver.send_pkt_to_hw()) build different
// category objects ?
// ** Solution:
// Abstract out different category of objects with an object interface (INetworkPkt)
// Create a specific class which can be used by generic constructiion method
// this class implements object interface in a generic way (Packet)
// Now have a builder class for each category, they all derive from a common builder interface
// common builder interface: IPacketBuilder (defines common outline/stages of building)
// specific builders: TcpPacketBuilder, UdpPacketBuilder (each have a Packet memeber, they're building)
// 
// Now, common driver method (IPacketBuilder* builder)
// -- builder->build_header()
// -- builder->build_payload()
// -- pkt = builder->get_pkt()
// -- submit pkt to hw

#include <vector>
#include <iostream>

// single item interface
// e.g., HousePlan
// single object implmenents HosuePlan -- > "class House"

// builder interface
// buildBasement, buildStructure, buildRoof, etc.

// IglooHousebuilder, TipiHouseBuilder

// civilEngineer can take generic HouseBuilder and go through the steps
// -- just uses BuilderInterface
// -- this can build a variety of objects whose build process is both "incremental" and "identical"

// my example: tcp_pkt, udp_pkt
// pkt will have header and payload
// -- prepare header
// -- prepare payload
// -- calculate checksum
// -- get_pkt
// -- driver can take a builder, build pkt, get_pkt and send pkt out

const size_t CONFIG_MAX_PAYLOAD = 1024;

struct TcpHeader {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq_num;
    uint32_t ack_num;
    uint16_t flags;
    uint16_t window_size;
    uint16_t checksum;
    uint16_t urgent_ptr;
    // skip optional bytes for now
};

struct UdpHeader {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t len;
    uint16_t checksum;
};

// interface for object
class INetworkPkt {
public:
    virtual void set_max_hdr_len(size_t len) = 0;
    virtual bool set_header(char * a_hdr, size_t len) = 0;
    virtual bool set_payload(char * a_payload, size_t len) = 0;
};

class Packet: public INetworkPkt {
public:
    void set_max_hdr_len(size_t len) {
        max_hdr_len = len;
    }

    bool set_header(char * a_hdr, size_t len) {
        if (max_hdr_len <= 0 && len > max_hdr_len) {
            return false;
        }
        memcpy(hdr, a_hdr, len);
        hdr_len = len;
        return true;
    }

    bool set_payload(char * a_payload, size_t len) {
        if (len > CONFIG_MAX_PAYLOAD) {
            return false;
        }
        memcpy(payload, a_payload, len);
        payload_len = len;
        return true;
    }
    friend std::ostream& operator<<(std::ostream& os, const Packet& pkt); 
private:
    char hdr[CONFIG_MAX_PAYLOAD];
    size_t hdr_len;
    size_t max_hdr_len;
    char payload[CONFIG_MAX_PAYLOAD];
    size_t payload_len;
};

std::ostream& operator<<(std::ostream& os, const Packet& pkt)  
{  
    os << " hdr len: " << pkt.hdr_len << "; payload len: " << pkt.payload_len;
    return os;  
}


class IPacketBuilder {
public:
    virtual bool build_header(char *hdr, size_t len) = 0;
    virtual bool build_payload(char *a_payload, size_t len) = 0;
    virtual Packet* get_pkt() = 0;
};

class TcpPacketBuilder: public IPacketBuilder {
public:

    TcpPacketBuilder() {
        // pretend: allocate in kernel space
        pkt = new Packet();
        pkt->set_max_hdr_len(sizeof(TcpHeader));
    }

    bool build_header(char * hdr, size_t len) {
        return pkt->set_header(hdr, len);
    }

    bool build_payload(char * a_payload, size_t len) {
        return pkt->set_payload(a_payload, len);
    }

    Packet* get_pkt() {
        return pkt;
    }

private:
    Packet *pkt;
};

class UdpPacketBuilder: public IPacketBuilder {
public:

    UdpPacketBuilder() {
        // pretend: allocate in kernel space
        pkt = new Packet();
        pkt->set_max_hdr_len(sizeof(UdpHeader));
    }

    bool build_header(char * hdr, size_t len) {
        return pkt->set_header(hdr, len);
    }

    bool build_payload(char * a_payload, size_t len) {
        return pkt->set_payload(a_payload, len);
    }

    Packet* get_pkt() {
        return pkt;
    }

private:
    Packet *pkt;
    UdpHeader udp_hdr;;
    char* payload;
    size_t payload_len;
};


// network stack call's the following driver method
// to handover pkts to driver for tx
// drviver implements different builders
// network stack instantiates an instance of the appropriate builder
// and passes it to the driver along with appropriate buffers
// IMP: if we change the way the TcpHeader has changed, the below method doesn't have to
void driver_submit_pkt(
    IPacketBuilder *builder,
    char * hdr,
    size_t hdr_len,
    char * payload,
    size_t payload_len
    )
{
    // generic code for any pkt
    if (!builder) {
        std::cout << "null builder" << std::endl;
    }
    // set_header
    builder->build_header(hdr, hdr_len);
    // // set_payload
    builder->build_payload(payload, payload_len);

    Packet *pkt = builder->get_pkt();
    std::cout << " submit_to_hw pkt with " << *pkt << std::endl;
}

int main() {

    TcpHeader tcp_hdr;
    char tcp_payload[1024];

    UdpHeader udp_hdr;
    char udp_payload[1024];

    TcpPacketBuilder tcp_builder;
    UdpPacketBuilder udp_builder;

    driver_submit_pkt(&tcp_builder, (char*)&tcp_hdr, sizeof(TcpHeader), tcp_payload, 1024);
    driver_submit_pkt(&udp_builder, (char*)&udp_hdr, sizeof(UdpHeader), udp_payload, 1024);

    return 0;
}

