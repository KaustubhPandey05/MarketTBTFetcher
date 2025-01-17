#ifndef HANDLEPACKET_H
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "HandleClient.h"
#include "AbxLib.h"
#include <iostream>
class HandlePacket
{
public:
    #pragma pack(1)
    struct Packet 
    {
        char symbol[5];
        char buySell;
        int quantity;
        int price;
        int sequence;

    };
    #pragma pack()
    void addPacket(const char* buffer);
    void writeToOutFile(std::string_view fileName)const;
    Packet parsePacket(const char * buffer)const;
    void sortPackets();
    std::vector<int> getSeqMisMatch();
    void initiateRecovery(std::string_view serverAddress,std::string_view port);
private:
    std::vector<Packet> packets{};
    std::map<int, bool> receivedPackets;
};

#endif // HANDLEPACKET_H
