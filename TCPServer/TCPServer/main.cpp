#include <iostream>
#include <WS2tcpip.h>
#include <winsock2.h>
#include <ostream>

#pragma comment (lib, "ws2_32.lib")

void main()
{
    //init Winsock
    WSADATA wsaData;
    int result;

    // Initialize Winsock (version 2.2)
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return;
    }

    //create socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

    if (listening = INVALID_SOCKET)
    {
        std::cerr << " failed to create socket, exiting...\n";
        WSACleanup();
        return;
    }

    //bind the socket to a port & ip address 
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    //INET_PTON could also be used

    result = bind(listening, (sockaddr*)&hint, sizeof(hint));

    if (result == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listening);
        WSACleanup();
        return;
    }

    //tell winsocket the socket is for listening
    result = listen(listening, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listening);
        WSACleanup();
        return;
    }


    //wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
        closesocket(listening);
        WSACleanup();
        return;
    }

    //remote name of client
    char host[NI_MAXHOST];

    //service client is connected on
    char service[NI_MAXHOST];

    ZeroMemory(host, NI_MAXHOST); //windows version of memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXHOST);

    if (getnameinfo((sockaddr*) & client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << "Connected on Port: " << service << "\n";
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << "Host connected on Port: " << ntohs(client.sin_port) << "\n";
    }

	//close listening socket
    closesocket(listening);

	//while Loop: accept and echo message back to client
    char buf[4096];

    while (true)
    {
        ZeroMemory(buf, 4096);

        //wait for client to send data
        int bytesRecieved = recv(clientSocket, buf, 4096, 0);

        if (bytesRecieved == SOCKET_ERROR)
        {
            std::cerr << "Error in recieving. exiting \n";
            break;
        }

        if (bytesRecieved == 0)
        {
            std::cout << "client disconnected\n";
            break;
        }

        //echo message
        send(clientSocket, buf, bytesRecieved + 1, 0);
    }


	//close socket
    closesocket(clientSocket);
    //shutdown/cleanup winsock
    WSACleanup();
	
}