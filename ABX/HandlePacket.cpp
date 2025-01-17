#include "HandlePacket.h"
void HandlePacket::addPacket(const char* buffer) 
{ 
     std::cout << "Adding packet to processor..." << std::endl;
     Packet packet = parsePacket(buffer);
     packets.push_back(packet);
     receivedPackets[packet.sequence] = true;
     std::cout << "Packet added: Sequence=" << packet.sequence << std::endl;
}
HandlePacket::Packet HandlePacket::parsePacket(const char* buffer)const
{
    Packet packet;
    std::cout<<"Data recieved in HandlePacket: "<<buffer<<std::endl;
    std::memcpy(packet.symbol,buffer,sizeof(packet.symbol)); // Symbol (4 bytes)
    packet.symbol[4]='\0';
    packet.buySell = buffer[4]; // Buy/Sell Indicator (1 byte)
    packet.quantity = AbxLib::TwiddleLong(buffer + 5); // Quantity (4 bytes)
    packet.price = AbxLib::TwiddleLong(buffer + 9); // Price (4 bytes)
    packet.sequence = AbxLib::TwiddleLong(buffer + 13); // Sequence (4 bytes)

    std::cout << "Parsed packet: Symbol=" << packet.symbol << ", BuySell=" << packet.buySell
              << ", Quantity=" << packet.quantity << ", Price=" << packet.price
              << ", Sequence=" << packet.sequence << std::endl;

    return packet;
}
std::vector<int> HandlePacket::getSeqMisMatch()
{
     std::vector<int> missingSequences;
     if (packets.empty()) 
     {
		std::cout << "No packets received yet." << std::endl;
		return missingSequences;
     }

     int maxSequence = packets.back().sequence;
     std::cout << "Checking for missing sequences up to " << maxSequence << std::endl;

     for (int seq = 1; seq < maxSequence; ++seq) 
     {
        if (receivedPackets.find(seq) == receivedPackets.end()) 
	{
            missingSequences.push_back(seq);
            std::cout << "Missing sequence detected: " << seq << std::endl;
	    receivedPackets[seq]=true;
        }
     }
     return missingSequences;
}
void HandlePacket::initiateRecovery(std::string_view serverAddress,std::string_view port)
{
    std::vector<int> missingSequences=getSeqMisMatch();
    if(missingSequences.empty())
     {
	std::cout<<"No Missing packets\n";
	  return;
     }
    for (int seq : missingSequences) 
    {
	char buffer[1024];
	HandleClientSide resendClient(serverAddress,port);
        resendClient.ConnectToServer();
        resendClient.sendData(2, seq);
	std::cout<<"Initiaiting recovery for seq "<<seq<<std::endl;
	if (resendClient.recieveData(buffer, 17) == 17)
	{
	    addPacket(buffer);
	}
	else
	{
	    std::cerr << "Failed to receive missing packet for sequence: " << seq << std::endl;
	}
	resendClient.closeConnection();
    }
    sortPackets();
    initiateRecovery(serverAddress,port);
	
}
void HandlePacket::sortPackets() 
{
    std::sort(packets.begin(), packets.end(), [](const Packet& a,const Packet& b) {
        return a.sequence < b.sequence;
    });
    std::cout << "Packets sorted." << std::endl;
}
void HandlePacket::writeToOutFile(std::string_view fileName)const//Alt json lib can be used but cant be bothered with c++20
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


