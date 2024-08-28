#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <dbcppp/Network.h>

using namespace std;
using canid_t = uint32_t;

const int FRAME_LENGTH = 8; // in bytes
const int NUM_SIGNALS = 4;

struct can_frame
{
	canid_t     can_id;
	string      timestamp;
	uint8_t     data[FRAME_LENGTH];
};

void parseData(stringstream &ss, can_frame &frame) {
    // Line format: 705#39C2A37B95B17C57
    // Need to take: ^         ^
    string token;
    int index = 0; // 0 -> id; 1 -> data
    while (getline(ss, token, '#')) {
        if (index == 0) {
            // this is how we read a hex number, %x is the hex format in C (similar to %d for decimal)
            sscanf(token.c_str(), "%x", &frame.can_id);
        } else {
            // now we need to break down the data into FRAME_LENGTH chunks of 1 byte.
            for (int dataIndex = 0, frameDataIndex = 0; dataIndex < token.size(); dataIndex += 2, frameDataIndex++) {
                char buffer[3];
                int length = token.copy(buffer, 2, dataIndex);
                
                // copy does not add the null terminated character at the end so we do it manually
                buffer[length] = '\0';

                // store this into the data array
                sscanf(buffer, "%x", &frame.data[frameDataIndex]);        
            }
        }
        index++;
    }
}
void parseLine(stringstream &ss, can_frame &frame) {
    // Line format: (1705638753.913408) vcan0 705#39C2A37B95B17C57
    // Need to take these out:          ^                 ^        ^
    string token;
    int index = 0; // 0 -> timestamp; 1 -> vcan0; 2 -> id#data
    while (getline(ss, token, ' ')) {
        if (index == 0) {
            frame.timestamp = token;
        } else if (index == 2) {
            stringstream ssData(token);
            parseData(ssData, frame);
        }
        index++;
    }
}
int main()
{
    // load dbc file
    unique_ptr<dbcppp::INetwork> net;
    {
        ifstream idbc("../SensorBus.dbc");
        net = dbcppp::INetwork::LoadDBCFromIs(idbc);
    }

    // this is just a map from id -> the definition of a CAN Frame
    unordered_map<uint64_t, const dbcppp::IMessage*> messages;
    for (const dbcppp::IMessage& msg : net->Messages())
    {
        messages.insert(make_pair(msg.Id(), &msg));
    }

    // Open log file
    ifstream log("../dump.log");

    if (!log.is_open()) {
        cerr << "Failed to open file" << endl;
        return 1;
    }

    string line;


    ofstream output;
    output.open("../output.txt", ios::trunc);

    // Ensure 1 decimal point for wheel values
    output << fixed << setprecision(1);

    // Read and parse the file line by line
    while (getline(log, line)) {
        stringstream ss(line);
        can_frame frame;
        parseLine(ss, frame);

        auto iter = messages.find(frame.can_id);
        if (iter != messages.end())
        {
            const dbcppp::IMessage* msg = iter->second;
            // cout << "Received Message: " << msg->Name() << "\n";
            for (const dbcppp::ISignal& sig : msg->Signals())
            {
                if (sig.Name() == "WheelSpeedFR" || sig.Name() == "WheelSpeedRR") {
                    uint64_t rawValue = sig.Decode(frame.data);
                    double physValue = sig.RawToPhys(rawValue);
                    output << frame.timestamp << ": " << sig.Name() << ": " << physValue << endl;
                }
            }
        }
    }

    // Close the file
    log.close();
    output.close();
}