#ifndef NETWORK_H
#define NETWORK_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "Errors.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#define MAX_BUF_LEN 1024
#define DEFAULT_PORT "27015"

SOCKET CreateListenSocket();
SOCKET CreateClientSocket(SOCKET ListenSocket);
int GetClientMessage(SOCKET ClientSocket, char** message);
BOOL SendClientMessage(SOCKET ClientSocket, char* message, int message_len);
void CloseConnection(SOCKET ListeSocket, SOCKET ClientSocket);

SOCKET CreateListenSocket()
{
    SOCKET ListenSocket = INVALID_SOCKET;

    WSADATA wsaData;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_protocol   = IPPROTO_TCP;
    hints.ai_flags      = AI_PASSIVE;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        printError("\nInitialize Winsock", WSAGetLastError());
        return INVALID_SOCKET;
    }


    if((INT)getaddrinfo(NULL, DEFAULT_PORT, &hints, &result) != 0){
        printError("\nResolve server addres and port", WSAGetLastError());
        WSACleanup();
        return INVALID_SOCKET;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(ListenSocket == INVALID_SOCKET){
        printError("\nCreating ListenSocket", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return INVALID_SOCKET;
    }

    if(bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR){
        printError("\nSetup TCP", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    freeaddrinfo(result);

    return ListenSocket;
}

SOCKET CreateClientSocket(SOCKET ListenSocket)
{
    SOCKET ClientSocket = INVALID_SOCKET;

    if(listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR){
        printError("\nListen socket", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    ClientSocket = accept(ListenSocket, NULL, NULL);
    if(ClientSocket == INVALID_SOCKET){
        printError("\nClient socket", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return ClientSocket;
}

int GetClientMessage(SOCKET ClientSocket, char** message)
{
    char* buf = malloc(MAX_BUF_LEN);
    int buf_len = MAX_BUF_LEN;
    int message_len = -1;

    message_len = recv(ClientSocket, (char*)buf, buf_len, 0);
    if(message_len <= 0) {
        printf("\nConnecting closed!\n");
        return 0;
    }
    
    *message = buf;
    return message_len;
} 

BOOL SendClientMessage(SOCKET ClientSocket, char* message, int message_len)
{
    message_len = send(ClientSocket, message, message_len, 0);
    if(message_len == SOCKET_ERROR){
        printError("\nMessage sending", WSAGetLastError());
        return FALSE;
    }

    return TRUE;
}

void CloseConnection(SOCKET ListeSocket, SOCKET ClientSocket)
{
    closesocket(ListeSocket);
    closesocket(ClientSocket);
    WSACleanup();
}

#endif