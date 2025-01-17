#ifndef HANDLEPACKET_H
#include <vector>
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
        char symbol[4];
        char buySell;
        int quantity;
        int price;
        int sequence;
    };
    #pragma pack()
    void addPacket(const char* buffer);
    void writeToOutFile(std::string_view fileName)const;
    Packet parsePacket(const char * buffer)const;
private:
    std::vector<Packet> packets{};
};

#endif // HANDLEPACKET_H
