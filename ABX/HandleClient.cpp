#include "HandleClient.h"
#include <iostream>
void HandleClientSide::ConnectToServer()
{
    sockFd=socket(AF_INET, SOCK_STREAM, 0);
    if(sockFd<0)
    {
        std::perror("Socket creation failed");
        throw std::runtime_error("Socket creation failed");
    }
    sockaddr_in serverAdress{};
    serverAdress.sin_family=AF_INET;
    serverAdress.sin_port=htons(std::stoi(port));
    serverAdress.sin_addr.s_addr=inet_addr(serverAddress.c_str());
    //serverAdress.sin_addr.s_addr = INADDR_ANY;
    if(inet_pton(AF_INET,serverAddress.c_str(),&serverAdress.sin_addr)<=0)
    {
        std::cerr<<"Invalid Address"<<std::endl;
        throw std::runtime_error("Invalid Address");
    }
    if (connect(sockFd, (struct sockaddr*)&serverAdress, sizeof(serverAdress))) 
    {
        std::perror("Connection to server failed");
        throw std::runtime_error("Connection failed");
    }
    std::cout << "Connected to server at " << serverAddress << ":" << port << std::endl;
}
int HandleClientSide::sendData(char callType,uint8_t resendSeq)const
{
    char request[2]={callType,static_cast<char>(resendSeq)};
    int buffRead=send(sockFd,request,sizeof(request),0);
    if(buffRead<0)
    {
        std::perror("Error in sending data");
        throw std::runtime_error("data sending error");
    }
    return buffRead;
}
int HandleClientSide::recieveData(char* buffer,int bufferSize)const
{
    int buffRead=recv(sockFd,buffer,bufferSize,0);
    if(buffRead<=0)
    {
        std::perror("Error in reciveing data");
        throw std::runtime_error("data recieving error");
    }
    return buffRead;
}

void HandleClientSide::closeConnection()const
{
    if (sockFd >= 0)
    {
        std::cout << "Closing connection " << std::endl;
        close(sockFd);
    }
    else
	std::cout<<"Sock FD invalid: "<<sockFd<<std::endl;
}
//constructor
HandleClientSide::HandleClientSide(std::string_view serverAddress,std::string_view port)
                        :serverAddress{serverAddress},port{port},sockFd{-1}
                        {
                        }

//destructors
HandleClientSide::~HandleClientSide()
{
   closeConnection();
}
