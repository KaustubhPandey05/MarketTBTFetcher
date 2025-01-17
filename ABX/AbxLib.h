#ifndef ABX_H
#define ABX_H
#include <arpa/inet.h>
#include <cstring>
namespace AbxLib {
    // Convert a 4-byte big-endian integer to host byte order
    inline int TwiddleLong(const char* buffer) 
    {
        int value;
        std::memcpy(&value, buffer, sizeof(int));
        return ntohl(value);
    }
}
#endif
