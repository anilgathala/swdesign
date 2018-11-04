// Builder design pattern:
// - when you need to build an object which cannot be built with a single constructor call
// - an object which is built in stages
// - in this case, create a builder and provide interfaces to add/build up incrementally
// - Examples:
// -- HtmlElement (a recursive structure which can have a list of items within)
// -- A Restaurant Bundled Menu Items (Veggie Meal, Non-veg Meal)
// ---- each item consists of multiple sub-items
// ---- built incrementally

// Imagine we're one sensor device that has three sensors - temp, humidity and light
// the sensor device / chip is connected to the host via some communication channel
// which uses RPC
// We'll need to build an "RPC Buffer", fill in sensor readings and send it across

#include <vector>
#include <iostream>

static int rnd_num = std::rand();
static int get_seq_num()
{
    return rnd_num++;
}

class SensorValue
{
public:
    SensorValue(std::string name, int max, int min, int current):
        name_(name), max_val_(max), min_val_(min), current_val_(current)
        {};
    std::string name_;
    int max_val_;
    int min_val_;
    int current_val_;
};

class MetricsValue
{
public:
    MetricsValue(std::string name, int max, int min, int current):
        name_(name), max_val_(max), min_val_(min), current_val_(current)
        {};
    std::string name_;
    int max_val_;
    int min_val_;
    int current_val_;
};

class IRPCPacket {
public:
    virtual void get_byte_stream(char * buf, int &len) = 0;
    virtual std::string get_pkt_type() = 0;
};

class SensorRPCPacket: public IRPCPacket
{
public:
    std::string type_;
    int sequence_num_;
    std::vector<SensorValue> sensor_values_;
    void get_byte_stream(char * buf, int &len) override { /* TODO */ };
    std::string get_pkt_type() override { return type_; }
    SensorRPCPacket() { type_ = "sensor"; };
    SensorRPCPacket(int sequence_num) : type_("sensor"), sequence_num_(sequence_num) {};
};

class MetricsRPCPacket: public IRPCPacket
{
public:
    std::string name_;
    int sequence_num_;
    std::vector<MetricsValue> metrics_values_;
    void get_byte_stream(char * buf, int &len) override { /* TODO */ };
    std::string get_pkt_type() override { return name_; }

    MetricsRPCPacket() { name_ = "metrics"; };
    MetricsRPCPacket(int sequence_num) : name_("metrics"), sequence_num_(sequence_num) {};
};

// goal of this class is track an on-going / partially built SensorRPCPacket obj
class SensorRPCPacketBuilder
{
public:
    SensorRPCPacketBuilder() {
       pkt_ = new SensorRPCPacket(get_seq_num());
    };

    // alternatively, add_sensor_val can take individual values (name, max, min , current)
    // and use vector.emplace_back() to avoid temp objects
    SensorRPCPacketBuilder& add_sensor_value(SensorValue val) {
        
        pkt_->sensor_values_.push_back(val);
        return *this;
    }
    SensorRPCPacket* get_rpc_packet() { return pkt_; }

    SensorRPCPacket* pkt_;
};

// goal of this class is track an on-going / partially built SensorRPCPacket obj
class MetricsRPCPacketBuilder
{
public:
    MetricsRPCPacketBuilder() {
       pkt_ = new MetricsRPCPacket(get_seq_num());
    };

    // alternatively, add_sensor_val can take individual values (name, max, min , current)
    // and use vector.emplace_back() to avoid temp objects
    MetricsRPCPacketBuilder& add_metrics_value(MetricsValue val) {
        
        pkt_->metrics_values_.push_back(val);
        return *this;
    }
    MetricsRPCPacket* get_rpc_packet() { return pkt_; }

    MetricsRPCPacket* pkt_;
};



bool RPCSend(IRPCPacket *pkt) {
    // serialize pkt 
    const int MAX_LEN = 2048;
    char buf[MAX_LEN];
    int len = 0;
    pkt->get_byte_stream(buf, len);

    // send bytes over channel
    std::cout << pkt->get_pkt_type() << " pkt sent over the channel!" << std::endl;
    return true;
}

int main()
{   
    SensorRPCPacketBuilder sensor_builder;
    // wait for it's time to collect -- temp -- sensor value
    SensorValue temp("Temperature", 98, 88, 92);
    sensor_builder.add_sensor_value(temp);

    // wait for it's time to collect -- hum & light -- sensor value
    SensorValue hum("Humidity", 25, 20, 22);
    SensorValue light("Light", 15, 8, 12);
    // notice chained add
    sensor_builder.add_sensor_value(hum).add_sensor_value(light);

    MetricsRPCPacketBuilder metrics_builder;
    MetricsValue cpu("cpu", 80, 15, 18);
    MetricsValue mem("mem", 60, 8, 12);
    MetricsValue io("io", 90, 75, 81);
    metrics_builder.add_metrics_value(cpu).add_metrics_value(mem).add_metrics_value(io);

    RPCSend(sensor_builder.get_rpc_packet());
    RPCSend(metrics_builder.get_rpc_packet());

    return 0;
}

