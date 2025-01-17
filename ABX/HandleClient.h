#ifndef HANDLECLIENTSIDE_H
#define HANDLECLIENTSIDE_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
class HandleClientSide
{
public:
    HandleClientSide(std::string_view,std::string_view);
    void ConnectToServer();
    int sendData(char callType,uint8_t resendSeq = 0)const;
    int recieveData(char* buffer,int bufferSize)const;
    void closeConnection()const;
    ~HandleClientSide(); 
private:
    std::string serverAddress;
    std::string port;
    int sockFd;   
};

#endif // HANDLECLIENTSIDE_H
