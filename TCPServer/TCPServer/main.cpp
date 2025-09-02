#include <iostream>
#include <WS2tcpip.h>
#include <winsock2.h>

#pragma comment (lib, "ws3_32.lib")

void main()
{
	//init Winsock
    WSADATA wsaData;
    int result;

    // Initialize Winsock (version 2.2)
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }

	//create socket

	//bind the socket to an ip address and port



	//close socket
    WSACleanup();
	//shutdown winsock
}