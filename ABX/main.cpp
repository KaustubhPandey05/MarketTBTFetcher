#include "HandlePacket.h"
constexpr short BUFFER_SIZE {1024};
const std::string PORT{"3000"};
const std::string outputFileName = "/home/kaus/OUT_FILES/dump.json";
const std::string serverAddress = "127.0.0.1";
void FetchMTBT(std::string_view serverIP,std::string_view port,std::string_view outFileName)
{
    HandleClientSide client(serverIP,port);
    HandlePacket handlePacket;
    int iPacketChunk{};
    std::cout<<"Starting the client\n";
    client.ConnectToServer();
    client.sendData(1);
    char buffer[BUFFER_SIZE];
    while(client.recieveData(buffer,BUFFER_SIZE)>0)
    {
	std::cout<<"Data in Buffer: "<<buffer<<std::endl;
	char onePacket[sizeof(HandlePacket::Packet)];
	std::memcpy(onePacket,buffer+iPacketChunk,sizeof(onePacket));
	std::cout<<"Single Packet data: "<<onePacket<<std::endl;
	handlePacket.addPacket(buffer);
	iPacketChunk+=sizeof(HandlePacket::Packet);
    }
    std::cout << "All packets received. Closing connection." << std::endl;
    client.closeConnection();
    handlePacket.writeToOutFile(outFileName);
}
int main()
{
    std::cout<<"Starting Fetch\n";
    FetchMTBT(serverAddress,PORT,outputFileName);	
    return 0;
}
