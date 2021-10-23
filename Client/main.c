#include "Network.h"
#include "Client.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_BUF_LEN 1024

int __cdecl main(int argc, char** argv)
{
    printf("\nConnection to server...\n\n");
    StartupConnection(NULL, NULL);
    SOCKET ConnectSocket = CreateConnectSocket(argc, argv[0], argv[1]);
    printf("\nServer connected!\n\n");

    DWORD* ProcessList = (DWORD*)malloc(MAX_BUF_LEN);
    int ProcessCount;
    ProcessCount = GetProcessList(ProcessList, MAX_BUF_LEN / sizeof(DWORD));
    char* message = (char*)malloc(ProcessCount * sizeof(DWORD));
    int message_len = ProcessCount * sizeof(DWORD);

    printf("\nBytes recived: %d\n", message_len);
    printf("\nProcesses number: %d\n", ProcessCount);

    for(int i = 0; i < ProcessCount; i++)
        printf("\t%d\n", ProcessList[i]);
    printf("\n\n");

    memcpy(message, ProcessList, message_len);
    SendServerMessage(ConnectSocket, message, message_len);
    free(ProcessList);
    free(message);

    message_len = GetServerMessage(ConnectSocket, &message);
    DWORD ProcessID;
    memcpy(&ProcessID, message, sizeof(DWORD));
    free(message);

    printf("\nClose proces (ID): %d  (y/n)?\n", ProcessID);
    if(getchar() == 'y'){
        if(CloseProcess(ProcessID)){
            printf("\nProcess closed!\n");
            SendServerMessage(ConnectSocket, "Process closed!", 16);
        }
        else{
            printf("\nProcess not closed!\n");
            SendServerMessage(ConnectSocket, "Process not closed!", 20);
        }
    }
    else{
        printf("\nProcess not closed!\n");
        SendServerMessage(ConnectSocket, "Process not closed", 19);
    }

    CloseConnection(ConnectSocket);

    printf("\nConnection closed!\n");

    getchar();
    getchar();
    return 0;
}