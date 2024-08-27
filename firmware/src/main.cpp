#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <dbcppp/Network.h>

using canid_t = uint32_t;
using signal_t = uint16_t;
const int FRAME_LENGTH = 8; // in bytes
const int NUM_SIGNALS = 4;

struct can_frame
{
	canid_t     can_id;
	double      timestamp;
	signal_t     data[NUM_SIGNALS];
};

int main()
{
    // load dbc file
    std::unique_ptr<dbcppp::INetwork> net;
    {
        std::ifstream idbc("../SensorBus.dbc");
        net = dbcppp::INetwork::LoadDBCFromIs(idbc);
    }

    // this is just a map from id -> the definition of a CAN Frame
    std::unordered_map<uint64_t, const dbcppp::IMessage*> messages;
    for (const dbcppp::IMessage& msg : net->Messages())
    {
        messages.insert(std::make_pair(msg.Id(), &msg));
    }

    can_frame frame;
    // need to parse can_id to from hexadecimal to decimal
    frame.can_id = 1797;

    // Open log file
    std::ifstream log("../dump.log");

    if (!log.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return 1;
    }

    std::string line;

    // Read and parse the file line by line
    while (std::getline(log, line)) {
        std::stringstream ss(line);
        std::string token;

        // Split the line by commas
        while (std::getline(ss, token, ',')) {
            auto iter = messages.find(frame.can_id);
            if (iter != messages.end())
            {
                const dbcppp::IMessage* msg = iter->second;
                std::cout << "Received Message: " << msg->Name() << "\n";
                for (const dbcppp::ISignal& sig : msg->Signals())
                {
                    sig.Decode(frame.data);
                }
            }
        }
    }

    // Close the file
    log.close();
}