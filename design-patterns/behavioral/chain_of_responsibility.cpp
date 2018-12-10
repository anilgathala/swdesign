// Chain of responsibility
// A request / event arrives into the system and there is a chain of handlers to (potentially) handle it
// The request /event is passed into the chain and handlers accordingly handle
//


#include <iostream>

class BaseInterruptHandler {
private:
    BaseInterruptHandler* next = nullptr;
public:
    void add_handler(BaseInterruptHandler* handler) {
        if (next) {
            next->add_handler(handler);
            return;
        }
        next = handler;
    }

    virtual void handle_irq(int num)
    {
        if (next) {
            next->handle_irq(num);
        }
    }
};

class PIRSensorInterruptHandler: public BaseInterruptHandler {
private:
    int irq_num_;
public:
    PIRSensorInterruptHandler(int irq): irq_num_(irq) {}
    void handle_irq(int num)
    {
        if (num == irq_num_) {
            std::cout << "PIRSensorInterruptHandler: handling irq num: " << num << std::endl;
            return;
        }
        BaseInterruptHandler::handle_irq(num);
    }
};

class TempSensorInterruptHandler: public BaseInterruptHandler {
private:
    int irq_num_;
public:
    TempSensorInterruptHandler(int irq): irq_num_(irq) {}
    void handle_irq(int num)
    {
        if (num == irq_num_) {
            std::cout << "TempSensorInterruptHandler: handling irq num: " << num << std::endl;
            return;
        }
        BaseInterruptHandler::handle_irq(num);
    }
};

class HumiditySensorInterruptHandler: public BaseInterruptHandler {
private:
    int irq_num_;
public:
    HumiditySensorInterruptHandler(int irq): irq_num_(irq) {}
    void handle_irq(int num)
    {
        if (num == irq_num_) {
            std::cout << "HumiditySensorInterruptHandler: handling irq num: " << num << std::endl;
            return;
        }
        BaseInterruptHandler::handle_irq(num);
    }
};

BaseInterruptHandler* get_irq_handler_chain()
{
    PIRSensorInterruptHandler *pir = new PIRSensorInterruptHandler(10);
    TempSensorInterruptHandler *temp = new TempSensorInterruptHandler(12);
    HumiditySensorInterruptHandler *hum = new HumiditySensorInterruptHandler(15);

    pir->add_handler(temp);
    temp->add_handler(hum);

    return pir;
}


int main()
{
    BaseInterruptHandler* irq_handler = get_irq_handler_chain();

    irq_handler->handle_irq(12);
    irq_handler->handle_irq(16); // no handler
    irq_handler->handle_irq(15);
    irq_handler->handle_irq(20); // no handler
    irq_handler->handle_irq(10);

    return 0;
}