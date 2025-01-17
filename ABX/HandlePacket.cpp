#include "HandlePacket.h"
void HandlePacket::addPacket(const char* buffer) 
{ 
     std::cout << "Adding packet to processor..." << std::endl;
     Packet packet = parsePacket(buffer);
     packets.push_back(packet);
     std::cout << "Packet added: Sequence=" << packet.sequence << std::endl;
}
HandlePacket::Packet HandlePacket::parsePacket(const char* buffer)const
{
    Packet packet;
    std::cout<<"Data recieved in HandlePacket: "<<buffer<<std::endl;
    std::memcpy(packet.symbol,buffer, sizeof(packet.symbol)); // Symbol (4 bytes)
    packet.buySell = buffer[4]; // Buy/Sell Indicator (1 byte)
    packet.quantity = AbxLib::TwiddleLong(buffer + 5); // Quantity (4 bytes)
    packet.price = AbxLib::TwiddleLong(buffer + 9); // Price (4 bytes)
    packet.sequence = AbxLib::TwiddleLong(buffer + 13); // Sequence (4 bytes)

    std::cout << "Parsed packet: Symbol=" << packet.symbol << ", BuySell=" << packet.buySell
              << ", Quantity=" << packet.quantity << ", Price=" << packet.price
              << ", Sequence=" << packet.sequence << std::endl;

    return packet;
}
void HandlePacket::writeToOutFile(std::string_view fileName)const
{
     std::string tempFileName(fileName);
     std::ofstream outFile(tempFileName);
     outFile<<"[\n";
     for(const auto& packet:packets)
	{
	     outFile << " {\n"
		     << " \"symbol\":\""<<packet.symbol<<"\",\n"
		     << " \"buySell\":\""<<packet.buySell<<"\",\n"
		     << " \"quantity\":"<<packet.quantity<<",\n"
		     << " \"price\":"<<packet.price<<",\n"
		     << " \"sequence\":"<<packet.sequence<<"\n"
		     << " }\n";
	     
	}
     outFile<<"]";
     std::cout << "Data written to " << fileName << " successfully." << std::endl;
}
