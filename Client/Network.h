#ifndef NETWORK_H
#define NETWORK_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "Errors.h"
#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define MAX_BUF_LEN 1024
#define DEFAULT_PORT "27015"
#define LOCAL_HOST "127.0.0.1"

BOOL StartupConnection(const char* IPAddress, const char* port);
SOCKET CreateConnectSocket();
int GetServerMessage(SOCKET ConnectSocket, char** message);
BOOL SendServerMessage(SOCKET ConnectSocket, char* message, int message_len);
void CloseConnection(SOCKET ConnectSocket);

struct addrinfo* result;

BOOL StartupConnection(const char* IPAddress, const char* port)
{
    WSADATA wsaData;

    struct addrinfo hints;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        printError(TEXT("\nInitialize Winsock"), WSAGetLastError());
        return FALSE;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_protocol   = IPPROTO_TCP;

    int IPResult = 1;
    if(IPAddress == NULL)
        IPResult = getaddrinfo(LOCAL_HOST, DEFAULT_PORT, &hints, &result);
    else{
        if(port == NULL)
            IPResult = getaddrinfo(IPAddress, DEFAULT_PORT, &hints, &result);
        else
            IPResult = getaddrinfo(IPAddress, port, &hints, &result);
    }
    if(IPResult != 0){
        printError(TEXT("\nResolve server addres and port"), WSAGetLastError());
        WSACleanup();
        return FALSE;
    }
    return TRUE;
}

SOCKET CreateConnectSocket()
{
    SOCKET ConnectSocket = INVALID_SOCKET;

    for(struct addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next){
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(ConnectSocket == INVALID_SOCKET){
            printError(TEXT("\nSocket"), WSAGetLastError());
            WSACleanup();
            return INVALID_SOCKET;
        }

        if(connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR){
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if(ConnectSocket == INVALID_SOCKET){
        printError(TEXT("\nUnable to connect to server"), WSAGetLastError());
        WSACleanup();
        return INVALID_SOCKET;
    }
    return ConnectSocket;
}

int GetServerMessage(SOCKET ConnectSocket, char** message)
{
    char* buf = malloc(MAX_BUF_LEN);
    int buf_len = MAX_BUF_LEN;
    int message_len = -1;

    message_len = recv(ConnectSocket, (char*)buf, buf_len, 0);
    if(message_len <= 0) {
        printf("\nConnecting closed!\n");
        return 0;
    }
    
    *message = buf;
    return message_len;
}

BOOL SendServerMessage(SOCKET ConnectSocket, char* message, int message_len)
{
    message_len = send(ConnectSocket, message, message_len, 0);
    if(message_len == SOCKET_ERROR){
        printError("\nMessage sending", WSAGetLastError());
        return FALSE;
    }

    return TRUE;
}

void CloseConnection(SOCKET ConnectSocket)
{
    closesocket(ConnectSocket);
    WSACleanup();
}

#endif

